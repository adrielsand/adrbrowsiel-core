/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMMON_NETWORK_CONSTANTS_H_
#define adrbrowsiel_COMMON_NETWORK_CONSTANTS_H_

extern const char kadrbrowsielProxyPattern[];
extern const char kadrbrowsielSoftwareProxyPattern[];

extern const char kadrbrowsielUsageStandardPath[];
extern const char kadrbrowsielUsageThresholdPath[];
extern const char kadrbrowsielReferralsServer[];
extern const char kadrbrowsielReferralsHeadersPath[];
extern const char kadrbrowsielReferralsInitPath[];
extern const char kadrbrowsielReferralsActivityPath[];
extern const char kadrbrowsielSafeBrowsing2Proxy[];
extern const char kadrbrowsielSafeBrowsingSslProxy[];
extern const char kadrbrowsielRedirectorProxy[];
extern const char kadrbrowsielClients4Proxy[];
extern const char kadrbrowsielStaticProxy[];

extern const char kAutofillPrefix[];
extern const char kClients4Prefix[];
extern const char kCRXDownloadPrefix[];
extern const char kEmptyDataURI[];
extern const char kEmptyImageDataURI[];
extern const char kJSDataURLPrefix[];
extern const char kGeoLocationsPattern[];
extern const char kSafeBrowsingPrefix[];
extern const char kSafeBrowsingCrxListPrefix[];
extern const char kSafeBrowsingFileCheckPrefix[];
extern const char kCRLSetPrefix1[];
extern const char kCRLSetPrefix2[];
extern const char kCRLSetPrefix3[];
extern const char kCRLSetPrefix4[];
extern const char kChromeCastPrefix[];
extern const char kWidevineGvt1Prefix[];
extern const char kWidevineGoogleDlPrefix[];

extern const char kUserAgentHeader[];
extern const char kadrbrowsielPartnerHeader[];
extern const char kadrbrowsielServicesKeyHeader[];

extern const char kBittorrentMimeType[];
extern const char kOctetStreamMimeType[];

extern const char kSecGpcHeader[];
#endif  // adrbrowsiel_COMMON_NETWORK_CONSTANTS_H_
