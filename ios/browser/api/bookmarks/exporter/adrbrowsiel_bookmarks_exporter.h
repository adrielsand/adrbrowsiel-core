/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_IOS_BROWSER_API_BOOKMARKS_EXPORTER_adrbrowsiel_BOOKMARKS_EXPORTER_H_
#define adrbrowsiel_IOS_BROWSER_API_BOOKMARKS_EXPORTER_adrbrowsiel_BOOKMARKS_EXPORTER_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, adrbrowsielBookmarksExporterState) {
  adrbrowsielBookmarksExporterStateCompleted,
  adrbrowsielBookmarksExporterStateStarted,
  adrbrowsielBookmarksExporterStateCancelled,
  adrbrowsielBookmarksExporterStateErrorCreatingFile,
  adrbrowsielBookmarksExporterStateErrorWritingHeader,
  adrbrowsielBookmarksExporterStateErrorWritingNodes
};

@class IOSBookmarkNode;

OBJC_EXPORT
@interface adrbrowsielBookmarksExporter : NSObject
- (instancetype)init;

- (void)exportToFile:(NSString*)filePath
        withListener:(void (^)(adrbrowsielBookmarksExporterState))listener;

- (void)exportToFile:(NSString*)filePath
           bookmarks:(NSArray<IOSBookmarkNode*>*)bookmarks
        withListener:(void (^)(adrbrowsielBookmarksExporterState))listener;
@end

NS_ASSUME_NONNULL_END

#endif  // adrbrowsiel_IOS_BROWSER_API_BOOKMARKS_EXPORTER_adrbrowsiel_BOOKMARKS_EXPORTER_H_
