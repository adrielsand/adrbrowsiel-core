/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_HTML_CANVAS_HTML_CANVAS_ELEMENT_H_
#define adrbrowsiel_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_HTML_CANVAS_HTML_CANVAS_ELEMENT_H_

#define adrbrowsiel_HTML_CANVAS_ELEMENT_H_                                           \
  String toDataURL(ScriptState* script_state, const String& mime_type,         \
                   const ScriptValue& quality_argument,                        \
                   ExceptionState& exception_state) const;                     \
  String toDataURL(ScriptState* script_state, const String& mime_type,         \
                   ExceptionState& exception_state) const {                    \
    return toDataURL(script_state, mime_type, ScriptValue(), exception_state); \
  }                                                                            \
                                                                               \
 private:                                                                      \
  mutable UntracedMember<ExecutionContext> scoped_execution_context_;

#include "../../../../../../../../third_party/blink/renderer/core/html/canvas/html_canvas_element.h"

#undef adrbrowsiel_HTML_CANVAS_ELEMENT_H_

#endif  // adrbrowsiel_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_HTML_CANVAS_HTML_CANVAS_ELEMENT_H_
