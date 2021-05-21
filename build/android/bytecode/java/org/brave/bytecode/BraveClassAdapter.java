/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielClassAdapter {
    public static ClassVisitor createAdapter(ClassVisitor chain) {
        chain = new adrbrowsielActivityClassAdapter(chain);
        chain = new adrbrowsielAndroidSyncSettingsClassAdapter(chain);
        chain = new adrbrowsielAppMenuClassAdapter(chain);
        chain = new adrbrowsielBookmarkUtilsClassAdapter(chain);
        chain = new adrbrowsielBottomControlsCoordinatorClassAdapter(chain);
        chain = new adrbrowsielBottomControlsMediatorClassAdapter(chain);
        chain = new adrbrowsielCommandLineInitUtilClassAdapter(chain);
        chain = new adrbrowsielEditUrlSuggestionProcessorClassAdapter(chain);
        chain = new adrbrowsielFeedSurfaceCoordinatorClassAdapter(chain);
        chain = new adrbrowsielHomepageManagerClassAdapter(chain);
        chain = new adrbrowsielIncognitoToggleTabLayoutClassAdapter(chain);
        chain = new adrbrowsielLaunchIntentDispatcherClassAdapter(chain);
        chain = new adrbrowsielMainPreferenceBaseClassAdapter(chain);
        chain = new adrbrowsielManageSyncSettingsClassAdapter(chain);
        chain = new adrbrowsielMimeUtilsClassAdapter(chain);
        chain = new adrbrowsielNewTabPageClassAdapter(chain);
        chain = new adrbrowsielNewTabPageLayoutClassAdapter(chain);
        chain = new adrbrowsielPasswordSettingsBaseClassAdapter(chain);
        chain = new adrbrowsielPermissionDialogDelegateClassAdapter(chain);
        chain = new adrbrowsielPermissionDialogModelClassAdapter(chain);
        chain = new adrbrowsielQueryTileSectionClassAdapter(chain);
        chain = new adrbrowsielSearchEngineAdapterClassAdapter(chain);
        chain = new adrbrowsielSettingsLauncherImplClassAdapter(chain);
        chain = new adrbrowsielSingleCategorySettingsClassAdapter(chain);
        chain = new adrbrowsielSiteSettingsDelegateClassAdapter(chain);
        chain = new adrbrowsielTabbedActivityClassAdapter(chain);
        chain = new adrbrowsielTabGroupUiCoordinatorClassAdapter(chain);
        chain = new adrbrowsielTabSwitcherModeTTCoordinatorPhoneClassAdapter(chain);
        chain = new adrbrowsielTabSwitcherModeTTPhoneClassAdapter(chain);
        chain = new adrbrowsielTileViewClassAdapter(chain);
        chain = new adrbrowsielToolbarLayoutClassAdapter(chain);
        chain = new adrbrowsielToolbarManagerClassAdapter(chain);
        chain = new adrbrowsielTopToolbarCoordinatorClassAdapter(chain);
        return chain;
    }
}
