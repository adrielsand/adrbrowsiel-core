// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef adrbrowsiel_BROWSER_UI_WEBUI_NEW_TAB_PAGE_INSTANT_SERVICE_MESSAGE_HANDLER_H_
#define adrbrowsiel_BROWSER_UI_WEBUI_NEW_TAB_PAGE_INSTANT_SERVICE_MESSAGE_HANDLER_H_

#include "base/values.h"
#include "chrome/browser/search/instant_service_observer.h"
#include "chrome/common/search/instant_types.h"
#include "content/public/browser/web_ui_message_handler.h"

class InstantService;
class Profile;
namespace content {
class WebUIDataSource;
}

// Handles messages from InstantService (used for top sites)
class InstantServiceMessageHandler : public content::WebUIMessageHandler,
                                     public InstantServiceObserver {
 public:
  explicit InstantServiceMessageHandler(Profile* profile);
  ~InstantServiceMessageHandler() override;

 private:
  // WebUIMessageHandler:
  void RegisterMessages() override;

  // InstantServiceObserver:
  void MostVisitedInfoChanged(const InstantMostVisitedInfo& info) override;

  // handlers
  void HandleUpdateMostVisitedInfo(const base::ListValue* args);
  void HandleDeleteMostVisitedTile(const base::ListValue* args);
  void HandleReorderMostVisitedTile(const base::ListValue* args);
  void HandleRestoreMostVisitedDefaults(const base::ListValue* args);
  void HandleUndoMostVisitedTileAction(const base::ListValue* args);
  void HandleSetMostVisitedSettings(const base::ListValue* args);
  void HandleEditTopSite(const base::ListValue* args);
  void HandleAddNewTopSite(const base::ListValue* args);

  int GetCustomLinksNum() const;

  GURL last_blacklisted_;
  // Weak pointer.
  Profile* profile_;
  InstantService* instant_service_;
  base::Value top_site_tiles_;

  DISALLOW_COPY_AND_ASSIGN(InstantServiceMessageHandler);
};

#endif  // adrbrowsiel_BROWSER_UI_WEBUI_NEW_TAB_PAGE_INSTANT_SERVICE_MESSAGE_HANDLER_H_
