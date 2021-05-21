/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_IOS_BROWSER_API_BOOKMARKS_IMPORTER_adrbrowsiel_BOOKMARKS_IMPORTER_H_
#define adrbrowsiel_IOS_BROWSER_API_BOOKMARKS_IMPORTER_adrbrowsiel_BOOKMARKS_IMPORTER_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, adrbrowsielBookmarksImporterState) {
  adrbrowsielBookmarksImporterStateCompleted,
  adrbrowsielBookmarksImporterStateAutoCompleted,
  adrbrowsielBookmarksImporterStateStarted,
  adrbrowsielBookmarksImporterStateCancelled
};

OBJC_EXPORT
@interface adrbrowsielImportedBookmark : NSObject
@property(nonatomic, readonly) bool inToolbar;
@property(nonatomic, readonly) bool isFolder;
@property(nullable, nonatomic, readonly, copy) NSURL* url;
@property(nullable, nonatomic, readonly, copy) NSArray<NSString*>* path;
@property(nonatomic, readonly, copy) NSString* title;
@property(nonatomic, readonly, copy) NSDate* creationTime;
@end

OBJC_EXPORT
@interface adrbrowsielBookmarksImporter : NSObject
- (instancetype)init;

- (void)cancel;

- (void)importFromFile:(NSString*)filePath
    topLevelFolderName:(NSString*)folderName
       automaticImport:(bool)automaticImport
          withListener:
              (void (^)(adrbrowsielBookmarksImporterState,
                        NSArray<adrbrowsielImportedBookmark*>* _Nullable))listener;

- (void)importFromArray:(NSArray<adrbrowsielImportedBookmark*>*)bookmarks
     topLevelFolderName:(NSString*)folderName
           withListener:(void (^)(adrbrowsielBookmarksImporterState))listener;
@end

NS_ASSUME_NONNULL_END

#endif  // adrbrowsiel_IOS_BROWSER_API_BOOKMARKS_IMPORTER_adrbrowsiel_BOOKMARKS_IMPORTER_H_
