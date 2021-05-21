/* Copyright 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_IMPORTER_adrbrowsiel_IN_PROCESS_IMPORTER_BRIDGE_H_
#define adrbrowsiel_BROWSER_IMPORTER_adrbrowsiel_IN_PROCESS_IMPORTER_BRIDGE_H_

#include <string>

#include "adrbrowsiel/common/importer/adrbrowsiel_importer_bridge.h"
#include "chrome/browser/importer/in_process_importer_bridge.h"

class adrbrowsielInProcessImporterBridge : public InProcessImporterBridge,
                                     public adrbrowsielImporterBridge {
 public:
  using InProcessImporterBridge::InProcessImporterBridge;

  adrbrowsielInProcessImporterBridge(const adrbrowsielInProcessImporterBridge&) = delete;
  adrbrowsielInProcessImporterBridge operator=(
      const adrbrowsielInProcessImporterBridge&) = delete;

  // adrbrowsielImporterBridge overrides:
  void SetCreditCard(const std::u16string& name_on_card,
                     const std::u16string& expiration_month,
                     const std::u16string& expiration_year,
                     const std::u16string& decrypted_card_number,
                     const std::string& origin) override;

 private:
  ~adrbrowsielInProcessImporterBridge() override;
};

#endif  // adrbrowsiel_BROWSER_IMPORTER_adrbrowsiel_IN_PROCESS_IMPORTER_BRIDGE_H_
