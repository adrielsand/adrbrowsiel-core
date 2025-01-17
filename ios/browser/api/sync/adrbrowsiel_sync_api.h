/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_IOS_BROWSER_API_SYNC_adrbrowsiel_SYNC_API_H_
#define adrbrowsiel_IOS_BROWSER_API_SYNC_adrbrowsiel_SYNC_API_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

OBJC_EXPORT
@interface adrbrowsielSyncAPI : NSObject

@property(class, readonly, strong)
    adrbrowsielSyncAPI* sharedSyncAPI NS_SWIFT_NAME(shared);
@property(nonatomic) bool syncEnabled;
@property(nonatomic, readonly) bool isSyncFeatureActive;

- (void)resetSync;

- (void)deleteDevice:(NSString*)guid;

- (bool)isValidSyncCode:(NSString*)syncCode;

- (NSString*)getSyncCode;

// returns false is sync is already configured or if the sync code is invalid
- (bool)setSyncCode:(NSString*)syncCode;

- (NSString*)syncCodeFromHexSeed:(NSString*)hexSeed;

- (nullable UIImage*)getQRCodeImage:(CGSize)size;

- (nullable NSString*)getDeviceListJSON;

- (id)createSyncDeviceObserver:(void (^)())onDeviceInfoChanged;
- (id)createSyncServiceObserver:(void (^)())onSyncServiceStateChanged;
@end

NS_ASSUME_NONNULL_END

#endif  // adrbrowsiel_IOS_BROWSER_API_SYNC_adrbrowsiel_SYNC_API_H_
