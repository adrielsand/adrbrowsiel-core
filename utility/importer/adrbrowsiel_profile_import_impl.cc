/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/utility/importer/adrbrowsiel_profile_import_impl.h"

#include <utility>

#include "base/bind.h"
#include "base/command_line.h"
#include "base/location.h"
#include "base/memory/ref_counted.h"
#include "base/single_thread_task_runner.h"
#include "base/strings/utf_string_conversions.h"
#include "base/threading/thread.h"
#include "base/threading/thread_task_runner_handle.h"
#include "adrbrowsiel/utility/importer/adrbrowsiel_external_process_importer_bridge.h"
#include "adrbrowsiel/utility/importer/chrome_importer.h"
#include "build/build_config.h"
#include "chrome/common/importer/profile_import.mojom.h"
#include "chrome/utility/importer/external_process_importer_bridge.h"
#include "chrome/utility/importer/importer.h"
#include "content/public/utility/utility_thread.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "mojo/public/cpp/bindings/shared_remote.h"

namespace {

scoped_refptr<Importer> CreateImporterByType(importer::ImporterType type) {
  switch (type) {
    case importer::TYPE_CHROME:
      return new ChromeImporter();
    default:
      NOTREACHED();
      return nullptr;
  }
}

}  // namespace

adrbrowsielProfileImportImpl::adrbrowsielProfileImportImpl(
    mojo::PendingReceiver<adrbrowsiel::mojom::ProfileImport> receiver)
    : receiver_(this, std::move(receiver)) {}

adrbrowsielProfileImportImpl::~adrbrowsielProfileImportImpl() = default;

void adrbrowsielProfileImportImpl::StartImport(
    const importer::SourceProfile& source_profile,
    uint16_t items,
    const base::flat_map<uint32_t, std::string>& localized_strings,
    mojo::PendingRemote<chrome::mojom::ProfileImportObserver> observer,
    mojo::PendingRemote<adrbrowsiel::mojom::ProfileImportObserver> adrbrowsiel_observer) {
  // Signal change to OSCrypt password for importing from Chrome/Chromium
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  if (base::StartsWith(base::UTF16ToUTF8(source_profile.importer_name),
                       "Chrome", base::CompareCase::SENSITIVE)) {
    command_line->AppendSwitch("import-chrome");
  } else if (base::StartsWith(base::UTF16ToUTF8(source_profile.importer_name),
                              "Chromium", base::CompareCase::SENSITIVE)) {
    command_line->AppendSwitch("import-chromium");
  }

  content::UtilityThread::Get()->EnsureBlinkInitialized();
  importer_ = CreateImporterByType(source_profile.importer_type);
  if (!importer_.get()) {
    mojo::Remote<chrome::mojom::ProfileImportObserver>(std::move(observer))
        ->OnImportFinished(false, "Importer could not be created.");
    return;
  }

  items_to_import_ = items;

  // Create worker thread in which importer runs.
  import_thread_.reset(new base::Thread("import_thread"));
#if defined(OS_WIN)
  import_thread_->init_com_with_mta(false);
#endif
  if (!import_thread_->Start()) {
    NOTREACHED();
    ImporterCleanup();
  }
  bridge_ = new adrbrowsielExternalProcessImporterBridge(
      localized_strings,
      mojo::SharedRemote<chrome::mojom::ProfileImportObserver>(
          std::move(observer)),
      mojo::SharedRemote<adrbrowsiel::mojom::ProfileImportObserver>(
          std::move(adrbrowsiel_observer)));
  import_thread_->task_runner()->PostTask(
      FROM_HERE,
      base::BindOnce(&Importer::StartImport, importer_, source_profile, items,
                     base::RetainedRef(bridge_)));
}

void adrbrowsielProfileImportImpl::CancelImport() {
  ImporterCleanup();
}

void adrbrowsielProfileImportImpl::ReportImportItemFinished(
    importer::ImportItem item) {
  items_to_import_ ^= item;  // Remove finished item from mask.
  if (items_to_import_ == 0) {
    ImporterCleanup();
  }
}

void adrbrowsielProfileImportImpl::ImporterCleanup() {
  importer_->Cancel();
  importer_.reset();
  bridge_.reset();
  import_thread_.reset();
}
