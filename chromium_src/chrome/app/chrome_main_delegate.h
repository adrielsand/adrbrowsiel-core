/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_APP_CHROME_MAIN_DELEGATE_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_APP_CHROME_MAIN_DELEGATE_H_

#include "adrbrowsiel/common/adrbrowsiel_content_client.h"
#include "chrome/common/chrome_content_client.h"

#define ChromeContentClient adrbrowsielContentClient
#include "../../../../chrome/app/chrome_main_delegate.h"
#undef ChromeContentClient

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_APP_CHROME_MAIN_DELEGATE_H_
