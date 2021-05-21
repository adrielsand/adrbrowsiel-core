/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_REWARDS_TIP_DIALOG_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_REWARDS_TIP_DIALOG_H_

#include <memory>
#include <string>

namespace base {
class DictionaryValue;
}

namespace content {
class WebContents;
}

namespace adrbrowsiel_rewards {

void OpenTipDialog(content::WebContents* initiator,
                   std::unique_ptr<base::DictionaryValue> params);

}

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_REWARDS_TIP_DIALOG_H_
