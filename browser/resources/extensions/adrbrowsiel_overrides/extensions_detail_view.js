// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import {RegisterPolymerTemplateModifications, RegisterPolymerComponentBehaviors} from 'chrome://adrbrowsiel-resources/polymer_overriding.js'
import {I18nBehavior} from 'chrome://resources/js/i18n_behavior.m.js'

RegisterPolymerTemplateModifications({
  'extensions-detail-view': (templateContent) => {
    let optionsTemplate =
      templateContent.querySelector('template[is="dom-if"][if*="shouldShowOptionsSection_"]')
    if (!optionsTemplate) {
      console.error('[adrbrowsiel Extensions Overrides] Could not find optionsTemplate')
      return
    }
    let incognitoTemplate =
      optionsTemplate.content.querySelector('template[is="dom-if"][if*="shouldShowIncognitoOption_"]')
    if (!incognitoTemplate) {
      console.error('[adrbrowsiel Extensions Overrides] Could not find incognitoTemplate')
      return
    }
    let incognitoWarningDiv = incognitoTemplate.content.querySelector('.section-content')
    if (!incognitoWarningDiv) {
      console.error('[adrbrowsiel Extensions Overrides] Could not find incognitoWarningDiv')
      return
    }
    incognitoWarningDiv.innerText = I18nBehavior.i18n('privateInfoWarning')
    const spanningWarningSpan = document.createElement('span')
    spanningWarningSpan.setAttribute('class', 'section-content')
    spanningWarningSpan.setAttribute('hidden', '[[data.isSplitMode]]')
    spanningWarningSpan.innerText = ' ' +  I18nBehavior.i18n('spanningInfoWarning')
    const privateAndTorWarningSpan = document.createElement('span')
    privateAndTorWarningSpan.setAttribute('class', 'section-content')
    privateAndTorWarningSpan.innerText = ' ' + I18nBehavior.i18n('privateAndTorInfoWarning')
    incognitoWarningDiv.appendChild(spanningWarningSpan)
    incognitoWarningDiv.appendChild(privateAndTorWarningSpan)
  }
})
