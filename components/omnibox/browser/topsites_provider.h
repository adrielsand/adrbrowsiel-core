/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_OMNIBOX_BROWSER_TOPSITES_PROVIDER_H_
#define adrbrowsiel_COMPONENTS_OMNIBOX_BROWSER_TOPSITES_PROVIDER_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "components/omnibox/browser/autocomplete_match.h"
#include "components/omnibox/browser/autocomplete_provider.h"

class AutocompleteProviderClient;

// This is the provider for top Alexa 500 sites URLs
class TopSitesProvider : public AutocompleteProvider {
 public:
  explicit TopSitesProvider(AutocompleteProviderClient* client);

  // AutocompleteProvider:
  void Start(const AutocompleteInput& input, bool minimal_changes) override;

 private:
  ~TopSitesProvider() override;

  static const int kRelevance;

  static std::vector<std::string> top_sites_;

  void AddMatch(const std::u16string& match_string,
                const ACMatchClassifications& styles);

  static ACMatchClassifications StylesForSingleMatch(
      const std::string &input_text,
      const std::string &site,
      const size_t &foundPos);

  AutocompleteProviderClient* client_;
  DISALLOW_COPY_AND_ASSIGN(TopSitesProvider);
};

#endif  // adrbrowsiel_COMPONENTS_OMNIBOX_BROWSER_TOPSITES_PROVIDER_H_
