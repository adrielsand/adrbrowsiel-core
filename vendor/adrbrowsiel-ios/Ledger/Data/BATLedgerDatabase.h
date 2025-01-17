/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#import <Foundation/Foundation.h>
#import "ledger.mojom.objc.h"
#import "CoreDataModels.h"

NS_ASSUME_NONNULL_BEGIN

typedef void (^BATLedgerDatabaseWriteCompletion)(BOOL success);

/// An interface into the ledger database
///
/// This class mirrors adrbrowsiel-core's `publisher_info_database.h/cc` file. This file will actually
/// likely be removed at a future date when database managment happens in the ledger library
OBJC_EXPORT
@interface BATLedgerDatabase : NSObject

/// Generates a SQL migration transaction that will move all data in the users
/// CoreData storage into version 10 of the adrbrowsiel-core's database schema to
/// then run and have ledger take over
///
/// Return's nil if the migration template cannot be found
+ (nullable NSString *)migrateCoreDataToSQLTransaction;

/// Generates a SQL migration transaction that will move token related tables
/// only (promos, promo creds, unblinded tokens) to version 10 of adrbrowsiel-core's
/// database schema.
///
/// Return's nil if the migration template cannot be found
+ (nullable NSString *)migrateCoreDataBATOnlyToSQLTransaction;

/// Deletes the server publisher list from the CoreData DB
+ (void)deleteCoreDataServerPublisherList:(nullable void (^)(NSError * _Nullable error))completion;

+ (NSString *)activityInfoInsertFor:(ActivityInfo *)info;
+ (NSString *)contributionInfoInsertFor:(ContributionInfo *)info;
+ (NSString *)contributionQueueInsertFor:(ContributionQueue *)obj;
+ (NSString *)contributionQueuePublisherInsertFor:(ContributionPublisher *)obj;
+ (NSString *)mediaPublisherInfoInsertFor:(MediaPublisherInfo *)obj;
+ (NSString *)pendingContributionInsertFor:(PendingContribution *)obj;
+ (NSString *)promotionInsertFor:(Promotion *)obj;
+ (NSString *)promotionCredsInsertFor:(PromotionCredentials *)obj;
+ (NSString *)publisherInfoInsertFor:(PublisherInfo *)obj;
+ (NSString *)recurringDonationInsertFor:(RecurringDonation *)obj;
+ (NSString *)unblindedTokenInsertFor:(UnblindedToken *)obj;

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithCoder:(NSCoder *)aDecoder NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
