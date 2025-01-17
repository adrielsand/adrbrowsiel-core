/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <algorithm>

#include "base/time/time.h"

namespace {

// Javascript max expiration is handled by blink::CookieStore
constexpr base::TimeDelta kMaxCookieExpiration =
    base::TimeDelta::FromDays(30*6);  // 6 months

base::Time adrbrowsielCanonExpiration(const base::Time& cookie_expires,
                                const base::Time& creation_time) {
  const base::Time max_expiration = creation_time + kMaxCookieExpiration;

  return std::min(cookie_expires, max_expiration);
}

}  // namespace

#define adrbrowsiel_CREATE \
  cookie_expires = adrbrowsielCanonExpiration(cookie_expires, creation_time);

#include "../../../../net/cookies/canonical_cookie.cc"
#undef adrbrowsiel_CREATE
