/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_WAYBACK_MACHINE_adrbrowsiel_WAYBACK_MACHINE_TAB_HELPER_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_WAYBACK_MACHINE_adrbrowsiel_WAYBACK_MACHINE_TAB_HELPER_H_

#include <memory>

#include "base/memory/weak_ptr.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

class adrbrowsielWaybackMachineDelegate;
class PrefService;

class adrbrowsielWaybackMachineTabHelper
    : public content::WebContentsObserver,
      public content::WebContentsUserData<adrbrowsielWaybackMachineTabHelper> {
 public:
  explicit adrbrowsielWaybackMachineTabHelper(content::WebContents* contents);
  ~adrbrowsielWaybackMachineTabHelper() override;

  adrbrowsielWaybackMachineTabHelper(const adrbrowsielWaybackMachineTabHelper&) = delete;
  adrbrowsielWaybackMachineTabHelper& operator=(
      const adrbrowsielWaybackMachineTabHelper&) = delete;

  void set_delegate(std::unique_ptr<adrbrowsielWaybackMachineDelegate> delegate);

  WEB_CONTENTS_USER_DATA_KEY_DECL();
 private:
  FRIEND_TEST_ALL_PREFIXES(adrbrowsielWaybackMachineTest, InfobarAddTest);

  // content::WebContentsObserver overrides:
  void DidFinishNavigation(
      content::NavigationHandle* navigation_handle) override;

  void CreateInfoBar();
  bool IsWaybackMachineEnabled() const;

  // virtual for test.
  virtual bool ShouldAttachWaybackMachineInfoBar(int response_code) const;

  PrefService* pref_service_ = nullptr;
  std::unique_ptr<adrbrowsielWaybackMachineDelegate> delegate_;

  base::WeakPtrFactory<adrbrowsielWaybackMachineTabHelper> weak_factory_;
};

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_WAYBACK_MACHINE_adrbrowsiel_WAYBACK_MACHINE_TAB_HELPER_H_
