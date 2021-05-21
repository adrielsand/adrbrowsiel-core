/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_IOS_APP_adrbrowsiel_CORE_MAIN_H_
#define adrbrowsiel_IOS_APP_adrbrowsiel_CORE_MAIN_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

OBJC_EXPORT
@interface adrbrowsielCoreMain : NSObject

- (instancetype)init;

- (instancetype)initWithSyncServiceURL:(NSString*)syncServiceURL;

- (void)scheduleLowPriorityStartupTasks;

- (void)setUserAgent:(NSString*)userAgent;

@end

NS_ASSUME_NONNULL_END

#endif  // adrbrowsiel_IOS_APP_adrbrowsiel_CORE_MAIN_H_
