// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import {RegisterPolymerTemplateModifications} from 'chrome://adrbrowsiel-resources/polymer_overriding.js'

RegisterPolymerTemplateModifications({
  'settings-printing-page': (templateContent) => {
    const link = templateContent.getElementById('cloudPrinters')
    if (!link) {
      console.error('[adrbrowsiel Settings Overrides] Could not find cloudPrinters id on printing page.')
    } else {
      link.setAttribute('hidden', 'true')
    }
  }
})
