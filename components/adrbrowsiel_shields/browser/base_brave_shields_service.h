/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_BASE_adrbrowsiel_SHIELDS_SERVICE_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_BASE_adrbrowsiel_SHIELDS_SERVICE_H_

#include <stdint.h>

#include <memory>
#include <string>
// TODO(adrbrowsiel): <mutex> is an unapproved C++11 header
#include <mutex>  // NOLINT

#include "base/files/file_path.h"
#include "base/sequenced_task_runner.h"
#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/adrbrowsiel_component.h"
#include "third_party/blink/public/mojom/loader/resource_load_info.mojom-shared.h"
#include "url/gurl.h"

using adrbrowsiel_component_updater::adrbrowsielComponent;

namespace adrbrowsiel_shields {

// The adrbrowsiel shields service in charge of checking adrbrowsiel shields like ad-block,
// tracking protection, etc.
class BaseadrbrowsielShieldsService : public adrbrowsielComponent {
 public:
  explicit BaseadrbrowsielShieldsService(adrbrowsielComponent::Delegate* delegate);
  ~BaseadrbrowsielShieldsService() override;
  bool Start();
  bool IsInitialized() const;
  virtual void ShouldStartRequest(const GURL& url,
                                  blink::mojom::ResourceType resource_type,
                                  const std::string& tab_host,
                                  bool* did_match_rule,
                                  bool* did_match_exception,
                                  bool* did_match_important,
                                  std::string* mock_data_url);

 protected:
  virtual bool Init() = 0;

 private:
  void InitShields();

  bool initialized_;
  std::mutex initialized_mutex_;
  scoped_refptr<base::SequencedTaskRunner> task_runner_;
};

}  // namespace adrbrowsiel_shields

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_BASE_adrbrowsiel_SHIELDS_SERVICE_H_
