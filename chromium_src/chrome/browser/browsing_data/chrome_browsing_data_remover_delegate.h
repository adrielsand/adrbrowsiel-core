/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_BROWSING_DATA_CHROME_BROWSING_DATA_REMOVER_DELEGATE_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_BROWSING_DATA_CHROME_BROWSING_DATA_REMOVER_DELEGATE_H_

class adrbrowsielBrowsingDataRemoverDelegate;

#define adrbrowsiel_CHROME_BROWSING_DATA_REMOVER_DELEGATE_H \
  friend class adrbrowsielBrowsingDataRemoverDelegate;

#include "../../../../../chrome/browser/browsing_data/chrome_browsing_data_remover_delegate.h"
#undef adrbrowsiel_CHROME_BROWSING_DATA_REMOVER_DELEGATE_H

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_BROWSING_DATA_CHROME_BROWSING_DATA_REMOVER_DELEGATE_H_
