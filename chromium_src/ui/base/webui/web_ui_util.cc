// Copyright (c) 2019 The adrbrowsiel Authors
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "ui/base/webui/web_ui_util.h"

#include "adrbrowsiel/ui/webui/resources/grit/adrbrowsiel_webui_resources.h"
#include "ui/resources/grit/webui_generated_resources.h"

// Replace text_defaults_md.css with adrbrowsiel's text_defaults_md.css
// which is defined in adrbrowsiel_webui_resources.grd.
#undef IDR_WEBUI_CSS_TEXT_DEFAULTS_MD_CSS
#define IDR_WEBUI_CSS_TEXT_DEFAULTS_MD_CSS IDR_adrbrowsiel_WEBUI_CSS_TEXT_DEFAULTS_MD

#include "../../../../../ui/base/webui/web_ui_util.cc"
#undef IDR_WEBUI_CSS_TEXT_DEFAULTS_MD_CSS
