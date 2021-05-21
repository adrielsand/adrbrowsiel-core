// Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef adrbrowsiel_BROWSER_UI_adrbrowsiel_ACTIONS_adrbrowsiel_ACTION_VIEW_CONTROLLER_H_
#define adrbrowsiel_BROWSER_UI_adrbrowsiel_ACTIONS_adrbrowsiel_ACTION_VIEW_CONTROLLER_H_

#include <memory>
#include <string>

#include "chrome/browser/ui/extensions/extension_action_view_controller.h"

class adrbrowsielActionIconWithBadgeImageSource;
class ToolbarActionViewController;

namespace ui {
class MenuModel;
}

// The purposes of this subclass are to:
// - Overcome the DCHECK in HideActivePopup since a toolbar will not be provided
// - Use our custom class for painting the badge differently compared to
//   user-installed extensions
// - Remove the context menu from the button since we do not allow uninstall
class adrbrowsielActionViewController : public ExtensionActionViewController {
 public:
    using ExtensionActionViewController::ExtensionActionViewController;
    bool IsEnabled(content::WebContents* web_contents) const override;
    gfx::Image GetIcon(content::WebContents* web_contents,
                       const gfx::Size& size) override;
    bool DisabledClickOpensMenu() const override;
    ui::MenuModel* GetContextMenu() override;
    bool ExecuteActionUI(std::string relative_path);
    ToolbarActionViewController* GetExtensionViewController(
        const std::string& extension_id);
 private:
    ExtensionActionViewController* GetPreferredPopupViewController() override;
    bool TriggerPopupWithUrl(PopupShowAction show_action,
                           const GURL& popup_url,
                           bool grant_tab_permissions) override;
    void OnPopupClosed() override;
    // Returns the image source for the icon.
    std::unique_ptr<adrbrowsielActionIconWithBadgeImageSource> GetIconImageSource(
        content::WebContents* web_contents,
        const gfx::Size& size);
    DISALLOW_COPY_AND_ASSIGN(adrbrowsielActionViewController);
};

#endif  // adrbrowsiel_BROWSER_UI_adrbrowsiel_ACTIONS_adrbrowsiel_ACTION_VIEW_CONTROLLER_H_
