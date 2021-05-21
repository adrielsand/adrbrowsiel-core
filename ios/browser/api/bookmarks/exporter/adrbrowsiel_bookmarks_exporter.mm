/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/ios/browser/api/bookmarks/exporter/adrbrowsiel_bookmarks_exporter.h"
#include "adrbrowsiel/ios/browser/api/bookmarks/adrbrowsiel_bookmarks_api.h"

#include <functional>
#include <vector>

#include "base/base_paths.h"
#include "base/bind.h"
#include "base/callback_helpers.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/guid.h"
#include "base/mac/foundation_util.h"
#include "base/path_service.h"
#include "base/sequenced_task_runner.h"
#include "base/strings/sys_string_conversions.h"
#include "base/task/post_task.h"
#include "base/task/thread_pool.h"
#include "base/values.h"
#include "adrbrowsiel/ios/browser/api/bookmarks/exporter/bookmark_html_writer.h"
#include "adrbrowsiel/ios/browser/api/bookmarks/exporter/bookmarks_encoder.h"
#include "components/bookmarks/browser/bookmark_node.h"
#include "components/strings/grit/components_strings.h"
#include "ios/chrome/browser/application_context.h"
#include "ios/chrome/browser/browser_state/chrome_browser_state.h"
#include "ios/chrome/browser/browser_state/chrome_browser_state_manager.h"
#include "ios/web/public/thread/web_task_traits.h"
#include "ios/web/public/thread/web_thread.h"
#import "net/base/mac/url_conversions.h"
#include "ui/base/l10n/l10n_util.h"
#include "url/gurl.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

class adrbrowsielBookmarksExportObserver : public BookmarksExportObserver {
 public:
  adrbrowsielBookmarksExportObserver(
      std::function<void(adrbrowsielBookmarksExporterState)> on_export_finished);
  void OnExportFinished(Result result) override;

 private:
  std::function<void(adrbrowsielBookmarksExporterState)> _on_export_finished;
};

adrbrowsielBookmarksExportObserver::adrbrowsielBookmarksExportObserver(
    std::function<void(adrbrowsielBookmarksExporterState)> on_export_finished)
    : _on_export_finished(on_export_finished) {}

void adrbrowsielBookmarksExportObserver::OnExportFinished(Result result) {
  switch (result) {
    case Result::kSuccess:
      return _on_export_finished(adrbrowsielBookmarksExporterStateCompleted);
    case Result::kCouldNotCreateFile:
      return _on_export_finished(adrbrowsielBookmarksExporterStateErrorCreatingFile);
    case Result::kCouldNotWriteHeader:
      return _on_export_finished(adrbrowsielBookmarksExporterStateErrorWritingHeader);
    case Result::kCouldNotWriteNodes:
      return _on_export_finished(adrbrowsielBookmarksExporterStateErrorWritingNodes);
    default:
      NOTREACHED();
  }
  delete this;
}

@interface IOSBookmarkNode(BookmarksExporter)
- (void)setNativeParent:(bookmarks::BookmarkNode*)parent;
@end

@interface adrbrowsielBookmarksExporter ()
@end

@implementation adrbrowsielBookmarksExporter

- (instancetype)init {
  if ((self = [super init])) {
  }
  return self;
}

- (void)exportToFile:(NSString*)filePath
        withListener:(void (^)(adrbrowsielBookmarksExporterState))listener {
  auto start_export =
      [](adrbrowsielBookmarksExporter* weak_exporter, NSString* filePath,
         std::function<void(adrbrowsielBookmarksExporterState)> listener) {
        // Export cancelled as the exporter has been deallocated
        __strong adrbrowsielBookmarksExporter* exporter = weak_exporter;
        if (!exporter) {
          listener(adrbrowsielBookmarksExporterStateStarted);
          listener(adrbrowsielBookmarksExporterStateCancelled);
          return;
        }

        DCHECK(GetApplicationContext());

        base::FilePath destination_file_path =
            base::mac::NSStringToFilePath(filePath);

        listener(adrbrowsielBookmarksExporterStateStarted);

        ios::ChromeBrowserStateManager* browserStateManager =
            GetApplicationContext()->GetChromeBrowserStateManager();
        DCHECK(browserStateManager);

        ChromeBrowserState* chromeBrowserState =
            browserStateManager->GetLastUsedBrowserState();
        DCHECK(chromeBrowserState);

        bookmark_html_writer::WriteBookmarks(
            chromeBrowserState, destination_file_path,
            new adrbrowsielBookmarksExportObserver(listener));
      };

  __weak adrbrowsielBookmarksExporter* weakSelf = self;
  base::PostTask(FROM_HERE, {web::WebThread::UI},
                 base::BindOnce(start_export, weakSelf, filePath, listener));
}

- (void)exportToFile:(NSString*)filePath
           bookmarks:(NSArray<IOSBookmarkNode*>*)bookmarks
        withListener:(void (^)(adrbrowsielBookmarksExporterState))listener {
  if ([bookmarks count] == 0) {
    listener(adrbrowsielBookmarksExporterStateStarted);
    listener(adrbrowsielBookmarksExporterStateCompleted);
    return;
  }

  auto start_export =
      [](adrbrowsielBookmarksExporter* weak_exporter, NSString* filePath,
         NSArray<IOSBookmarkNode*>* bookmarks,
         std::function<void(adrbrowsielBookmarksExporterState)> listener) {
        // Export cancelled as the exporter has been deallocated
        __strong adrbrowsielBookmarksExporter* exporter = weak_exporter;
        if (!exporter) {
          listener(adrbrowsielBookmarksExporterStateStarted);
          listener(adrbrowsielBookmarksExporterStateCancelled);
          return;
        }

        listener(adrbrowsielBookmarksExporterStateStarted);
        base::FilePath destination_file_path =
            base::mac::NSStringToFilePath(filePath);

        // Create artificial nodes
        auto bookmark_bar_node = [exporter getBookmarksBarNode];
        auto other_folder_node = [exporter getOtherBookmarksNode];
        auto mobile_folder_node = [exporter getMobileBookmarksNode];

        for (IOSBookmarkNode* bookmark : bookmarks) {
          // We export as the |mobile_bookmarks_node| by default.
          [bookmark setNativeParent:mobile_folder_node.get()];
        }

        auto encoded_bookmarks =
            ios::bookmarks_encoder::Encode(bookmark_bar_node.get(),
                                           other_folder_node.get(),
                                           mobile_folder_node.get());
        bookmark_html_writer::WriteBookmarks(
            std::move(encoded_bookmarks), destination_file_path,
            new adrbrowsielBookmarksExportObserver(listener));
      };

  __weak adrbrowsielBookmarksExporter* weakSelf = self;
  base::ThreadPool::PostTask(
      FROM_HERE,
      {base::MayBlock(), base::TaskPriority::USER_VISIBLE,
       base::TaskShutdownBehavior::BLOCK_SHUTDOWN},
      base::BindOnce(start_export, weakSelf, filePath, bookmarks, listener));
}

// MARK: - Internal artificial nodes used for exporting arbitrary bookmarks to a file

- (std::unique_ptr<bookmarks::BookmarkNode>)getRootNode {
  return std::make_unique<bookmarks::BookmarkNode>(
      /*id=*/0,
      base::GUID::ParseLowercase(bookmarks::BookmarkNode::kRootNodeGuid),
      GURL());
}

- (std::unique_ptr<bookmarks::BookmarkNode>)getBookmarksBarNode {
  auto node = std::make_unique<bookmarks::BookmarkNode>(
      /*id=*/1,
      base::GUID::ParseLowercase(bookmarks::BookmarkNode::kBookmarkBarNodeGuid),
      GURL());
  node->SetTitle(l10n_util::GetStringUTF16(IDS_BOOKMARK_BAR_FOLDER_NAME));
  return node;
}

- (std::unique_ptr<bookmarks::BookmarkNode>)getOtherBookmarksNode {
  auto node = std::make_unique<bookmarks::BookmarkNode>(
      /*id=*/2,
      base::GUID::ParseLowercase(
          bookmarks::BookmarkNode::kOtherBookmarksNodeGuid),
      GURL());
  node->SetTitle(l10n_util::GetStringUTF16(IDS_BOOKMARK_BAR_OTHER_FOLDER_NAME));
  return node;
}

- (std::unique_ptr<bookmarks::BookmarkNode>)getMobileBookmarksNode {
  auto node = std::make_unique<bookmarks::BookmarkNode>(
      /*id=*/3,
      base::GUID::ParseLowercase(
          bookmarks::BookmarkNode::kMobileBookmarksNodeGuid),
      GURL());
  node->SetTitle(l10n_util::GetStringUTF16(IDS_BOOKMARK_BAR_MOBILE_FOLDER_NAME));
  return node;
}
@end
