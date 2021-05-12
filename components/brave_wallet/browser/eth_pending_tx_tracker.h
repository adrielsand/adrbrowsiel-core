/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ETH_PENDING_TX_TRACKER_H_
#define BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ETH_PENDING_TX_TRACKER_H_

#include "base/memory/weak_ptr.h"

namespace brave_wallet {

class EthNonceTracker;
class EthJsonRpcController;
class EthTxStateManager;

class EthPendingTxTracker {
 public:
  EthPendingTxTracker(EthTxStateManager* tx_state_manager,
                      EthJsonRpcController* rpc_controller,
                      EthNonceTracker* nonce_tracker);
  ~EthPendingTxTracker();
  EthPendingTxTracker(const EthPendingTxTracker&) = delete;
  EthPendingTxTracker operator=(const EthPendingTxTracker&) = delete;

  void UpdatePendingTransactions();

 private:
  EthTxStateManager* tx_state_manager_;
  EthJsonRpcController* rpc_controller_;
  EthNonceTracker* nonce_tracker_;

  base::WeakPtrFactory<EthPendingTxTracker> weak_factory_;
};

}  // namespace brave_wallet

#endif  // BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ETH_PENDING_TX_TRACKER_H_
