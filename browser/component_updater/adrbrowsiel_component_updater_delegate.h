/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_COMPONENT_UPDATER_adrbrowsiel_COMPONENT_UPDATER_DELEGATE_H_
#define adrbrowsiel_BROWSER_COMPONENT_UPDATER_adrbrowsiel_COMPONENT_UPDATER_DELEGATE_H_

#include <string>

#include "base/macros.h"
#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/adrbrowsiel_component.h"
#include "components/component_updater/component_updater_service.h"

using adrbrowsiel_component_updater::adrbrowsielComponent;

namespace base {
class SequencedTaskRunner;
}

class PrefService;

namespace adrbrowsiel {

class adrbrowsielComponentUpdaterDelegate : public adrbrowsielComponent::Delegate {
 public:
  adrbrowsielComponentUpdaterDelegate();
  ~adrbrowsielComponentUpdaterDelegate() override;

  using ComponentObserver = update_client::UpdateClient::Observer;
  // adrbrowsiel_component_updater::adrbrowsielComponent::Delegate implementation
  void Register(const std::string& component_name,
                const std::string& component_base64_public_key,
                base::OnceClosure registered_callback,
                adrbrowsielComponent::ReadyCallback ready_callback) override;
  bool Unregister(const std::string& component_id) override;
  void OnDemandUpdate(const std::string& component_id) override;

  void AddObserver(ComponentObserver* observer) override;
  void RemoveObserver(ComponentObserver* observer) override;

  scoped_refptr<base::SequencedTaskRunner> GetTaskRunner() override;

  const std::string locale() const override;
  PrefService* local_state() override;

 private:
  scoped_refptr<base::SequencedTaskRunner> task_runner_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielComponentUpdaterDelegate);
};

}  // namespace adrbrowsiel

#endif  // adrbrowsiel_BROWSER_COMPONENT_UPDATER_adrbrowsiel_COMPONENT_UPDATER_DELEGATE_H_
