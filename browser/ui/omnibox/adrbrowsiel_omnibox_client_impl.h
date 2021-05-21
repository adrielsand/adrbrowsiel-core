/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_OMNIBOX_adrbrowsiel_OMNIBOX_CLIENT_IMPL_H_
#define adrbrowsiel_BROWSER_UI_OMNIBOX_adrbrowsiel_OMNIBOX_CLIENT_IMPL_H_

#include "adrbrowsiel/browser/autocomplete/adrbrowsiel_autocomplete_scheme_classifier.h"
#include "chrome/browser/ui/omnibox/chrome_omnibox_client.h"

class OmniboxEditController;
class PrefRegistrySimple;
class Profile;

class adrbrowsielOmniboxClientImpl : public ChromeOmniboxClient {
 public:
  adrbrowsielOmniboxClientImpl(OmniboxEditController* controller, Profile* profile);
  ~adrbrowsielOmniboxClientImpl() override;

  static void RegisterPrefs(PrefRegistrySimple* prefs);

  const AutocompleteSchemeClassifier& GetSchemeClassifier() const override;
  bool IsAutocompleteEnabled() const override;

  void OnInputAccepted(const AutocompleteMatch& match) override;

 private:
  Profile* profile_;
  adrbrowsielAutocompleteSchemeClassifier scheme_classifier_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielOmniboxClientImpl);
};

#endif  // adrbrowsiel_BROWSER_UI_OMNIBOX_adrbrowsiel_OMNIBOX_CLIENT_IMPL_H_
