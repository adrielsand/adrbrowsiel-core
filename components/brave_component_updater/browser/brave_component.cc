/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/adrbrowsiel_component.h"

#include <utility>

#include "base/bind.h"
#include "base/logging.h"
#include "base/sequenced_task_runner.h"

namespace adrbrowsiel_component_updater {

adrbrowsielComponent::adrbrowsielComponent(Delegate* delegate)
    : delegate_(delegate),
      weak_factory_(this) {}

adrbrowsielComponent::~adrbrowsielComponent() {
}

void adrbrowsielComponent::Register(const std::string& component_name,
                              const std::string& component_id,
                              const std::string& component_base64_public_key) {
  VLOG(2) << "register component: " << component_id;
  component_name_ = component_name;
  component_id_ = component_id;
  component_base64_public_key_ = component_base64_public_key;

  auto registered_callback =
      base::BindOnce(&adrbrowsielComponent::OnComponentRegistered,
                     delegate_,
                     component_id);
  auto ready_callback =
      base::BindRepeating(&adrbrowsielComponent::OnComponentReadyInternal,
                          weak_factory_.GetWeakPtr(),
                          component_id);

  delegate_->Register(component_name_,
                      component_base64_public_key_,
                      std::move(registered_callback),
                      ready_callback);
}

bool adrbrowsielComponent::Unregister() {
  VLOG(2) << "unregister component: " << component_id_;
  return delegate_->Unregister(component_id_);
}

scoped_refptr<base::SequencedTaskRunner> adrbrowsielComponent::GetTaskRunner() {
  return delegate_->GetTaskRunner();
}

void adrbrowsielComponent::AddObserver(ComponentObserver* observer) {
  DCHECK(delegate_);
  delegate_->AddObserver(observer);
}

void adrbrowsielComponent::RemoveObserver(ComponentObserver* observer) {
  DCHECK(delegate_);
  delegate_->RemoveObserver(observer);
}

void adrbrowsielComponent::OnComponentReadyInternal(
    const std::string& component_id,
    const base::FilePath& install_dir,
    const std::string& manifest) {
  VLOG(2) << "component ready: " << manifest;
  OnComponentReady(component_id, install_dir, manifest);
}

void adrbrowsielComponent::OnComponentReady(
    const std::string& component_id,
    const base::FilePath& install_dir,
    const std::string& manifest) {}

// static
void adrbrowsielComponent::OnComponentRegistered(
    Delegate* delegate,
    const std::string& component_id) {
  VLOG(2) << "component registered: " << component_id;
  delegate->OnDemandUpdate(component_id);
}

adrbrowsielComponent::Delegate* adrbrowsielComponent::delegate() {
  return delegate_;
}

}  // namespace adrbrowsiel_component_updater
