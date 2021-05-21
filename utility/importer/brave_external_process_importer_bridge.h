/* Copyright 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_UTILITY_IMPORTER_adrbrowsiel_EXTERNAL_PROCESS_IMPORTER_BRIDGE_H_
#define adrbrowsiel_UTILITY_IMPORTER_adrbrowsiel_EXTERNAL_PROCESS_IMPORTER_BRIDGE_H_

#include <string>

#include "adrbrowsiel/common/importer/adrbrowsiel_importer_bridge.h"
#include "adrbrowsiel/common/importer/profile_import.mojom.h"
#include "chrome/utility/importer/external_process_importer_bridge.h"

class adrbrowsielExternalProcessImporterBridge : public ExternalProcessImporterBridge,
                                           public adrbrowsielImporterBridge {
 public:
  adrbrowsielExternalProcessImporterBridge(
      const base::flat_map<uint32_t, std::string>& localized_strings,
      mojo::SharedRemote<chrome::mojom::ProfileImportObserver> observer,
      mojo::SharedRemote<adrbrowsiel::mojom::ProfileImportObserver> adrbrowsiel_observer);

  adrbrowsielExternalProcessImporterBridge(
      const adrbrowsielExternalProcessImporterBridge&) = delete;
  adrbrowsielExternalProcessImporterBridge& operator=(
      const adrbrowsielExternalProcessImporterBridge&) = delete;

  void SetCreditCard(const std::u16string& name_on_card,
                     const std::u16string& expiration_month,
                     const std::u16string& expiration_year,
                     const std::u16string& decrypted_card_number,
                     const std::string& origin) override;

 private:
  ~adrbrowsielExternalProcessImporterBridge() override;

  mojo::SharedRemote<adrbrowsiel::mojom::ProfileImportObserver> adrbrowsiel_observer_;
};

#endif  // adrbrowsiel_UTILITY_IMPORTER_adrbrowsiel_EXTERNAL_PROCESS_IMPORTER_BRIDGE_H_
