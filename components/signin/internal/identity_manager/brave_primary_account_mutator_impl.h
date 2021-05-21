/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_SIGNIN_INTERNAL_IDENTITY_MANAGER_adrbrowsiel_PRIMARY_ACCOUNT_MUTATOR_IMPL_H_
#define adrbrowsiel_COMPONENTS_SIGNIN_INTERNAL_IDENTITY_MANAGER_adrbrowsiel_PRIMARY_ACCOUNT_MUTATOR_IMPL_H_

#include "components/signin/internal/identity_manager/primary_account_mutator_impl.h"

namespace signin {

class adrbrowsielPrimaryAccountMutatorImpl : public PrimaryAccountMutatorImpl {
 public:
  adrbrowsielPrimaryAccountMutatorImpl(
      AccountTrackerService* account_tracker,
      ProfileOAuth2TokenService* token_service,
      PrimaryAccountManager* primary_account_manager,
      PrefService* pref_service,
      signin::AccountConsistencyMethod account_consistency);
  ~adrbrowsielPrimaryAccountMutatorImpl() override;

#if !BUILDFLAG(IS_CHROMEOS_ASH)
  bool ClearPrimaryAccount(
      signin_metrics::ProfileSignout source_metric,
      signin_metrics::SignoutDelete delete_metric) override;
#endif
};

}  //  namespace signin

#endif  // adrbrowsiel_COMPONENTS_SIGNIN_INTERNAL_IDENTITY_MANAGER_adrbrowsiel_PRIMARY_ACCOUNT_MUTATOR_IMPL_H_
