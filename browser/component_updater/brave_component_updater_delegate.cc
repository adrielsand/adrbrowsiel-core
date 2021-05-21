/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/component_updater/adrbrowsiel_component_updater_delegate.h"

#include <utility>

#include "base/sequenced_task_runner.h"
#include "base/task/post_task.h"
#include "base/task/thread_pool.h"
#include "adrbrowsiel/browser/component_updater/adrbrowsiel_component_installer.h"
#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/adrbrowsiel_on_demand_updater.h"
#include "chrome/browser/browser_process.h"
#include "components/component_updater/component_updater_service.h"

using adrbrowsiel_component_updater::adrbrowsielComponent;
using adrbrowsiel_component_updater::adrbrowsielOnDemandUpdater;

namespace adrbrowsiel {

adrbrowsielComponentUpdaterDelegate::adrbrowsielComponentUpdaterDelegate()
    : task_runner_(base::ThreadPool::CreateSequencedTaskRunner(
          {base::MayBlock(), base::TaskPriority::USER_VISIBLE,
           base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN})) {}

adrbrowsielComponentUpdaterDelegate::~adrbrowsielComponentUpdaterDelegate() {}

void adrbrowsielComponentUpdaterDelegate::Register(
    const std::string& component_name,
    const std::string& component_base64_public_key,
    base::OnceClosure registered_callback,
    adrbrowsielComponent::ReadyCallback ready_callback) {
  adrbrowsiel::RegisterComponent(g_browser_process->component_updater(),
                           component_name,
                           component_base64_public_key,
                           std::move(registered_callback),
                           std::move(ready_callback));
}

bool adrbrowsielComponentUpdaterDelegate::Unregister(
    const std::string& component_id) {
  return g_browser_process->component_updater()->UnregisterComponent(
      component_id);
}

void adrbrowsielComponentUpdaterDelegate::OnDemandUpdate(
    const std::string& component_id) {
  adrbrowsielOnDemandUpdater::GetInstance()->OnDemandUpdate(component_id);
}

void adrbrowsielComponentUpdaterDelegate::AddObserver(ComponentObserver* observer) {
  g_browser_process->component_updater()->AddObserver(observer);
}

void adrbrowsielComponentUpdaterDelegate::RemoveObserver(
    ComponentObserver* observer) {
  g_browser_process->component_updater()->RemoveObserver(observer);
}

scoped_refptr<base::SequencedTaskRunner>
adrbrowsielComponentUpdaterDelegate::GetTaskRunner() {
  return task_runner_;
}

const std::string adrbrowsielComponentUpdaterDelegate::locale() const {
  return g_browser_process->GetApplicationLocale();
}

PrefService* adrbrowsielComponentUpdaterDelegate::local_state() {
  return g_browser_process->local_state();
}

}  // namespace adrbrowsiel
