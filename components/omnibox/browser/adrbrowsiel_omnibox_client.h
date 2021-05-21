/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_OMNIBOX_BROWSER_adrbrowsiel_OMNIBOX_CLIENT_H_
#define adrbrowsiel_COMPONENTS_OMNIBOX_BROWSER_adrbrowsiel_OMNIBOX_CLIENT_H_

#include "components/omnibox/browser/omnibox_client.h"

class adrbrowsielOmniboxClient : public OmniboxClient {
 public:
  virtual bool IsAutocompleteEnabled() const;

 protected:
  ~adrbrowsielOmniboxClient() override {}
};

#endif  // adrbrowsiel_COMPONENTS_OMNIBOX_BROWSER_adrbrowsiel_OMNIBOX_CLIENT_H_
