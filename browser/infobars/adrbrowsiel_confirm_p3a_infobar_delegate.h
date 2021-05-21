/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_INFOBARS_adrbrowsiel_CONFIRM_P3A_INFOBAR_DELEGATE_H_
#define adrbrowsiel_BROWSER_INFOBARS_adrbrowsiel_CONFIRM_P3A_INFOBAR_DELEGATE_H_

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "components/infobars/core/confirm_infobar_delegate.h"
#include "url/gurl.h"

class InfoBarService;
class PrefService;

// An infobar that is run with a string, buttons, and a "Learn More" link.
class adrbrowsielConfirmP3AInfoBarDelegate : public ConfirmInfoBarDelegate {
 public:
  static void Create(InfoBarService* infobar_service, PrefService* local_state);

 private:
  explicit adrbrowsielConfirmP3AInfoBarDelegate(PrefService* local_state);
  ~adrbrowsielConfirmP3AInfoBarDelegate() override;

  infobars::InfoBarDelegate::InfoBarIdentifier GetIdentifier() const override;
  const gfx::VectorIcon& GetVectorIcon() const override;
  bool ShouldExpire(const NavigationDetails& details) const override;
  void InfoBarDismissed() override;
  std::u16string GetMessageText() const override;
  int GetButtons() const override;
  std::u16string GetButtonLabel(InfoBarButton button) const override;
  std::u16string GetLinkText() const override;
  GURL GetLinkURL() const override;
  bool Accept() override;
  bool Cancel() override;

  PrefService* local_state_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielConfirmP3AInfoBarDelegate);
};

#endif  // adrbrowsiel_BROWSER_INFOBARS_adrbrowsiel_CONFIRM_P3A_INFOBAR_DELEGATE_H_
