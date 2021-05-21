/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * you can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/views/tabs/adrbrowsiel_tab_search_button.h"

#include <algorithm>

#include "ui/gfx/geometry/size.h"

adrbrowsielTabSearchButton::~adrbrowsielTabSearchButton() = default;

int adrbrowsielTabSearchButton::GetCornerRadius() const {
  // Copied from LayoutProvider::GetCornerRadiusMetric() for kMaximum.
  // We override GetCornerRadiusMetric() by adrbrowsielLayoutProvider. However,
  // TabSearchButton needs original radius value.
  auto size = GetContentsBounds().size();
  return std::min(size.width(), size.height()) / 2;
}
