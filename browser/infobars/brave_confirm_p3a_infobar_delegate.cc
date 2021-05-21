/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/infobars/adrbrowsiel_confirm_p3a_infobar_delegate.h"

#include <memory>
#include <utility>

#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/common/url_constants.h"
#include "adrbrowsiel/components/p3a/pref_names.h"
#include "adrbrowsiel/grit/adrbrowsiel_generated_resources.h"
#include "chrome/browser/infobars/infobar_service.h"
#include "chrome/grit/chromium_strings.h"
#include "components/infobars/core/infobar.h"
#include "components/prefs/pref_service.h"
#include "components/strings/grit/components_strings.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/views/vector_icons.h"

// static
void adrbrowsielConfirmP3AInfoBarDelegate::Create(InfoBarService* infobar_service,
                                            PrefService* local_state) {
  // Don't show infobar if:
  // - P3A is disabled
  // - notice has already been acknowledged
  if (local_state) {
    if (!local_state->GetBoolean(adrbrowsiel::kP3AEnabled) ||
        local_state->GetBoolean(adrbrowsiel::kP3ANoticeAcknowledged)) {
      local_state->SetBoolean(adrbrowsiel::kP3ANoticeAcknowledged, true);
      return;
    }
  }

  infobar_service->AddInfoBar(infobar_service->CreateConfirmInfoBar(
      std::unique_ptr<ConfirmInfoBarDelegate>(
          new adrbrowsielConfirmP3AInfoBarDelegate(local_state))));
}

adrbrowsielConfirmP3AInfoBarDelegate::adrbrowsielConfirmP3AInfoBarDelegate(
    PrefService* local_state)
    : ConfirmInfoBarDelegate(), local_state_(local_state) {}

adrbrowsielConfirmP3AInfoBarDelegate::~adrbrowsielConfirmP3AInfoBarDelegate() {}

infobars::InfoBarDelegate::InfoBarIdentifier
adrbrowsielConfirmP3AInfoBarDelegate::GetIdentifier() const {
  return adrbrowsiel_CONFIRM_P3A_INFOBAR_DELEGATE;
}

const gfx::VectorIcon& adrbrowsielConfirmP3AInfoBarDelegate::GetVectorIcon() const {
  return views::kInfoIcon;
}

bool adrbrowsielConfirmP3AInfoBarDelegate::ShouldExpire(
    const NavigationDetails& details) const {
  return false;
}

void adrbrowsielConfirmP3AInfoBarDelegate::InfoBarDismissed() {
  // Mark notice as acknowledged when infobar is dismissed
  if (local_state_) {
    local_state_->SetBoolean(adrbrowsiel::kP3ANoticeAcknowledged, true);
  }
}

std::u16string adrbrowsielConfirmP3AInfoBarDelegate::GetMessageText() const {
  return l10n_util::GetStringUTF16(IDS_adrbrowsiel_CONFIRM_P3A_INFO_BAR);
}

int adrbrowsielConfirmP3AInfoBarDelegate::GetButtons() const {
  return BUTTON_OK | BUTTON_CANCEL;
}

std::u16string adrbrowsielConfirmP3AInfoBarDelegate::GetButtonLabel(
    InfoBarButton button) const {
  if (button == BUTTON_CANCEL) {
    return l10n_util::GetStringUTF16(IDS_adrbrowsiel_CONFIRM_P3A_INFO_BAR_DISABLE);
  }
  return l10n_util::GetStringUTF16(IDS_adrbrowsiel_CONFIRM_P3A_INFO_BAR_ACKNOWLEDGE);
}

std::u16string adrbrowsielConfirmP3AInfoBarDelegate::GetLinkText() const {
  return l10n_util::GetStringUTF16(IDS_LEARN_MORE);
}

GURL adrbrowsielConfirmP3AInfoBarDelegate::GetLinkURL() const {
  return GURL(kP3ALearnMoreURL);
}

bool adrbrowsielConfirmP3AInfoBarDelegate::Accept() {
  // Mark notice as acknowledged when infobar is dismissed
  if (local_state_) {
    local_state_->SetBoolean(adrbrowsiel::kP3ANoticeAcknowledged, true);
  }
  return true;
}

bool adrbrowsielConfirmP3AInfoBarDelegate::Cancel() {
  // OK button is "Disable"
  // Clicking should disable P3A
  if (local_state_) {
    local_state_->SetBoolean(adrbrowsiel::kP3AEnabled, false);
  }
  return true;
}
