/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_WEBUI_WEBCOMPAT_REPORTER_UI_H_
#define adrbrowsiel_BROWSER_UI_WEBUI_WEBCOMPAT_REPORTER_UI_H_

#include <string>

#include "base/macros.h"
#include "chrome/browser/ui/webui/constrained_web_dialog_ui.h"

namespace content {
class WebUI;
}

class WebcompatReporterUI : public ConstrainedWebDialogUI {
 public:
  WebcompatReporterUI(content::WebUI* web_ui, const std::string& host);
  ~WebcompatReporterUI() override;

 private:
  DISALLOW_COPY_AND_ASSIGN(WebcompatReporterUI);
};

#endif  // adrbrowsiel_BROWSER_UI_WEBUI_WEBCOMPAT_REPORTER_UI_H_
