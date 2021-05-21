/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_TAB_HELPERS_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_TAB_HELPERS_H_

namespace content {
class WebContents;
}

namespace adrbrowsiel {

void AttachTabHelpers(content::WebContents* web_contents);

}  // namespace adrbrowsiel

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_TAB_HELPERS_H_

