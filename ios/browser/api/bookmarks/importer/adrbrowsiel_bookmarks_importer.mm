/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/ios/browser/api/bookmarks/importer/adrbrowsiel_bookmarks_importer.h"

#include <vector>

#include "base/base_paths.h"
#include "base/bind.h"
#include "base/callback_helpers.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/mac/foundation_util.h"
#include "base/path_service.h"
#include "base/sequenced_task_runner.h"
#include "base/strings/sys_string_conversions.h"
#include "base/task/post_task.h"
#include "base/task/thread_pool.h"
#include "adrbrowsiel/ios/browser/api/bookmarks/importer/bookmark_html_reader.h"
#include "adrbrowsiel/ios/browser/api/bookmarks/importer/bookmarks_importer.h"
#include "adrbrowsiel/ios/browser/api/bookmarks/importer/imported_bookmark_entry.h"
#include "ios/web/public/thread/web_task_traits.h"
#include "ios/web/public/thread/web_thread.h"
#import "net/base/mac/url_conversions.h"
#include "url/gurl.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

@implementation adrbrowsielImportedBookmark
- (instancetype)initFromChromiumImportedBookmark:
    (const ImportedBookmarkEntry&)entry {
  if ((self = [super init])) {
    NSMutableArray<NSString*>* paths = [[NSMutableArray alloc] init];
    for (const auto& path : entry.path) {
      [paths addObject:base::SysUTF16ToNSString(path)];
    }

    _inToolbar = entry.in_toolbar;
    _isFolder = entry.is_folder;
    _url = net::NSURLWithGURL(entry.url);
    _path = paths;
    _title = base::SysUTF16ToNSString(entry.title);
    _creationTime =
        [NSDate dateWithTimeIntervalSince1970:entry.creation_time.ToDoubleT()];
  }
  return self;
}

- (ImportedBookmarkEntry)toChromiumImportedBookmark {
  std::vector<std::u16string> paths;
  for (NSString* path in self.path) {
    paths.push_back(base::SysNSStringToUTF16(path));
  }

  ImportedBookmarkEntry entry;
  entry.creation_time =
      base::Time::FromDoubleT([self.creationTime timeIntervalSince1970]);
  entry.url = net::GURLWithNSURL(self.url);
  entry.title = base::SysNSStringToUTF16(self.title);
  entry.in_toolbar = self.inToolbar;
  entry.path = paths;
  return entry;
}
@end

@interface adrbrowsielBookmarksImporter () {
  scoped_refptr<base::SequencedTaskRunner> import_thread_;
}
@property(atomic) bool cancelled;  // atomic
@end

@implementation adrbrowsielBookmarksImporter
- (instancetype)init {
  if ((self = [super init])) {
    self.cancelled = false;

    // Create worker thread in which importer runs.
    // In Chromium, this is created with `base::Thread("import_thread")`
    import_thread_ = base::ThreadPool::CreateSequencedTaskRunner(
        {base::MayBlock(), base::TaskPriority::USER_VISIBLE,
         base::TaskShutdownBehavior::BLOCK_SHUTDOWN});
  }
  return self;
}

- (void)dealloc {
  [self cancel];
}

- (void)cancel {
  self.cancelled = true;
}

- (void)importFromFile:(NSString*)filePath
    topLevelFolderName:(NSString*)folderName
       automaticImport:(bool)automaticImport
          withListener:
              (void (^)(adrbrowsielBookmarksImporterState,
                        NSArray<adrbrowsielImportedBookmark*>* _Nullable))listener {
  base::FilePath source_file_path = base::mac::NSStringToFilePath(filePath);

  // In Chromium, this is IDS_BOOKMARK_GROUP (804)
  std::u16string top_level_folder_name = base::SysNSStringToUTF16(folderName);

  auto start_import = [](adrbrowsielBookmarksImporter* weak_importer,
                         const base::FilePath& source_file_path,
                         const std::u16string& top_level_folder_name,
                         bool automaticImport,
                         std::function<void(adrbrowsielBookmarksImporterState,
                                            NSArray<adrbrowsielImportedBookmark*>*)>
                             listener) {
    // Import cancelled as the importer has been deallocated
    __strong adrbrowsielBookmarksImporter* importer = weak_importer;
    if (!importer) {
      listener(adrbrowsielBookmarksImporterStateStarted, nullptr);
      listener(adrbrowsielBookmarksImporterStateCancelled, nullptr);
      return;
    }

    listener(adrbrowsielBookmarksImporterStateStarted, nullptr);
    std::vector<ImportedBookmarkEntry> bookmarks;
    bookmark_html_reader::ImportBookmarksFile(
        base::BindRepeating(
            [](adrbrowsielBookmarksImporter* importer) -> bool {
              return [importer isImporterCancelled];
            },
            base::Unretained(importer)),
        base::BindRepeating(
            [](adrbrowsielBookmarksImporter* importer, const GURL& url) -> bool {
              return [importer canImportURL:url];
            },
            base::Unretained(importer)),
        source_file_path, &bookmarks, nullptr);

    if (!bookmarks.empty() && ![importer isImporterCancelled]) {
      if (automaticImport) {
        auto complete_import =
            [](std::vector<ImportedBookmarkEntry> bookmarks,
               const std::u16string& top_level_folder_name,
               std::function<void(adrbrowsielBookmarksImporterState,
                                  NSArray<adrbrowsielImportedBookmark*>*)> listener) {
              BookmarksImporter::AddBookmarks(top_level_folder_name, bookmarks);
              listener(adrbrowsielBookmarksImporterStateAutoCompleted, nullptr);
            };

        // Import into the Profile/ChromeBrowserState on the main-thread.
        base::PostTask(FROM_HERE, {web::WebThread::UI},
                       base::BindOnce(complete_import, std::move(bookmarks),
                                      top_level_folder_name, listener));
      } else {
        listener(adrbrowsielBookmarksImporterStateCompleted,
                 [importer convertToIOSImportedBookmarks:bookmarks]);
      }
    } else {
      listener(adrbrowsielBookmarksImporterStateCancelled, nullptr);
    }
  };

  // Run the importer on the sequenced task runner.
  __weak adrbrowsielBookmarksImporter* weakSelf = self;
  import_thread_->PostTask(
      FROM_HERE,
      base::BindOnce(start_import, weakSelf, source_file_path,
                     top_level_folder_name, automaticImport, listener));
}

- (void)importFromArray:(NSArray<adrbrowsielImportedBookmark*>*)bookmarks
     topLevelFolderName:(NSString*)folderName
           withListener:(void (^)(adrbrowsielBookmarksImporterState))listener {
  // In Chromium, this is IDS_BOOKMARK_GROUP (804)
  std::u16string top_level_folder_name = base::SysNSStringToUTF16(folderName);

  auto start_import =
      [](adrbrowsielBookmarksImporter* weak_importer,
         NSArray<adrbrowsielImportedBookmark*>* bookmarks,
         const std::u16string& top_level_folder_name,
         std::function<void(adrbrowsielBookmarksImporterState)> listener) {
        // Import cancelled as the importer has been deallocated
        __strong adrbrowsielBookmarksImporter* importer = weak_importer;
        if (!importer) {
          listener(adrbrowsielBookmarksImporterStateStarted);
          listener(adrbrowsielBookmarksImporterStateCancelled);
          return;
        }

        listener(adrbrowsielBookmarksImporterStateStarted);
        BookmarksImporter::AddBookmarks(
            top_level_folder_name,
            [importer convertToChromiumImportedBookmarks:bookmarks]);
        listener(adrbrowsielBookmarksImporterStateCompleted);
      };

  // Import into the Profile/ChromeBrowserState on the main-thread.
  __weak adrbrowsielBookmarksImporter* weakSelf = self;
  base::PostTask(FROM_HERE, {web::WebThread::UI},
                 base::BindOnce(start_import, weakSelf, bookmarks,
                                top_level_folder_name, listener));
}

// MARK: - Private

- (bool)isImporterCancelled {
  return self.cancelled;
}

// Returns true if |url| has a valid scheme that we allow to import. We
// filter out the URL with a unsupported scheme.
- (bool)canImportURL:(const GURL&)url {
  // The URL is not valid.
  if (!url.is_valid()) {
    return false;
  }

  // Filter out the URLs with unsupported schemes.
  const char* const kInvalidSchemes[] = {"wyciwyg", "place", "about", "chrome"};
  for (size_t i = 0; i < base::size(kInvalidSchemes); ++i) {
    if (url.SchemeIs(kInvalidSchemes[i])) {
      return false;
    }
  }

  return true;
}

// Converts an array of Chromium imported bookmarks to iOS imported bookmarks.
- (NSArray<adrbrowsielImportedBookmark*>*)convertToIOSImportedBookmarks:
    (const std::vector<ImportedBookmarkEntry>&)bookmarks {
  NSMutableArray<adrbrowsielImportedBookmark*>* results =
      [[NSMutableArray alloc] init];
  for (const auto& bookmark : bookmarks) {
    adrbrowsielImportedBookmark* imported_bookmark = [[adrbrowsielImportedBookmark alloc]
        initFromChromiumImportedBookmark:bookmark];
    [results addObject:imported_bookmark];
  }
  return results;
}

// Converts an array of iOS imported bookmarks to Chromium imported bookmarks.
- (std::vector<ImportedBookmarkEntry>)convertToChromiumImportedBookmarks:
    (NSArray<adrbrowsielImportedBookmark*>*)bookmarks {
  std::vector<ImportedBookmarkEntry> results;
  for (adrbrowsielImportedBookmark* bookmark in bookmarks) {
    results.push_back([bookmark toChromiumImportedBookmark]);
  }
  return results;
}
@end