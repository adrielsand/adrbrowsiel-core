/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import "adrbrowsiel/ios/app/adrbrowsiel_core_main.h"

#import <UIKit/UIKit.h>

#include "base/compiler_specific.h"
#include "base/strings/sys_string_conversions.h"
#include "adrbrowsiel/ios/app/adrbrowsiel_main_delegate.h"
#import "adrbrowsiel/ios/browser/adrbrowsiel_web_client.h"
#include "ios/chrome/app/startup/provider_registration.h"
#import "ios/chrome/app/startup_tasks.h"
#include "ios/chrome/browser/application_context.h"
#include "ios/chrome/browser/browser_state/chrome_browser_state.h"
#include "ios/chrome/browser/browser_state/chrome_browser_state_manager.h"
#include "ios/public/provider/chrome/browser/chrome_browser_provider.h"
#include "ios/web/public/init/web_main.h"

@interface adrbrowsielCoreMain () {
  std::unique_ptr<adrbrowsielWebClient> _webClient;
  std::unique_ptr<adrbrowsielMainDelegate> _delegate;
  std::unique_ptr<web::WebMain> _webMain;
  ChromeBrowserState* _mainBrowserState;
}
@end

@implementation adrbrowsielCoreMain

- (instancetype)init {
  return [self initWithSyncServiceURL:@""];
}

- (instancetype)initWithSyncServiceURL:(NSString*)syncServiceURL {
  if ((self = [super init])) {
    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(onAppEnterBackground:)
               name:UIApplicationDidEnterBackgroundNotification
             object:nil];
    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(onAppEnterForeground:)
               name:UIApplicationWillEnterForegroundNotification
             object:nil];
    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(onAppWillTerminate:)
               name:UIApplicationWillTerminateNotification
             object:nil];

    // Register all providers before calling any Chromium code.
    [ProviderRegistration registerProviders];

    _webClient.reset(new adrbrowsielWebClient());
    web::SetWebClient(_webClient.get());

    _delegate.reset(new adrbrowsielMainDelegate());
    _delegate->SetSyncServiceURL(base::SysNSStringToUTF8(syncServiceURL));

    web::WebMainParams params(_delegate.get());
    _webMain = std::make_unique<web::WebMain>(std::move(params));

    ios::GetChromeBrowserProvider()->Initialize();

    ios::ChromeBrowserStateManager* browserStateManager =
        GetApplicationContext()->GetChromeBrowserStateManager();
    ChromeBrowserState* chromeBrowserState =
        browserStateManager->GetLastUsedBrowserState();
    _mainBrowserState = chromeBrowserState;
  }
  return self;
}

- (void)onAppEnterBackground:(NSNotification*)notification {
  auto* context = GetApplicationContext();
  if (context)
    context->OnAppEnterBackground();
}

- (void)onAppEnterForeground:(NSNotification*)notification {
  auto* context = GetApplicationContext();
  if (context)
    context->OnAppEnterForeground();
}

- (void)onAppWillTerminate:(NSNotification*)notification {
  _webMain.reset();
}

- (void)scheduleLowPriorityStartupTasks {
  [StartupTasks scheduleDeferredBrowserStateInitialization:_mainBrowserState];
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  _mainBrowserState = nullptr;
  _webMain.reset();
  _delegate.reset();
  _webClient.reset();
}

- (void)setUserAgent:(NSString*)userAgent {
  _webClient->SetUserAgent(base::SysNSStringToUTF8(userAgent));
}

@end
