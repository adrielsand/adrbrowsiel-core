// Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import {RegisterPolymerTemplateModifications} from 'chrome://adrbrowsiel-resources/polymer_overriding.js'

import '../adrbrowsiel_privacy_page/adrbrowsiel_personalization_options.m.js'

RegisterPolymerTemplateModifications({
  'settings-personalization-options': (templateContent) => {
    const searchSuggestToggle = templateContent.getElementById('searchSuggestToggle')
    if (!searchSuggestToggle) {
      console.error(`[adrbrowsiel Settings Overrides] Couldn't find search suggest toggle`)
    } else {
      searchSuggestToggle.insertAdjacentHTML('afterend', `
        <settings-adrbrowsiel-personalization-options prefs="{{prefs}}"></settings-adrbrowsiel-personalization-options>
      `)
    }
  },
})
