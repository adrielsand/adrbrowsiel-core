/* Copyright 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/utility/importer/adrbrowsiel_external_process_importer_bridge.h"

#include <utility>

adrbrowsielExternalProcessImporterBridge::adrbrowsielExternalProcessImporterBridge(
    const base::flat_map<uint32_t, std::string>& localized_strings,
    mojo::SharedRemote<chrome::mojom::ProfileImportObserver> observer,
    mojo::SharedRemote<adrbrowsiel::mojom::ProfileImportObserver> adrbrowsiel_observer)
    : ExternalProcessImporterBridge(std::move(localized_strings),
                                         std::move(observer)),
      adrbrowsiel_observer_(std::move(adrbrowsiel_observer)) {}

adrbrowsielExternalProcessImporterBridge::
    ~adrbrowsielExternalProcessImporterBridge() = default;

void adrbrowsielExternalProcessImporterBridge::SetCreditCard(
    const std::u16string& name_on_card,
    const std::u16string& expiration_month,
    const std::u16string& expiration_year,
    const std::u16string& decrypted_card_number,
    const std::string& origin) {
  adrbrowsiel_observer_->OnCreditCardImportReady(
      name_on_card, expiration_month,
      expiration_year, decrypted_card_number,
      origin);
}
