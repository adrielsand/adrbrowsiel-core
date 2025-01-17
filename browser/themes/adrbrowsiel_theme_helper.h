/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_THEMES_adrbrowsiel_THEME_HELPER_H_
#define adrbrowsiel_BROWSER_THEMES_adrbrowsiel_THEME_HELPER_H_

#include "base/optional.h"
#include "chrome/browser/themes/theme_helper.h"

class Profile;

class adrbrowsielThemeHelper : public ThemeHelper {
 public:
  adrbrowsielThemeHelper() = default;
  ~adrbrowsielThemeHelper() override;

  adrbrowsielThemeHelper(const adrbrowsielThemeHelper&) = delete;
  adrbrowsielThemeHelper& operator=(const adrbrowsielThemeHelper&) = delete;

  void set_is_tor() { is_tor_ = true; }
  void set_is_guest() { is_guest_ = true; }

 protected:
  // ThemeHelper overrides:
  SkColor GetDefaultColor(
      int id,
      bool incognito,
      const CustomThemeSupplier* theme_supplier) const override;

  base::Optional<SkColor> GetOmniboxColor(
      int id,
      bool incognito,
      const CustomThemeSupplier* theme_supplier,
      bool* has_custom_color) const override;

 private:
  bool is_tor_ = false;
  bool is_guest_ = false;
};

#endif  // adrbrowsiel_BROWSER_THEMES_adrbrowsiel_THEME_HELPER_H_
