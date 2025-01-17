/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/extensions/extension_view_host.h"
#include "extensions/common/constants.h"

namespace {
bool IsadrbrowsielExtension(extensions::ExtensionViewHost* host) {
  // Clipping issue is not present on linux.
#if defined(OS_MAC) || defined(OS_WIN)
  const auto extensionId = host->extension()->id();
  return (extensionId == adrbrowsiel_extension_id ||
          extensionId == adrbrowsiel_rewards_extension_id);
#endif
  return false;
}
}  // namespace

// HACK: removes border radius for adrbrowsiel extensions
//       as webview does not support radius clipping which results
//       in white strips at top and bottom of popup.
// TODO(mkarolin): add adrbrowsiel extension radius back in when macOS and Windows
//       popups support web dialog window radius clipping.
#define adrbrowsiel_ADDED_TO_WIDGET                      \
  const int radius = IsadrbrowsielExtension(host_.get()) \
                         ? 0                       \
                         : GetBubbleFrameView()->GetCornerRadius();

#include "../../../../../../../chrome/browser/ui/views/extensions/extension_popup.cc"
#undef adrbrowsiel_ADDED_TO_WIDGET
