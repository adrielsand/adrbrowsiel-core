/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_COMPONENT_UPDATER_BROWSER_LOCAL_DATA_FILES_SERVICE_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_COMPONENT_UPDATER_BROWSER_LOCAL_DATA_FILES_SERVICE_H_

#include <memory>
#include <string>

#include "base/files/file_path.h"
#include "base/observer_list.h"
#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/adrbrowsiel_component.h"

namespace adrbrowsiel_component_updater {

class LocalDataFilesObserver;

const char kLocalDataFilesComponentName[] = "adrbrowsiel Local Data Updater";
const char kLocalDataFilesComponentId[] = "afalakplffnnnlkncjhbmahjfjhmlkal";
const char kLocalDataFilesComponentBase64PublicKey[] =
    "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAs4TIQXRCftLpGmQZxmm6"
    "AU8pqGKLoDyi537HGQyRKcK7j/CSXCf3vwJr7xkV72p7bayutuzyNZ3740QxBPie"
    "sfBOp8bBb8d2VgTHP3b+SuNmK/rsSRsMRhT05x8AAr/7ab6U3rW0Gsalm2653xnn"
    "QS8vt0s62xQTmC+UMXowaSLUZ0Be/TOu6lHZhOeo0NBMKc6PkOu0R1EEfP7dJR6S"
    "M/v4dBUBZ1HXcuziVbCXVyU51opZCMjlxyUlQR9pTGk+Zh5sDn1Vw1MwLnWiEfQ4"
    "EGL1V7GeI4vgLoOLgq7tmhEratHGCfC1IHm9luMACRr/ybMI6DQJOvgBvecb292F"
    "xQIDAQAB";

// The component in charge of delegating access to different DAT files
// such as tracking protection.
class LocalDataFilesService : public adrbrowsielComponent {
 public:
  explicit LocalDataFilesService(adrbrowsielComponent::Delegate* delegate);
  ~LocalDataFilesService() override;
  bool Start();
  bool IsInitialized() const { return initialized_; }
  void AddObserver(LocalDataFilesObserver* observer);
  void RemoveObserver(LocalDataFilesObserver* observer);

  static void SetComponentIdAndBase64PublicKeyForTest(
      const std::string& component_id,
      const std::string& component_base64_public_key);

 protected:
  void OnComponentReady(const std::string& component_id,
      const base::FilePath& install_dir,
      const std::string& manifest) override;

 private:
  static std::string g_local_data_files_component_id_;
  static std::string g_local_data_files_component_base64_public_key_;

  bool initialized_;
  base::ObserverList<LocalDataFilesObserver>::Unchecked observers_;

  DISALLOW_COPY_AND_ASSIGN(LocalDataFilesService);
};

// Creates the LocalDataFilesService
std::unique_ptr<LocalDataFilesService>
LocalDataFilesServiceFactory(adrbrowsielComponent::Delegate* delegate);

}  // namespace adrbrowsiel_component_updater

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_COMPONENT_UPDATER_BROWSER_LOCAL_DATA_FILES_SERVICE_H_
