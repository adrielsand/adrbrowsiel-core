/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/tor/mock_tor_launcher_factory.h"

// static
MockTorLauncherFactory& MockTorLauncherFactory::GetInstance() {
  static base::NoDestructor<MockTorLauncherFactory> instance;
  return *instance;
}

MockTorLauncherFactory::MockTorLauncherFactory() = default;
MockTorLauncherFactory::~MockTorLauncherFactory() = default;
