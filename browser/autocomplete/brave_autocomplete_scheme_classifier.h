/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_AUTOCOMPLETE_adrbrowsiel_AUTOCOMPLETE_SCHEME_CLASSIFIER_H_
#define adrbrowsiel_BROWSER_AUTOCOMPLETE_adrbrowsiel_AUTOCOMPLETE_SCHEME_CLASSIFIER_H_

#include <string>

#include "adrbrowsiel/components/adrbrowsiel_webtorrent/browser/buildflags/buildflags.h"
#include "chrome/browser/autocomplete/chrome_autocomplete_scheme_classifier.h"

class adrbrowsielAutocompleteSchemeClassifier
    : public ChromeAutocompleteSchemeClassifier {
 public:
  explicit adrbrowsielAutocompleteSchemeClassifier(Profile* profile);
  ~adrbrowsielAutocompleteSchemeClassifier() override;

  metrics::OmniboxInputType GetInputTypeForScheme(
      const std::string& scheme) const override;

 private:
#if BUILDFLAG(ENABLE_adrbrowsiel_WEBTORRENT)
  Profile* profile_ = nullptr;
#endif

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielAutocompleteSchemeClassifier);
};

#endif  // adrbrowsiel_BROWSER_AUTOCOMPLETE_adrbrowsiel_AUTOCOMPLETE_SCHEME_CLASSIFIER_H_

