/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_BROWSER_NET_NETWORK_HELPER_DELEGATE_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_BROWSER_NET_NETWORK_HELPER_DELEGATE_H_

#include "adrbrowsiel/browser/net/url_context.h"

namespace adrbrowsiel_rewards {

int OnBeforeURLRequest(
    const adrbrowsiel::ResponseCallback& next_callback,
    std::shared_ptr<adrbrowsiel::adrbrowsielRequestInfo> ctx);

}  // namespace adrbrowsiel_rewards

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_BROWSER_NET_NETWORK_HELPER_DELEGATE_H_
