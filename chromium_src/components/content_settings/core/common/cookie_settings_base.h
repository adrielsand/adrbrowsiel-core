/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_COMPONENTS_CONTENT_SETTINGS_CORE_COMMON_COOKIE_SETTINGS_BASE_H_
#define adrbrowsiel_CHROMIUM_SRC_COMPONENTS_CONTENT_SETTINGS_CORE_COMMON_COOKIE_SETTINGS_BASE_H_

#define adrbrowsiel_COOKIE_SETTINGS_BASE_H                                      \
  bool ShouldUseEphemeralStorage(                                         \
      const GURL& url, const GURL& site_for_cookies,                      \
      const base::Optional<url::Origin>& top_frame_origin) const;         \
  bool IsEphemeralCookieAccessAllowed(const GURL& url,                    \
                                      const GURL& first_party_url) const; \
  bool IsEphemeralCookieAccessAllowed(                                    \
      const GURL& url, const GURL& site_for_cookies,                      \
      const base::Optional<url::Origin>& top_frame_origin) const;         \
  bool IsChromiumCookieAccessAllowed(const GURL& url,                     \
                                     const GURL& first_party_url) const;  \
  bool IsChromiumCookieAccessAllowed(                                     \
      const GURL& url, const GURL& site_for_cookies,                      \
      const base::Optional<url::Origin>& top_frame_origin) const;

#include "../../../../../../components/content_settings/core/common/cookie_settings_base.h"

#undef adrbrowsiel_COOKIE_SETTINGS_BASE_H

#endif  // adrbrowsiel_CHROMIUM_SRC_COMPONENTS_CONTENT_SETTINGS_CORE_COMMON_COOKIE_SETTINGS_BASE_H_
