/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_PERMISSIONS_MOCK_PERMISSION_LIFETIME_PROMPT_FACTORY_H_
#define adrbrowsiel_BROWSER_PERMISSIONS_MOCK_PERMISSION_LIFETIME_PROMPT_FACTORY_H_

#include <memory>
#include <vector>

#include "adrbrowsiel/browser/permissions/mock_permission_lifetime_prompt.h"
#include "components/permissions/permission_prompt.h"
#include "components/permissions/permission_request.h"
#include "components/permissions/permission_request_manager.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "url/gurl.h"

namespace content {
class WebContents;
}

namespace permissions {
enum class RequestType;

class MockPermissionLifetimePromptFactory {
 public:
  explicit MockPermissionLifetimePromptFactory(
      PermissionRequestManager* manager);
  MockPermissionLifetimePromptFactory(
      const MockPermissionLifetimePromptFactory&) = delete;
  MockPermissionLifetimePromptFactory& operator=(
      const MockPermissionLifetimePromptFactory&) = delete;
  ~MockPermissionLifetimePromptFactory();

  // Create method called by the PRM to show a bubble.
  std::unique_ptr<PermissionPrompt> Create(
      content::WebContents* web_contents,
      PermissionPrompt::Delegate* delegate);

  void set_response_type(PermissionRequestManager::AutoResponseType type) {
    response_type_ = type;
  }

  PermissionRequestManager::AutoResponseType response_type() {
    return response_type_;
  }

  // Number of times |Show| was called on any bubble.
  int show_count() { return show_count_; }
  bool IsVisible() const;
  void WaitForPermissionBubble();
  void HideView(MockPermissionLifetimePrompt* view);

  MOCK_METHOD(void, OnPermissionPromptCreated, (MockPermissionLifetimePrompt*));

 private:
  int show_count_;

  std::vector<MockPermissionLifetimePrompt*> prompts_;
  PermissionRequestManager::AutoResponseType response_type_;

  base::RepeatingClosure show_bubble_quit_closure_;

  // The bubble manager that will be associated with this factory.
  PermissionRequestManager* manager_;
};

}  // namespace permissions

#endif  // adrbrowsiel_BROWSER_PERMISSIONS_MOCK_PERMISSION_LIFETIME_PROMPT_FACTORY_H_
