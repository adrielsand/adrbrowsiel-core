/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_COMPONENT_UPDATER_RESOURCE_COMPONENT_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_COMPONENT_UPDATER_RESOURCE_COMPONENT_H_

#include <map>
#include <string>

#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/component_updater/resource_component_observer.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/component_updater/resource_info.h"
#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/adrbrowsiel_component.h"

namespace adrbrowsiel_ads {

class ResourceComponent : public adrbrowsiel_component_updater::adrbrowsielComponent {
 public:
  explicit ResourceComponent(Delegate* delegate);
  ~ResourceComponent() override;

  ResourceComponent(const ResourceComponent&) = delete;
  ResourceComponent& operator=(const ResourceComponent&) = delete;

  void RegisterComponentsForLocale(const std::string& locale);

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);
  void NotifyObservers(const std::string& id);

  base::Optional<base::FilePath> GetPath(const std::string& id,
                                         const int version);

 private:
  void RegisterComponentForCountryCode(const std::string& country_code);

  void RegisterComponentForLanguageCode(const std::string& language_code);

  void OnComponentReady(const std::string& component_id,
                        const base::FilePath& install_dir,
                        const std::string& manifest) override;

  void OnGetManifest(const std::string& component_id,
                     const base::FilePath& install_dir,
                     const std::string& json);

  std::map<std::string, ResourceInfo> resources_;
  base::ObserverList<Observer> observers_;
  base::WeakPtrFactory<ResourceComponent> weak_factory_{this};
};

}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_COMPONENT_UPDATER_RESOURCE_COMPONENT_H_
