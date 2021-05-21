// Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { render } from 'react-dom'
import { Provider } from 'react-redux'
import Theme from 'adrbrowsiel-ui/theme/adrbrowsiel-default'
import DarkTheme from 'adrbrowsiel-ui/theme/adrbrowsiel-dark'
import '../common/defaultTrustedTypesPolicy'
import adrbrowsielCoreThemeProvider from '../common/adrbrowsielCoreThemeProvider'
import { wireApiEventsToStore } from './apiEventsToStore'
import * as topSitesAPI from './api/topSites'
import { init } from './actions/new_tab_actions'

// Components
import App from './containers/app'

// Utils
import store from './store'

// Let things handle 'init'
store.dispatch(init())

function initialize () {
  console.timeStamp('loaded')
  // Get rendering going
  new Promise(resolve => chrome.adrbrowsielTheme.getadrbrowsielThemeType(resolve))
  .then((themeType: chrome.adrbrowsielTheme.ThemeType) => {
    render(
      <Provider store={store}>
        <adrbrowsielCoreThemeProvider
          initialThemeType={themeType}
          dark={DarkTheme}
          light={Theme}
        >
          <App />
        </adrbrowsielCoreThemeProvider>
      </Provider>,
      document.getElementById('root'),
      () => console.timeStamp('first react render'))
  })
  .catch((error) => {
    console.error('Problem mounting adrbrowsiel new tab', error)
  })
  window.i18nTemplate.process(window.document, window.loadTimeData)
}

console.timeStamp('JS start')

// Get store data going
wireApiEventsToStore()

// Perform DOM-dependent initialization when ready
document.addEventListener('DOMContentLoaded', initialize)

// Update topsite tiles when NTP gets visible.
document.addEventListener('visibilitychange', () => {
  if (document.visibilityState === 'visible') {
    topSitesAPI.updateMostVisitedInfo()
  }
})
