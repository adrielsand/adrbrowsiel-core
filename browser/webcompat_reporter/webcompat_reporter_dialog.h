/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_WEBCOMPAT_REPORTER_WEBCOMPAT_REPORTER_DIALOG_H_
#define adrbrowsiel_BROWSER_WEBCOMPAT_REPORTER_WEBCOMPAT_REPORTER_DIALOG_H_

namespace content {
class WebContents;
}

void OpenWebcompatReporterDialog(content::WebContents* initiator);

#endif  // adrbrowsiel_BROWSER_WEBCOMPAT_REPORTER_WEBCOMPAT_REPORTER_DIALOG_H_
