/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_wallet/browser/eth_pending_tx_tracker.h"

#include "base/synchronization/lock.h"
#include "base/logging.h"
#include "brave/components/brave_wallet/browser/eth_nonce_tracker.h"
#include "brave/components/brave_wallet/browser/eth_tx_state_manager.h"

namespace brave_wallet {

EthPendingTxTracker::EthPendingTxTracker(EthTxStateManager* tx_state_manager,
                                         EthJsonRpcController* rpc_controller,
                                         EthNonceTracker* nonce_tracker)
    : tx_state_manager_(tx_state_manager),
      rpc_controller_(rpc_controller),
      nonce_tracker_(nonce_tracker),
      weak_factory_(this) {
        LOG(ERROR)<<tx_state_manager_;
        LOG(ERROR)<<rpc_controller_;
        LOG(ERROR)<<nonce_tracker_;
      }
EthPendingTxTracker::~EthPendingTxTracker() = default;

void EthPendingTxTracker::UpdatePendingTransactions() {
  base::Lock* nonce_lock = nonce_tracker_->GetLock();
  if (!nonce_lock->Try())
    return;

  auto pending_transactions = tx_state_manager_->GetTransactionsByStatus(
      EthTxStateManager::TransactionStatus::SUBMITTED, base::Optional<EthAddress>());

  nonce_lock->Release();
}

}  // namespace brave_wallet
