/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_UTILITY_IMPORTER_adrbrowsiel_PROFILE_IMPORT_IMPL_H_
#define adrbrowsiel_UTILITY_IMPORTER_adrbrowsiel_PROFILE_IMPORT_IMPL_H_

#include <memory>
#include <string>

#include "adrbrowsiel/common/importer/profile_import.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver.h"

class adrbrowsielExternalProcessImporterBridge;
class Importer;

namespace base {
class Thread;
}  // namespace base

namespace importer {
struct SourceProfile;
}  // namespace importer

class adrbrowsielProfileImportImpl : public adrbrowsiel::mojom::ProfileImport {
 public:
  explicit adrbrowsielProfileImportImpl(
      mojo::PendingReceiver<adrbrowsiel::mojom::ProfileImport> receiver);
  ~adrbrowsielProfileImportImpl() override;

  adrbrowsielProfileImportImpl(const adrbrowsielProfileImportImpl&) = delete;
  adrbrowsielProfileImportImpl& operator=(const adrbrowsielProfileImportImpl&) = delete;

 private:
  // adrbrowsiel::mojom::ProfileImport overrides:
  void StartImport(
      const importer::SourceProfile& source_profile,
      uint16_t items,
      const base::flat_map<uint32_t, std::string>& localized_strings,
      mojo::PendingRemote<chrome::mojom::ProfileImportObserver> observer,
      mojo::PendingRemote<adrbrowsiel::mojom::ProfileImportObserver> adrbrowsiel_observer)
      override;
  void CancelImport() override;
  void ReportImportItemFinished(importer::ImportItem item) override;

  void ImporterCleanup();

  mojo::Receiver<adrbrowsiel::mojom::ProfileImport> receiver_;
  std::unique_ptr<base::Thread> import_thread_;

  // Bridge object is passed to importer, so that it can send IPC calls
  // directly back to the ProfileImportProcessHost.
  scoped_refptr<adrbrowsielExternalProcessImporterBridge> bridge_;

  // A bitmask of importer::ImportItem.
  uint16_t items_to_import_ = 0;

  // Importer of the appropriate type (Firefox, Safari, IE, etc.)
  scoped_refptr<Importer> importer_;
};

#endif  // adrbrowsiel_UTILITY_IMPORTER_adrbrowsiel_PROFILE_IMPORT_IMPL_H_
