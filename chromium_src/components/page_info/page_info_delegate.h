/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_COMPONENTS_PAGE_INFO_PAGE_INFO_DELEGATE_H_
#define adrbrowsiel_CHROMIUM_SRC_COMPONENTS_PAGE_INFO_PAGE_INFO_DELEGATE_H_

#define IsContentDisplayedInVrHeadset                      \
  adrbrowsielShouldShowPermission(ContentSettingsType type) = 0; \
  virtual bool IsContentDisplayedInVrHeadset

#include "../../../../components/page_info/page_info_delegate.h"
#undef IsContentDisplayedInVrHeadset

#endif  // adrbrowsiel_CHROMIUM_SRC_COMPONENTS_PAGE_INFO_PAGE_INFO_DELEGATE_H_
