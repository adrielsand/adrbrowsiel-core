/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

namespace base {
class FilePath;
}  // namespace base

namespace {
base::FilePath GetLocalizableadrbrowsielAppShortcutsSubdirName();
}

#include "../../../../../../chrome/browser/web_applications/components/web_app_shortcut_mac.mm"  // NOLINT

namespace {
base::FilePath GetLocalizableadrbrowsielAppShortcutsSubdirName() {
  static const char kadrbrowsielBrowserDevelopmentAppDirName[] =
      "adrbrowsiel Browser Development Apps.localized";
  static const char kadrbrowsielBrowserAppDirName[] =
      "adrbrowsiel Browser Apps.localized";
  static const char kadrbrowsielBrowserBetaAppDirName[] =
      "adrbrowsiel Browser Beta Apps.localized";
  static const char kadrbrowsielBrowserDevAppDirName[] =
      "adrbrowsiel Browser Dev Apps.localized";
  static const char kadrbrowsielBrowserNightlyAppDirName[] =
      "adrbrowsiel Browser Nightly Apps.localized";

  switch (chrome::GetChannel()) {
    case version_info::Channel::STABLE:
      return base::FilePath(kadrbrowsielBrowserAppDirName);
    case version_info::Channel::BETA:
      return base::FilePath(kadrbrowsielBrowserBetaAppDirName);
    case version_info::Channel::DEV:
      return base::FilePath(kadrbrowsielBrowserDevAppDirName);
    case version_info::Channel::CANARY:
      return base::FilePath(kadrbrowsielBrowserNightlyAppDirName);
    case version_info::Channel::UNKNOWN:
      return base::FilePath(kadrbrowsielBrowserDevelopmentAppDirName);
    default:
      NOTREACHED();
      return base::FilePath(kadrbrowsielBrowserDevelopmentAppDirName);
  }
}
}  // namespace
