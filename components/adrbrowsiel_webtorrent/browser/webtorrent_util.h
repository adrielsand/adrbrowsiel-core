/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_WEBTORRENT_BROWSER_WEBTORRENT_UTIL_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_WEBTORRENT_BROWSER_WEBTORRENT_UTIL_H_

class GURL;

namespace content {
class BrowserContext;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace net {
class HttpResponseHeaders;
}

namespace webtorrent {

bool IsWebtorrentEnabled(content::BrowserContext* browser_context);
bool IsWebtorrentPrefEnabled(content::BrowserContext* browser_context);
void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);
bool IsWebtorrentURL(const GURL& url);
bool IsTorrentFile(const GURL& url, const net::HttpResponseHeaders* headers);
bool TorrentURLMatched(const GURL& url);

}  // webtorrent

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_WEBTORRENT_BROWSER_WEBTORRENT_UTIL_H_
