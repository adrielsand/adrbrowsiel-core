// Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef adrbrowsiel_BROWSER_FTX_FTX_PROTOCOL_HANDLER_H_
#define adrbrowsiel_BROWSER_FTX_FTX_PROTOCOL_HANDLER_H_

#include <string>

#include "base/optional.h"
#include "content/public/browser/web_contents.h"
#include "url/gurl.h"
#include "url/origin.h"

namespace ftx {

void HandleFTXProtocol(const GURL& url,
                       content::WebContents::OnceGetter web_contents_getter,
                       ui::PageTransition page_transition,
                       bool has_user_gesture,
                       const base::Optional<url::Origin>& initiator);

bool IsFTXProtocol(const GURL& url);

}  // namespace ftx

#endif  // adrbrowsiel_BROWSER_FTX_FTX_PROTOCOL_HANDLER_H_
