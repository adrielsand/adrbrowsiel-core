/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "third_party/blink/renderer/modules/webaudio/analyser_node.h"

#include "third_party/blink/public/platform/web_content_settings_client.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/workers/worker_global_scope.h"

#define adrbrowsiel_ANALYSERHANDLER_CONSTRUCTOR                                    \
  if (ExecutionContext* context = node.GetExecutionContext()) {              \
    if (WebContentSettingsClient* settings =                                 \
            adrbrowsiel::GetContentSettingsClientFor(context)) {                   \
      analyser_.audio_farbling_callback_ =                                   \
          adrbrowsiel::adrbrowsielSessionCache::From(*context).GetAudioFarblingCallback( \
              settings);                                                     \
    }                                                                        \
  }

#include "../../../../../../../third_party/blink/renderer/modules/webaudio/analyser_node.cc"

#undef adrbrowsiel_ANALYSERNODE_CONSTRUCTOR
