/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_L10N_BROWSER_LOCALE_HELPER_MOCK_H_
#define adrbrowsiel_COMPONENTS_L10N_BROWSER_LOCALE_HELPER_MOCK_H_

#include <string>

#include "adrbrowsiel/components/l10n/browser/locale_helper.h"

#include "testing/gmock/include/gmock/gmock.h"

namespace adrbrowsiel_l10n {

class LocaleHelperMock : public LocaleHelper {
 public:
  LocaleHelperMock();
  ~LocaleHelperMock() override;

  LocaleHelperMock(const LocaleHelperMock&) = delete;
  LocaleHelperMock& operator=(const LocaleHelperMock&) = delete;

  MOCK_CONST_METHOD0(GetLocale, std::string());
};

}  // namespace adrbrowsiel_l10n

#endif  // adrbrowsiel_COMPONENTS_L10N_BROWSER_LOCALE_HELPER_MOCK_H_
