/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/attestation/attestation.h"
#include "bat/ledger/internal/ledger_impl.h"

namespace ledger {
namespace attestation {

Attestation::Attestation(LedgerImpl* ledger) : ledger_(ledger) {
}

Attestation::~Attestation() = default;

}  // namespace attestation
}  // namespace ledger
