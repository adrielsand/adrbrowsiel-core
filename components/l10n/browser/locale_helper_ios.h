/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_L10N_BROWSER_LOCALE_HELPER_IOS_H_
#define adrbrowsiel_COMPONENTS_L10N_BROWSER_LOCALE_HELPER_IOS_H_

#include <string>

#include "adrbrowsiel/components/l10n/browser/locale_helper.h"

namespace adrbrowsiel_l10n {

class LocaleHelperIos : public LocaleHelper {
 public:
  LocaleHelperIos(const LocaleHelperIos&) = delete;
  LocaleHelperIos& operator=(const LocaleHelperIos&) = delete;

  static LocaleHelperIos* GetInstanceImpl();

 private:
  friend struct base::DefaultSingletonTraits<LocaleHelperIos>;

  LocaleHelperIos();
  ~LocaleHelperIos() override;

  // LocaleHelper impl
  std::string GetLocale() const override;
};

}  // namespace adrbrowsiel_l10n

#endif  // adrbrowsiel_COMPONENTS_L10N_BROWSER_LOCALE_HELPER_IOS_H_
