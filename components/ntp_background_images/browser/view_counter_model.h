// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef adrbrowsiel_COMPONENTS_NTP_BACKGROUND_IMAGES_BROWSER_VIEW_COUNTER_MODEL_H_
#define adrbrowsiel_COMPONENTS_NTP_BACKGROUND_IMAGES_BROWSER_VIEW_COUNTER_MODEL_H_

#include "base/gtest_prod_util.h"

namespace ntp_background_images {

class ViewCounterModel {
 public:
  ViewCounterModel();
  ~ViewCounterModel();

  ViewCounterModel(const ViewCounterModel&) = delete;
  ViewCounterModel& operator=(const ViewCounterModel&) = delete;

  int current_wallpaper_image_index() const {
    return current_wallpaper_image_index_;
  }

  void set_total_image_count(int count) { total_image_count_ = count; }
  void set_ignore_count_to_branded_wallpaper(bool ignore) {
    ignore_count_to_branded_wallpaper_ = ignore;
  }

  bool ShouldShowBrandedWallpaper() const;
  void RegisterPageView();
  void ResetCurrentWallpaperImageIndex();

  // If |true|, kInitialCountToBrandedWallpaper is set to
  // |count_to_branded_wallpaper_|.
  void Reset(bool use_initial_count = true);

 private:
  static const int kInitialCountToBrandedWallpaper = 1;
  static const int kRegularCountToBrandedWallpaper = 3;

  FRIEND_TEST_ALL_PREFIXES(ViewCounterModelTest, NTPSponsoredImagesTest);
  FRIEND_TEST_ALL_PREFIXES(NTPBackgroundImagesViewCounterTest, ModelTest);

  int current_wallpaper_image_index_ = 0;
  int count_to_branded_wallpaper_ = 0;
  int total_image_count_ = 0;
  bool ignore_count_to_branded_wallpaper_ = false;
};

}  // namespace ntp_background_images

#endif  // adrbrowsiel_COMPONENTS_NTP_BACKGROUND_IMAGES_BROWSER_VIEW_COUNTER_MODEL_H_
