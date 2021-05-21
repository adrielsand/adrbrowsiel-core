/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser;

import org.chromium.chrome.browser.feedback.HelpAndFeedbackLauncher;
import org.chromium.chrome.browser.help.adrbrowsielHelpAndFeedbackLauncher;
import org.chromium.chrome.browser.partnerbookmarks.PartnerBookmark;
import org.chromium.chrome.browser.partnercustomizations.adrbrowsielPartnerBrowserCustomizations;
import org.chromium.chrome.browser.partnercustomizations.PartnerBrowserCustomizations;

public class adrbrowsielAppHooks extends AppHooksImpl {
    @Override
    public HelpAndFeedbackLauncher createHelpAndFeedbackLauncher() {
        return new adrbrowsielHelpAndFeedbackLauncher();
    }

    @Override
    public PartnerBookmark.BookmarkIterator getPartnerBookmarkIterator() {
        return null;
    }

    @Override
    public PartnerBrowserCustomizations.Provider getCustomizationProvider() {
        return new adrbrowsielPartnerBrowserCustomizations.ProviderPackage();
    }
}
