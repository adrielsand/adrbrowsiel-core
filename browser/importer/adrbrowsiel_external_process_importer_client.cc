/* Copyright 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/importer/adrbrowsiel_external_process_importer_client.h"

#include <utility>

#include "base/bind.h"
#include "adrbrowsiel/browser/importer/adrbrowsiel_in_process_importer_bridge.h"
#include "chrome/browser/service_sandbox_type.h"
#include "chrome/grit/generated_resources.h"
#include "content/public/browser/service_process_host.h"

namespace {
bool ShouldUseadrbrowsielImporter(importer::ImporterType type) {
  if (type == importer::TYPE_CHROME)
    return true;

  return false;
}
}  // namespace

adrbrowsielExternalProcessImporterClient::adrbrowsielExternalProcessImporterClient(
    base::WeakPtr<ExternalProcessImporterHost> importer_host,
    const importer::SourceProfile& source_profile,
    uint16_t items,
    InProcessImporterBridge* bridge)
    : ExternalProcessImporterClient(
          importer_host, source_profile, items, bridge) {}

adrbrowsielExternalProcessImporterClient::
    ~adrbrowsielExternalProcessImporterClient() = default;

void adrbrowsielExternalProcessImporterClient::Start() {
  if (!ShouldUseadrbrowsielImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::Start();
    return;
  }

  AddRef();  // balanced in Cleanup.

  auto options = content::ServiceProcessHost::Options()
                     .WithDisplayName(IDS_UTILITY_PROCESS_PROFILE_IMPORTER_NAME)
                     .Pass();
  options.sandbox_type =
      content::GetServiceSandboxType<adrbrowsiel::mojom::ProfileImport>();
  content::ServiceProcessHost::Launch(
      adrbrowsiel_profile_import_.BindNewPipeAndPassReceiver(), std::move(options));

  adrbrowsiel_profile_import_.set_disconnect_handler(
      base::BindOnce(&ExternalProcessImporterClient::OnProcessCrashed, this));

  base::flat_map<uint32_t, std::string> localized_strings;
  adrbrowsiel_profile_import_->StartImport(
      source_profile_, items_, localized_strings,
      receiver_.BindNewPipeAndPassRemote(),
      adrbrowsiel_receiver_.BindNewPipeAndPassRemote());
}

void adrbrowsielExternalProcessImporterClient::Cancel() {
  if (!ShouldUseadrbrowsielImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::Cancel();
    return;
  }

  if (cancelled_)
    return;

  cancelled_ = true;
  adrbrowsiel_profile_import_->CancelImport();
  CloseMojoHandles();
  Release();
}

void adrbrowsielExternalProcessImporterClient::CloseMojoHandles() {
  if (!ShouldUseadrbrowsielImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::CloseMojoHandles();
    return;
  }

  adrbrowsiel_profile_import_.reset();
  adrbrowsiel_receiver_.reset();
  receiver_.reset();
}

void adrbrowsielExternalProcessImporterClient::OnImportItemFinished(
    importer::ImportItem import_item) {
  if (!ShouldUseadrbrowsielImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::OnImportItemFinished(import_item);
    return;
  }

  if (cancelled_)
    return;

  bridge_->NotifyItemEnded(import_item);
  adrbrowsiel_profile_import_->ReportImportItemFinished(import_item);
}

void adrbrowsielExternalProcessImporterClient::OnCreditCardImportReady(
    const std::u16string& name_on_card,
    const std::u16string& expiration_month,
    const std::u16string& expiration_year,
    const std::u16string& decrypted_card_number,
    const std::string& origin) {
  if (cancelled_)
    return;

  static_cast<adrbrowsielInProcessImporterBridge*>(
      bridge_.get())->SetCreditCard(name_on_card,
                                    expiration_month,
                                    expiration_year,
                                    decrypted_card_number,
                                    origin);
}
