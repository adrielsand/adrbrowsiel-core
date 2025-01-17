/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_IOS_BROWSER_API_BOOKMARKS_adrbrowsiel_BOOKMARKS_OBSERVER_H_
#define adrbrowsiel_IOS_BROWSER_API_BOOKMARKS_adrbrowsiel_BOOKMARKS_OBSERVER_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class IOSBookmarkNode;

OBJC_EXPORT
@protocol BookmarkModelObserver <NSObject>
@optional
- (void)bookmarkModelLoaded;
- (void)bookmarkNodeChanged:(IOSBookmarkNode*)bookmarkNode;
- (void)bookmarkNodeChildrenChanged:(IOSBookmarkNode*)bookmarkNode;
- (void)bookmarkNode:(IOSBookmarkNode*)bookmarkNode
     movedFromParent:(IOSBookmarkNode*)oldParent
            toParent:(IOSBookmarkNode*)newParent;
- (void)bookmarkNodeDeleted:(IOSBookmarkNode*)node
                 fromFolder:(IOSBookmarkNode*)folder;
- (void)bookmarkModelRemovedAllNodes;
- (void)bookmarkNodeFaviconChanged:(IOSBookmarkNode*)bookmarkNode;
@end

OBJC_EXPORT
@protocol BookmarkModelListener
- (void)destroy;
@end

OBJC_EXPORT
@interface BookmarkModelListenerImpl : NSObject <BookmarkModelListener>
- (instancetype)init:(id<BookmarkModelObserver>)observer
       bookmarkModel:(void*)bookmarkModel;
@end

NS_ASSUME_NONNULL_END

#endif /* adrbrowsiel_IOS_BROWSER_API_BOOKMARKS_adrbrowsiel_BOOKMARKS_OBSERVER_H_ */
