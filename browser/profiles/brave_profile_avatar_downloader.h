/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_PROFILES_adrbrowsiel_PROFILE_AVATAR_DOWNLOADER_H_
#define adrbrowsiel_BROWSER_PROFILES_adrbrowsiel_PROFILE_AVATAR_DOWNLOADER_H_

#include "chrome/browser/profiles/profile_avatar_downloader.h"

class adrbrowsielProfileAvatarDownloader : public ProfileAvatarDownloader {
 public:
  adrbrowsielProfileAvatarDownloader(size_t icon_index,
                               FetchCompleteCallback callback);
  ~adrbrowsielProfileAvatarDownloader() override;

  void Start();

 private:
  FetchCompleteCallback callback_;
};

#endif  // adrbrowsiel_BROWSER_PROFILES_adrbrowsiel_PROFILE_AVATAR_DOWNLOADER_H_
