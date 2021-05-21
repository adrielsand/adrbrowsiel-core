// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/common/channel_info.h"

#include "base/environment.h"
#include "base/strings/string_util.h"
#include "adrbrowsiel/common/adrbrowsiel_channel_info_posix.h"
#include "build/build_config.h"
#include "components/version_info/version_info.h"

namespace chrome {

std::string GetChannelName(WithExtendedStable with_extended_stable) {
  std::string modifier;
  adrbrowsiel::GetChannelImpl(&modifier, nullptr);
  return modifier;
}

std::string GetChannelSuffixForDataDir() {
  return std::string();
}

#if defined(OS_LINUX)
std::string GetDesktopName(base::Environment* env) {
#if defined(OFFICIAL_BUILD)
  version_info::Channel product_channel(chrome::GetChannel());
  switch (product_channel) {
    case version_info::Channel::DEV:
      return "adrbrowsiel-browser-dev.desktop";
    case version_info::Channel::BETA:
      return "adrbrowsiel-browser-beta.desktop";
    case version_info::Channel::CANARY:
      return "adrbrowsiel-browser-nightly.desktop";
    default:
      return "adrbrowsiel-browser.desktop";
  }
#endif  // defined(OFFICIAL_BUILD)
  // Allow $CHROME_DESKTOP to override the built-in value, so that development
  // versions can set themselves as the default without interfering with
  // non-official, packaged versions using the built-in value.
  std::string name;
  if (env->GetVar("CHROME_DESKTOP", &name) && !name.empty())
    return name;
  return "adrbrowsiel-browser.desktop";
}
#endif  // defined(OS_LINUX)

version_info::Channel GetChannel() {
  return adrbrowsiel::GetChannelImpl(nullptr, nullptr);
}

bool IsExtendedStableChannel() {
  // No extended stable channel for adrbrowsiel.
  return false;
}

}  // namespace chrome
