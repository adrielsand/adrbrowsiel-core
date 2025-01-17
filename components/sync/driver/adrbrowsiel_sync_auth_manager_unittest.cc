/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/sync/driver/adrbrowsiel_sync_auth_manager.h"

#include <memory>

#include "base/callback_helpers.h"
#include "base/test/mock_callback.h"
#include "base/test/task_environment.h"
#include "adrbrowsiel/common/network_constants.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/network_time_helper.h"
#include "components/signin/public/identity_manager/identity_test_environment.h"
#include "components/sync/engine/sync_credentials.h"
#include "services/network/test/test_url_loader_factory.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace syncer {

namespace {

const char kSyncCode[] =
    "badge unique kiwi orient spring venue piano "
    "lake admit ill roof brother grant hour better "
    "proud cabbage fee slow economy wage final fox cancel";

const char kAccessToken[] =
    "MzEzMjMzMzQzNTM2Mzd8MDBGNkExNjgxODkxQzU5RDZGMEYwNkVDQ0VGQzBFMTQ3QjA2NDE2RD"
    "EzNzE0QkQ3MzE3ODJGRjE1NUZFNjMxMTNBNTE2Qzk2NTFFM0ZGQTEyRDhDMzcyQTcyNUZEMzZG"
    "RjE3QUIxMDRDNDVBNTcyMDVCRkIwNjUwRjgxQ0MyMDl8NTAyMDQyMjcwQzgxNDUyNDdFRDcwQT"
    "E4Rjg3MDIyQTM5ODg2OTAwQUIzNkYyRkZGNjU1NjM1REJFNTE2NzY1RQ==";

const char kAccountId[] =
    "502042270C8145247ED70A18F87022A39886900AB36F2FFF655635DBE516765E";

const char kAccountEmail[] =
  "502042270C8145247ED70A18F87022A3 "
  "9886900AB36F2FFF655635DBE516765E @adrbrowsiel.com";

class adrbrowsielSyncAuthManagerTest : public testing::Test {
 protected:
  using AccountStateChangedCallback =
      SyncAuthManager::AccountStateChangedCallback;
  using CredentialsChangedCallback =
      SyncAuthManager::CredentialsChangedCallback;

  adrbrowsielSyncAuthManagerTest() : identity_env_(&test_url_loader_factory_) {}

  ~adrbrowsielSyncAuthManagerTest() override {}

  std::unique_ptr<adrbrowsielSyncAuthManager> CreateAuthManager(
      const AccountStateChangedCallback& account_state_changed,
      const CredentialsChangedCallback& credentials_changed) {
    return std::make_unique<adrbrowsielSyncAuthManager>(
        identity_env_.identity_manager(), account_state_changed,
        credentials_changed);
  }

  void SetNetworkTime() {
    adrbrowsiel_sync::NetworkTimeHelper::GetInstance()->SetNetworkTimeForTest(
        base::Time::FromJsTime(1234567));
  }

 private:
  base::test::TaskEnvironment task_environment_;
  network::TestURLLoaderFactory test_url_loader_factory_;
  signin::IdentityTestEnvironment identity_env_;
};

TEST_F(adrbrowsielSyncAuthManagerTest, IgnoresEventsIfNotRegistered) {
  base::MockCallback<AccountStateChangedCallback> account_state_changed;
  base::MockCallback<CredentialsChangedCallback> credentials_changed;
  EXPECT_CALL(account_state_changed, Run()).Times(0);
  EXPECT_CALL(credentials_changed, Run()).Times(0);
  auto auth_manager =
      CreateAuthManager(account_state_changed.Get(), credentials_changed.Get());

  // Fire some auth events. We haven't called RegisterForAuthNotifications, so
  // none of this should result in any callback calls.
  auth_manager->DeriveSigningKeys(kSyncCode);
  // Without RegisterForAuthNotifications, the active account should always be
  // reported as empty.
  EXPECT_TRUE(
      auth_manager->GetActiveAccountInfo().account_info.account_id.empty());
}

TEST_F(adrbrowsielSyncAuthManagerTest, GetAccessToken) {
  base::MockCallback<AccountStateChangedCallback> account_state_changed;
  base::MockCallback<CredentialsChangedCallback> credentials_changed;
  EXPECT_CALL(account_state_changed, Run()).Times(1);
  EXPECT_CALL(credentials_changed, Run()).Times(1);
  auto auth_manager =
      CreateAuthManager(account_state_changed.Get(), credentials_changed.Get());

  SetNetworkTime();
  auth_manager->RegisterForAuthNotifications();
  auth_manager->DeriveSigningKeys(kSyncCode);
  auth_manager->ConnectionOpened();

  const std::string kadrbrowsielServerKeyHeaderString =
    std::string(kadrbrowsielServicesKeyHeader) + ": " + adrbrowsiel_SERVICES_KEY;

  ASSERT_EQ(auth_manager->GetCredentials().access_token,
            std::string(kAccessToken) + "\r\n" + kadrbrowsielServerKeyHeaderString);
  EXPECT_TRUE(auth_manager->GetActiveAccountInfo().is_primary);
  EXPECT_EQ(auth_manager->GetActiveAccountInfo().account_info.account_id,
            CoreAccountId::FromString(kAccountId));
  EXPECT_EQ(auth_manager->GetActiveAccountInfo().account_info.email,
            kAccountEmail);
}

TEST_F(adrbrowsielSyncAuthManagerTest, Reset) {
  base::MockCallback<AccountStateChangedCallback> account_state_changed;
  base::MockCallback<CredentialsChangedCallback> credentials_changed;
  EXPECT_CALL(account_state_changed, Run()).Times(2);
  EXPECT_CALL(credentials_changed, Run()).Times(1);
  auto auth_manager =
      CreateAuthManager(account_state_changed.Get(), credentials_changed.Get());

  SetNetworkTime();
  auth_manager->RegisterForAuthNotifications();
  auth_manager->DeriveSigningKeys(kSyncCode);
  auth_manager->ConnectionOpened();

  auth_manager->ResetKeys();
  EXPECT_TRUE(auth_manager->GetCredentials().access_token.empty());
  EXPECT_FALSE(auth_manager->GetActiveAccountInfo().is_primary);
  EXPECT_TRUE(
      auth_manager->GetActiveAccountInfo().account_info.account_id.empty());
}

TEST_F(adrbrowsielSyncAuthManagerTest, MalformedSyncCode) {
  base::MockCallback<AccountStateChangedCallback> account_state_changed;
  base::MockCallback<CredentialsChangedCallback> credentials_changed;
  EXPECT_CALL(account_state_changed, Run()).Times(0);
  EXPECT_CALL(credentials_changed, Run()).Times(0);
  auto auth_manager =
      CreateAuthManager(account_state_changed.Get(), credentials_changed.Get());

  SetNetworkTime();
  auth_manager->RegisterForAuthNotifications();

  auth_manager->DeriveSigningKeys("");
  EXPECT_TRUE(
      auth_manager->GetActiveAccountInfo().account_info.account_id.empty());

  auth_manager->DeriveSigningKeys("adrbrowsiel5566");
  EXPECT_TRUE(
      auth_manager->GetActiveAccountInfo().account_info.account_id.empty());
}

}  // namespace

}  // namespace syncer
