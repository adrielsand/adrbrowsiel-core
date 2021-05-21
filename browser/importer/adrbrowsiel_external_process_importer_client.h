/* Copyright 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_IMPORTER_adrbrowsiel_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_
#define adrbrowsiel_BROWSER_IMPORTER_adrbrowsiel_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "adrbrowsiel/common/importer/profile_import.mojom.h"
#include "chrome/browser/importer/external_process_importer_client.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "mojo/public/cpp/bindings/remote.h"

class adrbrowsielExternalProcessImporterClient
    : public ExternalProcessImporterClient,
      public adrbrowsiel::mojom::ProfileImportObserver {
 public:
  adrbrowsielExternalProcessImporterClient(
      base::WeakPtr<ExternalProcessImporterHost> importer_host,
      const importer::SourceProfile& source_profile,
      uint16_t items,
      InProcessImporterBridge* bridge);

  adrbrowsielExternalProcessImporterClient(
      const adrbrowsielExternalProcessImporterClient&) = delete;
  adrbrowsielExternalProcessImporterClient& operator=(
      const adrbrowsielExternalProcessImporterClient&) = delete;

  // ExternalProcessImportClient overrides:
  void Start() override;
  void Cancel() override;
  void CloseMojoHandles() override;
  void OnImportItemFinished(importer::ImportItem import_item) override;

  // adrbrowsiel::mojom::ProfileImportObserver overrides:
  void OnCreditCardImportReady(const std::u16string& name_on_card,
                               const std::u16string& expiration_month,
                               const std::u16string& expiration_year,
                               const std::u16string& decrypted_card_number,
                               const std::string& origin) override;

 protected:
  ~adrbrowsielExternalProcessImporterClient() override;

 private:
  // Used to start and stop the actual adrbrowsiel importer running in a different
  // process.
  mojo::Remote<adrbrowsiel::mojom::ProfileImport> adrbrowsiel_profile_import_;

  // Used to receive progress updates from the adrbrowsiel importer.
  mojo::Receiver<adrbrowsiel::mojom::ProfileImportObserver> adrbrowsiel_receiver_{this};
};

#endif  // adrbrowsiel_BROWSER_IMPORTER_adrbrowsiel_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_
