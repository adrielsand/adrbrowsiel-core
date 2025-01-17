/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_CONTENT_SETTINGS_RENDERER_adrbrowsiel_CONTENT_SETTINGS_AGENT_IMPL_HELPER_H_
#define adrbrowsiel_COMPONENTS_CONTENT_SETTINGS_RENDERER_adrbrowsiel_CONTENT_SETTINGS_AGENT_IMPL_HELPER_H_

#include "third_party/blink/public/platform/web_content_settings_client.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"

static bool AllowFingerprinting(blink::LocalFrame* frame) {
  if (!frame || !frame->GetContentSettingsClient()) {
    return true;
  }
  return frame->GetContentSettingsClient()->AllowFingerprinting(true);
}

#endif  // adrbrowsiel_COMPONENTS_CONTENT_SETTINGS_RENDERER_adrbrowsiel_CONTENT_SETTINGS_AGENT_IMPL_HELPER_H_
