/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_SIGNIN_INTERNAL_IDENTITY_MANAGER_adrbrowsiel_MUTABLE_PROFILE_OAUTH2_TOKEN_SERVICE_DELEGATE_H_
#define adrbrowsiel_COMPONENTS_SIGNIN_INTERNAL_IDENTITY_MANAGER_adrbrowsiel_MUTABLE_PROFILE_OAUTH2_TOKEN_SERVICE_DELEGATE_H_

#include "components/signin/internal/identity_manager/mutable_profile_oauth2_token_service_delegate.h"

class adrbrowsielMutableProfileOAuth2TokenServiceDelegate
    : public MutableProfileOAuth2TokenServiceDelegate {
 public:
  adrbrowsielMutableProfileOAuth2TokenServiceDelegate(
      SigninClient* client,
      AccountTrackerService* account_tracker_service,
      network::NetworkConnectionTracker* network_connection_tracker,
      scoped_refptr<TokenWebData> token_web_data,
      signin::AccountConsistencyMethod account_consistency,
      bool revoke_all_tokens_on_load,
      FixRequestErrorCallback fix_request_error_callback);
  ~adrbrowsielMutableProfileOAuth2TokenServiceDelegate() override;
  adrbrowsielMutableProfileOAuth2TokenServiceDelegate(
      const adrbrowsielMutableProfileOAuth2TokenServiceDelegate&) = delete;
  adrbrowsielMutableProfileOAuth2TokenServiceDelegate& operator=(
      const adrbrowsielMutableProfileOAuth2TokenServiceDelegate&) = delete;

  void LoadCredentials(const CoreAccountId& primary_account_id) override;

 private:
  AccountTrackerService* account_tracker_service_;
};

#endif  // adrbrowsiel_COMPONENTS_SIGNIN_INTERNAL_IDENTITY_MANAGER_adrbrowsiel_MUTABLE_PROFILE_OAUTH2_TOKEN_SERVICE_DELEGATE_H_
