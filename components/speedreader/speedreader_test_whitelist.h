/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_SPEEDREADER_SPEEDREADER_TEST_WHITELIST_H_
#define adrbrowsiel_COMPONENTS_SPEEDREADER_SPEEDREADER_TEST_WHITELIST_H_

class GURL;

namespace speedreader {

bool IsWhitelistedForTest(const GURL& url);

}  // namespace speedreader

#endif  // adrbrowsiel_COMPONENTS_SPEEDREADER_SPEEDREADER_TEST_WHITELIST_H_
