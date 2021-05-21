/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_IOS_APP_adrbrowsiel_MAIN_DELEGATE_H_
#define adrbrowsiel_IOS_APP_adrbrowsiel_MAIN_DELEGATE_H_

#include <string>

#include "base/macros.h"
#include "ios/chrome/app/startup/ios_chrome_main_delegate.h"

class adrbrowsielWebClient;

class adrbrowsielMainDelegate : public IOSChromeMainDelegate {
 public:
  adrbrowsielMainDelegate();
  ~adrbrowsielMainDelegate() override;

  void SetSyncServiceURL(const std::string& url);

 protected:
  // web::WebMainDelegate implementation:
  void BasicStartupComplete() override;

 private:
  std::string adrbrowsiel_sync_service_url_;
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielMainDelegate);
};

#endif  // adrbrowsiel_IOS_APP_adrbrowsiel_MAIN_DELEGATE_H_
