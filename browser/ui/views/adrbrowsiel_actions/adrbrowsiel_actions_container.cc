/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/views/adrbrowsiel_actions/adrbrowsiel_actions_container.h"

#include <memory>
#include <string>
#include <utility>

#include "base/command_line.h"
#include "base/one_shot_event.h"
#include "adrbrowsiel/browser/adrbrowsiel_rewards/rewards_service_factory.h"
#include "adrbrowsiel/browser/extensions/adrbrowsiel_component_loader.h"
#include "adrbrowsiel/browser/profiles/profile_util.h"
#include "adrbrowsiel/browser/ui/adrbrowsiel_actions/adrbrowsiel_action_view_controller.h"
#include "adrbrowsiel/browser/ui/views/adrbrowsiel_actions/adrbrowsiel_action_view.h"
#include "adrbrowsiel/browser/ui/views/adrbrowsiel_actions/adrbrowsiel_rewards_action_stub_view.h"
#include "adrbrowsiel/browser/ui/views/rounded_separator.h"
#include "adrbrowsiel/common/adrbrowsiel_switches.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/common/pref_names.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/extensions/extensions_container.h"
#include "chrome/browser/ui/layout_constants.h"
#include "chrome/browser/ui/toolbar/toolbar_action_view_controller.h"
#include "chrome/browser/ui/toolbar/toolbar_actions_bar_bubble_delegate.h"
#include "chrome/browser/ui/views/toolbar/toolbar_action_view.h"
#include "components/prefs/pref_service.h"
#include "extensions/browser/extension_action_manager.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_registry_observer.h"
#include "extensions/browser/extension_system.h"
#include "extensions/common/constants.h"
#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/views/layout/box_layout.h"
#include "ui/views/layout/grid_layout.h"
#include "ui/views/view.h"

class adrbrowsielActionsContainer::EmptyExtensionsContainer
    : public ExtensionsContainer {
 public:
  EmptyExtensionsContainer() = default;
  virtual ~EmptyExtensionsContainer() = default;

  ToolbarActionViewController* GetActionForId(
      const std::string& action_id) override { return nullptr; }

  ToolbarActionViewController* GetPoppedOutAction() const override {
    return nullptr;
  }

  bool IsActionVisibleOnToolbar(
    const ToolbarActionViewController* action) const override { return false; }

  extensions::ExtensionContextMenuModel::ButtonVisibility GetActionVisibility(
      const ToolbarActionViewController* action) const override {
    return extensions::ExtensionContextMenuModel::PINNED;
  }

  void UndoPopOut() override {}

  void SetPopupOwner(ToolbarActionViewController* popup_owner) override {}

  void HideActivePopup() override {}

  bool CloseOverflowMenuIfOpen() override { return false; }

  void PopOutAction(ToolbarActionViewController* action,
                    bool is_sticky,
                    base::OnceClosure closure) override {}

  bool ShowToolbarActionPopupForAPICall(const std::string& action_id) override {
    return false;
  }

  void ShowToolbarActionBubble(
      std::unique_ptr<ToolbarActionsBarBubbleDelegate> bubble) override {}

  void ShowToolbarActionBubbleAsync(
      std::unique_ptr<ToolbarActionsBarBubbleDelegate> bubble) override {}

  void ToggleExtensionsMenu() override {}

  bool HasAnyExtensions() const override { return false; }

 private:
  DISALLOW_COPY_AND_ASSIGN(EmptyExtensionsContainer);
};

adrbrowsielActionsContainer::adrbrowsielActionInfo::adrbrowsielActionInfo()
    : position_(ACTION_ANY_POSITION) {}

adrbrowsielActionsContainer::adrbrowsielActionInfo::~adrbrowsielActionInfo() {
  Reset();
}

void adrbrowsielActionsContainer::adrbrowsielActionInfo::Reset() {
  // Destroy view before view_controller.
  // Destructor for |ToolbarActionView| tries to access controller instance.
  view_.reset();
  view_controller_.reset();
}

adrbrowsielActionsContainer::adrbrowsielActionsContainer(Browser* browser, Profile* profile)
    : views::View(),
      extensions::adrbrowsielActionAPI::Observer(),
      browser_(browser),
      extension_system_(extensions::ExtensionSystem::Get(profile)),
      extension_action_api_(extensions::ExtensionActionAPI::Get(profile)),
      extension_registry_(extensions::ExtensionRegistry::Get(profile)),
      extension_action_manager_(
          extensions::ExtensionActionManager::Get(profile)),
      adrbrowsiel_action_api_(extensions::adrbrowsielActionAPI::Get(browser)),
      extension_registry_observer_(this),
      extension_action_observer_(this),
      adrbrowsiel_action_observer_(this),
      empty_extensions_container_(new EmptyExtensionsContainer),
      rewards_service_(
          adrbrowsiel_rewards::RewardsServiceFactory::GetForProfile(profile)),
      weak_ptr_factory_(this) {
  // Handle when the extension system is ready
  extension_system_->ready().Post(
      FROM_HERE, base::Bind(&adrbrowsielActionsContainer::OnExtensionSystemReady,
                            weak_ptr_factory_.GetWeakPtr()));
}

adrbrowsielActionsContainer::~adrbrowsielActionsContainer() {
  actions_.clear();
}

void adrbrowsielActionsContainer::Init() {
  // automatic layout
  auto vertical_container_layout = std::make_unique<views::BoxLayout>(
      views::BoxLayout::Orientation::kHorizontal);
  vertical_container_layout->set_main_axis_alignment(
      views::BoxLayout::MainAxisAlignment::kCenter);
  vertical_container_layout->set_cross_axis_alignment(
      views::BoxLayout::CrossAxisAlignment::kCenter);
  SetLayoutManager(std::move(vertical_container_layout));

  // children
  RoundedSeparator* adrbrowsiel_button_separator_ = new RoundedSeparator();
  // TODO(petemill): theme color
  adrbrowsiel_button_separator_->SetColor(SkColorSetRGB(0xb2, 0xb5, 0xb7));
  constexpr int kSeparatorMargin = 3;
  constexpr int kSeparatorWidth = 1;
  adrbrowsiel_button_separator_->SetPreferredSize(gfx::Size(
                                    kSeparatorWidth + kSeparatorMargin*2,
                                    GetLayoutConstant(LOCATION_BAR_ICON_SIZE)));
  // separator left & right margin
  adrbrowsiel_button_separator_->SetBorder(
      views::CreateEmptyBorder(0, kSeparatorMargin, 0, kSeparatorMargin));
  // Just in case the extensions load before this function does (not likely!)
  // make sure separator is at index 0
  AddChildViewAt(adrbrowsiel_button_separator_, 0);
  // Populate actions
  actions_[adrbrowsiel_extension_id].position_ = 1;
  actions_[adrbrowsiel_rewards_extension_id].position_ = ACTION_ANY_POSITION;

  // React to adrbrowsiel Rewards preferences changes.
  hide_adrbrowsiel_rewards_button_.Init(
      adrbrowsiel_rewards::prefs::kHideButton,
      browser_->profile()->GetPrefs(),
      base::Bind(&adrbrowsielActionsContainer::OnadrbrowsielRewardsPreferencesChanged,
                 base::Unretained(this)));
}

bool adrbrowsielActionsContainer::IsContainerAction(const std::string& id) const {
  return (actions_.find(id) != actions_.end());
}

bool adrbrowsielActionsContainer::ShouldAddAction(const std::string& id) const {
  if (!IsContainerAction(id))
    return false;
  if (id == adrbrowsiel_rewards_extension_id)
    return ShouldAddadrbrowsielRewardsAction();
  return true;
}

bool adrbrowsielActionsContainer::ShouldAddadrbrowsielRewardsAction() const {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kDisableadrbrowsielRewardsExtension)) {
    return false;
  }

  if (!adrbrowsiel::IsRegularProfile(browser_->profile())) {
    return false;
  }

  const PrefService* prefs = browser_->profile()->GetPrefs();
  return !prefs->GetBoolean(adrbrowsiel_rewards::prefs::kHideButton);
}

void adrbrowsielActionsContainer::AddAction(const extensions::Extension* extension) {
  DCHECK(extension);
  if (!ShouldAddAction(extension->id()))
    return;
  VLOG(1) << "AddAction (" << extension->id() << "), was already loaded: "
          << static_cast<bool>(actions_[extension->id()].view_);
  if (!actions_[extension->id()].view_controller_) {
    const auto& id = extension->id();
    // Remove existing stub view, if present
    actions_[id].Reset();
    // Create a ExtensionActionViewController for the extension
    // Passing stub ExtensionsContainer instead of ToolbarActionsBar since we
    // do not require that logic.
    // If we do require notifications when popups are open or closed,
    // then we should inherit and pass |this| through.
    actions_[id].view_controller_ = std::make_unique<adrbrowsielActionViewController>(
        extension, browser_,
        extension_action_manager_->GetExtensionAction(*extension),
        empty_extensions_container_.get(),
        /*in_overflow_mode*/false);
    // The button view
    actions_[id].view_ = std::make_unique<adrbrowsielActionView>(
        actions_[id].view_controller_.get(), this);
    AttachAction(actions_[id]);
    // Handle if we are in a continuing pressed state for this extension.
    if (is_rewards_pressed_ && id == adrbrowsiel_rewards_extension_id) {
      is_rewards_pressed_ = false;
      actions_[id].view_controller_->ExecuteAction(
          true, ToolbarActionViewController::InvocationSource::kToolbarButton);
    }
  }
}

void adrbrowsielActionsContainer::AddActionStubForRewards() {
  const std::string id = adrbrowsiel_rewards_extension_id;
  if (!ShouldAddAction(id)) {
    return;
  }
  if (actions_[id].view_) {
    return;
  }
#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  actions_[id].view_ = std::make_unique<adrbrowsielRewardsActionStubView>(
      browser_->profile(), this);
  AttachAction(actions_[id]);
#endif
}

void adrbrowsielActionsContainer::AttachAction(adrbrowsielActionInfo &action) {
  // Add extension view after separator view
  // `AddChildView` should be called first, so that changes that modify
  // layout (e.g. preferred size) are forwarded to its parent
  if (action.position_ != ACTION_ANY_POSITION) {
    DCHECK_GT(action.position_, 0);
    AddChildViewAt(action.view_.get(), action.position_);
  } else {
    AddChildView(action.view_.get());
  }
  // we control destruction
  action.view_->set_owned_by_client();
  Update();
  PreferredSizeChanged();
}

void adrbrowsielActionsContainer::AddAction(const std::string& id) {
  DCHECK(extension_registry_);
  const extensions::Extension* extension =
      extension_registry_->enabled_extensions().GetByID(id);
  if (extension) {
    AddAction(extension);
    return;
  }
#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  if (id == adrbrowsiel_rewards_extension_id) {
    AddActionStubForRewards();
    return;
  }
#endif
  LOG(ERROR) << "Extension not found for adrbrowsiel Action: " << id;
}

void adrbrowsielActionsContainer::RemoveAction(const std::string& id) {
  DCHECK(IsContainerAction(id));
  VLOG(1) << "RemoveAction (" << id << "), was loaded: "
          << static_cast<bool>(actions_[id].view_);
  // This will reset references and automatically remove the child from the
  // parent (us)
  actions_[id].Reset();
  // layout
  Update();
  PreferredSizeChanged();
}

// Adds or removes action
void adrbrowsielActionsContainer::ShowAction(const std::string& id, bool show) {
  if (show != IsActionShown(id))
    show ? AddAction(id) : RemoveAction(id);
}

// Checks if action for the given |id| has been added
bool adrbrowsielActionsContainer::IsActionShown(const std::string& id) const {
  DCHECK(IsContainerAction(id));
  return(actions_.at(id).view_ != nullptr);
}

void adrbrowsielActionsContainer::UpdateActionState(const std::string& id) {
  if (actions_[id].view_controller_)
    actions_[id].view_controller_->UpdateState();
}

void adrbrowsielActionsContainer::Update() {
  // Update state of each action and also determine if there are any buttons to
  // show
  bool can_show = false;
  for (auto const& pair : actions_) {
    if (pair.second.view_controller_)
      pair.second.view_controller_->UpdateState();
    if (!can_show && pair.second.view_)
      can_show = true;
  }
  // only show separator if we're showing any buttons
  const bool visible = !should_hide_ && can_show;
  SetVisible(visible);
  Layout();
}

void adrbrowsielActionsContainer::SetShouldHide(bool should_hide) {
  should_hide_ = should_hide;
  Update();
}

content::WebContents* adrbrowsielActionsContainer::GetCurrentWebContents() {
  return browser_->tab_strip_model()->GetActiveWebContents();
}

void adrbrowsielActionsContainer::OnToolbarActionViewDragDone() {
}

views::LabelButton* adrbrowsielActionsContainer::GetOverflowReferenceView() const {
  // Our action views should always be visible,
  // so we should not need another view.
  NOTREACHED();
  return nullptr;
}

// ToolbarActionView::Delegate members
gfx::Size adrbrowsielActionsContainer::GetToolbarActionSize() {
  // Width > Height to give space for a large bubble (especially for shields).
  // TODO(petemill): Generate based on toolbar size.
  return gfx::Size(34, 24);
}

void adrbrowsielActionsContainer::WriteDragDataForView(View* sender,
                                                   const gfx::Point& press_pt,
                                                   OSExchangeData* data) {
  // Not supporting drag for action buttons inside this container
}

int adrbrowsielActionsContainer::GetDragOperationsForView(View* sender,
                                                      const gfx::Point& p) {
  return ui::DragDropTypes::DRAG_NONE;
}

bool adrbrowsielActionsContainer::CanStartDragForView(View* sender,
                                                  const gfx::Point& press_pt,
                                                  const gfx::Point& p) {
  return false;
}
// end ToolbarActionView::Delegate members

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
// adrbrowsielRewardsActionStubView::Delegate members
void adrbrowsielActionsContainer::OnRewardsStubButtonClicked() {
  // Keep button state visually pressed until new extension button
  // takes over.
  actions_[adrbrowsiel_rewards_extension_id].view_->SetState(
      views::Button::STATE_PRESSED);
  extensions::ExtensionService* service =
           extension_system_->extension_service();
  if (service) {
    is_rewards_pressed_ = true;
    extensions::ComponentLoader* loader = service->component_loader();
          static_cast<extensions::adrbrowsielComponentLoader*>(loader)->
              AddRewardsExtension();

    if (rewards_service_) {
      rewards_service_->StartProcess(base::DoNothing());
    }
  }
}
// end adrbrowsielRewardsActionStubView::Delegate members
#endif

void adrbrowsielActionsContainer::OnExtensionSystemReady() {
  // observe changes in extension system
  extension_registry_observer_.Add(extension_registry_);
  extension_action_observer_.Add(extension_action_api_);
  adrbrowsiel_action_observer_.Add(adrbrowsiel_action_api_);
  // Check if extensions already loaded
  AddAction(adrbrowsiel_extension_id);
#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  AddAction(adrbrowsiel_rewards_extension_id);
#endif
}

// ExtensionRegistry::Observer
void adrbrowsielActionsContainer::OnExtensionLoaded(
    content::BrowserContext* browser_context,
    const extensions::Extension* extension) {
  if (IsContainerAction(extension->id()))
    AddAction(extension);
}

void adrbrowsielActionsContainer::OnExtensionUnloaded(
    content::BrowserContext* browser_context,
    const extensions::Extension* extension,
    extensions::UnloadedExtensionReason reason) {
  if (IsContainerAction(extension->id()))
    RemoveAction(extension->id());
}
// end ExtensionRegistry::Observer

// ExtensionActionAPI::Observer
void adrbrowsielActionsContainer::OnExtensionActionUpdated(
    extensions::ExtensionAction* extension_action,
    content::WebContents* web_contents,
    content::BrowserContext* browser_context) {
  if (IsContainerAction(extension_action->extension_id()))
    UpdateActionState(extension_action->extension_id());
}
// end ExtensionActionAPI::Observer

// adrbrowsielActionAPI::Observer
void adrbrowsielActionsContainer::OnadrbrowsielActionShouldTrigger(
    const std::string& extension_id,
    std::unique_ptr<std::string> ui_relative_path) {
  if (!IsContainerAction(extension_id)) {
    return;
  }
  if (actions_[extension_id].view_controller_) {
    if (ui_relative_path)
      actions_[extension_id].view_controller_
          ->ExecuteActionUI(*ui_relative_path);
    else
      actions_[extension_id].view_controller_->ExecuteAction(
          true, ToolbarActionViewController::InvocationSource::kApi);
  }
}

void adrbrowsielActionsContainer::ChildPreferredSizeChanged(views::View* child) {
  PreferredSizeChanged();
}

// adrbrowsiel Rewards preferences change observers callback
void adrbrowsielActionsContainer::OnadrbrowsielRewardsPreferencesChanged() {
  ShowAction(adrbrowsiel_rewards_extension_id, ShouldAddadrbrowsielRewardsAction());
}
