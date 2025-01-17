// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#import <Foundation/Foundation.h>

#import "BATadrbrowsielAds.h"
#import "BATadrbrowsielLedger.h"

NS_ASSUME_NONNULL_BEGIN

/// Configuration around adrbrowsiel rewards for ads & ledger
OBJC_EXPORT
NS_SWIFT_NAME(adrbrowsielRewardsConfiguration)
@interface BATadrbrowsielRewardsConfiguration : NSObject <NSCopying>

/// Whether or not rewards is being tested
@property (nonatomic, getter=isTesting) BOOL testing;
//@property (nonatomic, getter=isDebug) BOOL debug;
/// The rewards environment
@property (nonatomic) BATEnvironment environment;
/// The rewards build channel
@property (nonatomic, nullable) BATadrbrowsielAdsBuildChannel *buildChannel;
/// Where ledger and ads should save their state
@property (nonatomic, copy) NSString *stateStoragePath;
/// The number of seconds between overrides. Defaults to 0 (no override) which means reconciles
/// occur every 30 days (see: bat-native-ledger/static_values.h/_reconcile_default_interval)
@property (nonatomic) int overridenNumberOfSecondsBetweenReconcile;
/// Whether or not to enable short retries between contribution attempts
@property (nonatomic) BOOL useShortRetries;

/// The default configuration. Environment is dev, no changes to ads or ledger configuration
///
/// State is stored in Application Support
@property (nonatomic, class, readonly) BATadrbrowsielRewardsConfiguration *defaultConfiguration NS_SWIFT_NAME(default);
/// The staging configuration. Environment is staging, no changes to ads or ledger configuration
///
/// State is stored in Application Support
@property (nonatomic, class, readonly) BATadrbrowsielRewardsConfiguration *stagingConfiguration NS_SWIFT_NAME(staging);
/// The production configuration. Environment is production, no changes to ads or ledger configuration
///
/// State is stored in Application Support
@property (nonatomic, class, readonly) BATadrbrowsielRewardsConfiguration *productionConfiguration NS_SWIFT_NAME(production);
/// The testing configuration. Environment is development & is_testing is set to true. Short retries are enabled,
/// number of seconds between reconciles is set to 30 seconds instead of 30 days.
///
/// State is saved to a directory created in /tmp
@property (nonatomic, class, readonly) BATadrbrowsielRewardsConfiguration *testingConfiguration NS_SWIFT_NAME(testing);

@end

OBJC_EXPORT
NS_SWIFT_NAME(adrbrowsielRewardsDelegate)
@protocol BATadrbrowsielRewardsDelegate <NSObject>
@required

- (void)logMessageWithFilename:(NSString *)file
                    lineNumber:(int)lineNumber
                     verbosity:(int)verbosity
                       message:(NSString *)message;

/// A notification that the ledger service did start
- (void)ledgerServiceDidStart:(BATadrbrowsielLedger *)ledger;

/// Obtain the favicon URL given some page's URL. The client can then choose
/// to download said favicon and cache it for later when `retrieveFavicon` is
/// called.
///
/// If the favicon URL cannot be obtained, call completion with `nil`
- (void)faviconURLFromPageURL:(NSURL *)pageURL
                   completion:(void (^)(NSURL * _Nullable faviconURL))completion;

@end

/// A container for handling adrbrowsiel Rewards. Use `ads` to handle how many ads the users see,
/// when to display them. Use `ledger` to manage interactions between the users wallet & publishers
OBJC_EXPORT
NS_SWIFT_NAME(adrbrowsielRewards)
@interface BATadrbrowsielRewards : NSObject

@property (nonatomic, readonly) BATadrbrowsielAds *ads;
/// Whether or not adrbrowsiel Ads is enabled
@property (nonatomic, assign, getter=isAdsEnabled) BOOL adsEnabled;
@property (nonatomic, readonly, nullable) BATadrbrowsielLedger *ledger;
@property (nonatomic, weak) id<BATadrbrowsielRewardsDelegate> delegate;

/// Resets the ads & ledger (by purging its data). This should likely never be used in production.
- (void)reset;

/// Create a adrbrowsielRewards instance with a given configuration
- (instancetype)initWithConfiguration:(BATadrbrowsielRewardsConfiguration *)configuration;
/// Create a adrbrowsielRewards instance with a given configuration and custom ledger classes for mocking
- (instancetype)initWithConfiguration:(BATadrbrowsielRewardsConfiguration *)configuration
                             delegate:(nullable id<BATadrbrowsielRewardsDelegate>)delegate
                          ledgerClass:(nullable Class)ledgerClass
                             adsClass:(nullable Class)adsClass NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE;

/// Starts the ledger service if it hadn't already been started and calls a completion handler
/// after it has been initialized
- (void)startLedgerService:(nullable void (^)())completion;

@end

OBJC_EXPORT
@interface BATadrbrowsielRewards (Reporting)

/// Report that a tab with a given id was updated
- (void)reportTabUpdated:(NSInteger)tabId
                     url:(NSURL *)url
              faviconURL:(nullable NSURL *)faviconURL
              isSelected:(BOOL)isSelected
               isPrivate:(BOOL)isPrivate;
/// Report that a page has loaded in the current browser tab, and the HTML is available for analysis
///
/// @note Send nil for `adsInnerText` if the load happened due to tabs restoring
///       after app launch
- (void)reportLoadedPageWithURL:(NSURL *)url
             redirectedFromURLs:(NSArray<NSURL *> *)redirectionURLs
                     faviconURL:(nullable NSURL *)faviconURL
                          tabId:(UInt32)tabId
                           html:(NSString *)html
                   adsInnerText:(nullable NSString *)adsInnerText NS_SWIFT_NAME(reportLoadedPage(url:redirectionURLs:faviconUrl:tabId:html:adsInnerText:));
/// Report any XHR load happening in the page
- (void)reportXHRLoad:(NSURL *)url
                tabId:(UInt32)tabId
        firstPartyURL:(nullable NSURL *)firstPartyURL
          referrerURL:(nullable NSURL *)referrerURL NS_SWIFT_NAME(reportXHRLoad(url:tabId:firstPartyURL:referrerURL:));
/// Report posting data to a form?
- (void)reportPostData:(NSData *)postData
                   url:(NSURL *)url
                 tabId:(UInt32)tabId
         firstPartyURL:(nullable NSURL *)firstPartyURL
           referrerURL:(nullable NSURL *)referrerURL NS_SWIFT_NAME(reportPostData(_:url:tabId:firstPartyURL:referrerURL:));;
/// Report that media has started on a tab with a given id
- (void)reportMediaStartedWithTabId:(UInt32)tabId NS_SWIFT_NAME(reportMediaStarted(tabId:));
/// Report that media has stopped on a tab with a given id
- (void)reportMediaStoppedWithTabId:(UInt32)tabId NS_SWIFT_NAME(reportMediaStopped(tabId:));
/// Report that a tab with a given id navigated to a new page in the same tab
- (void)reportTabNavigationWithTabId:(UInt32)tabId NS_SWIFT_NAME(reportTabNavigation(tabId:));
/// Report that a tab with a given id was closed by the user
- (void)reportTabClosedWithTabId:(UInt32)tabId NS_SWIFT_NAME(reportTabClosed(tabId:));

@end

NS_ASSUME_NONNULL_END
