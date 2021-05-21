/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/infobars/adrbrowsiel_wayback_machine_delegate_impl.h"

#include "base/command_line.h"
#include "adrbrowsiel/common/adrbrowsiel_switches.h"
#include "adrbrowsiel/components/adrbrowsiel_wayback_machine/adrbrowsiel_wayback_machine_infobar_delegate.h"
#include "adrbrowsiel/components/adrbrowsiel_wayback_machine/adrbrowsiel_wayback_machine_tab_helper.h"
#include "components/infobars/core/infobar.h"
#include "chrome/browser/infobars/infobar_service.h"

// static
void adrbrowsielWaybackMachineDelegateImpl::AttachTabHelperIfNeeded(
    content::WebContents* web_contents) {
  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableadrbrowsielWaybackMachineExtension)) {
    adrbrowsielWaybackMachineTabHelper::CreateForWebContents(web_contents);
    auto* tab_helper =
        adrbrowsielWaybackMachineTabHelper::FromWebContents(web_contents);
    tab_helper->set_delegate(
        std::make_unique<adrbrowsielWaybackMachineDelegateImpl>());
  }
}

adrbrowsielWaybackMachineDelegateImpl::adrbrowsielWaybackMachineDelegateImpl() = default;
adrbrowsielWaybackMachineDelegateImpl::~adrbrowsielWaybackMachineDelegateImpl() = default;


void adrbrowsielWaybackMachineDelegateImpl::CreateInfoBar(
    content::WebContents* web_contents) {
  InfoBarService::FromWebContents(web_contents)->AddInfoBar(
      CreateInfoBarView(
          std::make_unique<adrbrowsielWaybackMachineInfoBarDelegate>(),
          web_contents),
      true);
}
