/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/bundle/creative_brave_news_ad_info.h"

namespace ads {

CreativeBraveNewsAdInfo::CreativeBraveNewsAdInfo() = default;

CreativeBraveNewsAdInfo::CreativeBraveNewsAdInfo(
    const CreativeBraveNewsAdInfo& info) = default;

CreativeBraveNewsAdInfo::~CreativeBraveNewsAdInfo() = default;

bool CreativeBraveNewsAdInfo::operator==(
    const CreativeBraveNewsAdInfo& rhs) const {
  return title == rhs.title && description == rhs.description &&
         image_url == rhs.image_url && size == rhs.size;
}

bool CreativeBraveNewsAdInfo::operator!=(
    const CreativeBraveNewsAdInfo& rhs) const {
  return !(*this == rhs);
}

}  // namespace ads
