/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_COMPONENTS_COMPONENT_UPDATER_COMPONENT_UPDATER_SERVICE_H_
#define adrbrowsiel_CHROMIUM_SRC_COMPONENTS_COMPONENT_UPDATER_COMPONENT_UPDATER_SERVICE_H_

class IPFSDOMHandler;

#define adrbrowsiel_COMPONENT_UPDATER_SERVICE_H_ friend class ::IPFSDOMHandler;
#define adrbrowsiel_COMPONENT_UPDATER_SERVICE_H_ON_DEMAND_UPDATER \
 private:                                                   \
  friend void adrbrowsielOnDemandUpdate(const std::string&);      \
                                                            \
 public:
#include "../../../../components/component_updater/component_updater_service.h"

#endif  // adrbrowsiel_CHROMIUM_SRC_COMPONENTS_COMPONENT_UPDATER_COMPONENT_UPDATER_SERVICE_H_
