/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_web_ui_controller_factory.h"

#define adrbrowsiel_CHROME_WEBUI_CONTROLLER_FACTORY \
  return adrbrowsielWebUIControllerFactory::GetInstance();

#include "../../../../../../chrome/browser/ui/webui/chrome_web_ui_controller_factory.cc"
#undef adrbrowsiel_CHROME_WEBUI_CONTROLLER_FACTORY
