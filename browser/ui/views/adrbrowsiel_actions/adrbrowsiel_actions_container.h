/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_adrbrowsiel_ACTIONS_adrbrowsiel_ACTIONS_CONTAINER_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_adrbrowsiel_ACTIONS_adrbrowsiel_ACTIONS_CONTAINER_H_

#include <map>
#include <memory>
#include <string>

#include "adrbrowsiel/browser/extensions/api/adrbrowsiel_action_api.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/rewards_service.h"
#include "chrome/browser/extensions/api/extension_action/extension_action_api.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/toolbar/toolbar_action_view_controller.h"
#include "chrome/browser/ui/views/toolbar/toolbar_action_view.h"
#include "components/prefs/pref_member.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_registry_observer.h"
#include "extensions/common/extension.h"
#include "ui/views/view.h"

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
#include "adrbrowsiel/browser/ui/views/adrbrowsiel_actions/adrbrowsiel_rewards_action_stub_view.h"
#endif

class adrbrowsielActionViewController;
class adrbrowsielActionsContainerTest;
class RewardsBrowserTest;

namespace extensions {
class ExtensionActionManager;
}

namespace views {
class Button;
}

// This View contains all the built-in adrbrowsielActions such as Shields and Payments
// TODO(petemill): consider splitting to separate model, like
// ToolbarActionsModel and ToolbarActionsBar
class adrbrowsielActionsContainer : public views::View,
                              public extensions::adrbrowsielActionAPI::Observer,
                              public extensions::ExtensionActionAPI::Observer,
                              public extensions::ExtensionRegistryObserver,
                              public ToolbarActionView::Delegate,
#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
                              public adrbrowsielRewardsActionStubView::Delegate
#endif
                              {
 public:
  adrbrowsielActionsContainer(Browser* browser, Profile* profile);
  ~adrbrowsielActionsContainer() override;
  void Init();
  void Update();
  void SetShouldHide(bool should_hide);
  // ToolbarActionView::Delegate
  content::WebContents* GetCurrentWebContents() override;
  // Notifies that a drag completed.
  void OnToolbarActionViewDragDone() override;
  // Returns the view of the toolbar actions overflow menu to use as a
  // reference point for a popup when this view isn't visible.
  views::LabelButton* GetOverflowReferenceView() const override;
  // Returns the preferred size of the ToolbarActionView.
  gfx::Size GetToolbarActionSize() override;
  // Overridden from views::DragController (required by
  // ToolbarActionView::Delegate):
  void WriteDragDataForView(View* sender,
                            const gfx::Point& press_pt,
                            ui::OSExchangeData* data) override;
  int GetDragOperationsForView(View* sender, const gfx::Point& p) override;
  bool CanStartDragForView(View* sender,
                           const gfx::Point& press_pt,
                           const gfx::Point& p) override;

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  // adrbrowsielRewardsActionStubView::Delegate
  void OnRewardsStubButtonClicked() override;
#endif

  // ExtensionRegistryObserver:
  void OnExtensionLoaded(content::BrowserContext* browser_context,
                         const extensions::Extension* extension) override;
  void OnExtensionUnloaded(content::BrowserContext* browser_context,
                           const extensions::Extension* extension,
                           extensions::UnloadedExtensionReason reason) override;

  // ExtensionActionAPI::Observer
  // Called when there is a change to the given |extension_action|.
  // |web_contents| is the web contents that was affected, and
  // |browser_context| is the associated BrowserContext. (The latter is
  // included because ExtensionActionAPI is shared between normal and
  // incognito contexts, so |browser_context| may not equal
  // |browser_context_|.)
  void OnExtensionActionUpdated(
      extensions::ExtensionAction* extension_action,
      content::WebContents* web_contents,
      content::BrowserContext* browser_context) override;

  // adrbrowsiel Rewards preferences change observers callback.
  void OnadrbrowsielRewardsPreferencesChanged();

  // views::View:
  void ChildPreferredSizeChanged(views::View* child) override;

 private:
  friend class ::adrbrowsielActionsContainerTest;
  friend class ::RewardsBrowserTest;

  class EmptyExtensionsContainer;

  // Special positions in the container designators
  enum ActionPosition : int {
    ACTION_ANY_POSITION = -1,
  };

  // Action info container
  struct adrbrowsielActionInfo {
    adrbrowsielActionInfo();
    ~adrbrowsielActionInfo();
    // Reset view and view controller
    void Reset();

    int position_;
    std::unique_ptr<views::Button> view_;
    std::unique_ptr<adrbrowsielActionViewController> view_controller_;
  };

  // Actions that belong to the container
  std::map<std::string, adrbrowsielActionInfo> actions_;

  // Actions operations
  bool ShouldAddAction(const std::string& id) const;
  bool IsContainerAction(const std::string& id) const;
  void AddAction(const extensions::Extension* extension);
  void AddAction(const std::string& id);
  bool ShouldAddadrbrowsielRewardsAction() const;
  void AddActionStubForRewards();
  void RemoveAction(const std::string& id);
  void ShowAction(const std::string& id, bool show);
  bool IsActionShown(const std::string& id) const;
  void UpdateActionState(const std::string& id);
  void AttachAction(adrbrowsielActionInfo &action);

  // adrbrowsielActionAPI::Observer
  void OnadrbrowsielActionShouldTrigger(const std::string& extension_id,
      std::unique_ptr<std::string> ui_relative_path) override;

  bool should_hide_ = false;

  bool is_rewards_pressed_ = false;

  // The Browser this LocationBarView is in.  Note that at least
  // chromeos::SimpleWebViewDialog uses a LocationBarView outside any browser
  // window, so this may be NULL.
  Browser* const browser_;

  void OnExtensionSystemReady();

  extensions::ExtensionSystem* extension_system_;
  extensions::ExtensionActionAPI* extension_action_api_;
  extensions::ExtensionRegistry* extension_registry_;
  extensions::ExtensionActionManager* extension_action_manager_;
  extensions::adrbrowsielActionAPI* adrbrowsiel_action_api_;

  // Listen to extension load, unloaded notifications.
  ScopedObserver<extensions::ExtensionRegistry,
                 extensions::ExtensionRegistryObserver>
      extension_registry_observer_;

  // Listen to when the action is updated
  ScopedObserver<extensions::ExtensionActionAPI,
                 extensions::ExtensionActionAPI::Observer>
      extension_action_observer_;

  // Listen to when we need to open a popup
  ScopedObserver<extensions::adrbrowsielActionAPI,
                 extensions::adrbrowsielActionAPI::Observer>
      adrbrowsiel_action_observer_;

  // Listen for adrbrowsiel Rewards preferences changes.
  BooleanPrefMember adrbrowsiel_rewards_enabled_;
  BooleanPrefMember hide_adrbrowsiel_rewards_button_;

  std::unique_ptr<EmptyExtensionsContainer> empty_extensions_container_;

  adrbrowsiel_rewards::RewardsService* rewards_service_;

  base::WeakPtrFactory<adrbrowsielActionsContainer> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielActionsContainer);
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_adrbrowsiel_ACTIONS_adrbrowsiel_ACTIONS_CONTAINER_H_
