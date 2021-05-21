/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_NOTIFICATION_HELPER_WIN_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_NOTIFICATION_HELPER_WIN_H_

#include <windows.ui.notifications.h>
#include <wrl/event.h>

#include "base/memory/weak_ptr.h"

#include "adrbrowsiel/components/adrbrowsiel_ads/browser/notification_helper.h"

namespace adrbrowsiel_ads {

class NotificationHelperWin
    : public NotificationHelper,
      public base::SupportsWeakPtr<NotificationHelperWin> {
 public:
  NotificationHelperWin(const NotificationHelperWin&) = delete;
  NotificationHelperWin& operator=(const NotificationHelperWin&) = delete;

  static NotificationHelperWin* GetInstanceImpl();

 private:
  friend struct base::DefaultSingletonTraits<NotificationHelperWin>;

  NotificationHelperWin();
  ~NotificationHelperWin() override;

  bool IsFocusAssistEnabled() const;

  bool IsNotificationsEnabled();

  std::wstring GetAppId() const;

  HRESULT InitializeToastNotifier();

  template <unsigned int size, typename T>
  HRESULT CreateActivationFactory(wchar_t const (&class_name)[size],
                                  T** object) const;

  Microsoft::WRL::ComPtr<ABI::Windows::UI::Notifications::IToastNotifier>
      notifier_;

  // NotificationHelper impl
  bool ShouldShowNotifications() override;

  bool ShowMyFirstAdNotification() override;

  bool CanShowBackgroundNotifications() const override;
};

}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_NOTIFICATION_HELPER_WIN_H_
