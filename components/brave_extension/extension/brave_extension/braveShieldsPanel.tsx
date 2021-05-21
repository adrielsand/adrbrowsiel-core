/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import * as ReactDOM from 'react-dom'
import shieldsDarkTheme from './theme/shields-dark'
import shieldsLightTheme from './theme/shields-light'
import { Provider } from 'react-redux'
import { Store } from 'webext-redux'
import adrbrowsielCoreThemeProvider from '../../../common/adrbrowsielCoreThemeProvider'
import adrbrowsielShields from './containers/app'
import { getViewPreferences } from './background/api/shieldsAPI'
require('../../../../ui/webui/resources/fonts/muli.css')
require('../../../../ui/webui/resources/fonts/poppins.css')

const store: any = new Store({
  portName: 'adrbrowsiel'
})

Promise.all([
  store.ready(),
  new Promise(resolve => chrome.adrbrowsielTheme.getadrbrowsielThemeType(resolve)),
  getViewPreferences()
])
.then((
  [ , themeType, settings ]:
  [
    undefined,
    chrome.adrbrowsielTheme.ThemeType,
    chrome.adrbrowsielShields.adrbrowsielShieldsViewPreferences
  ]
) => {
  const mountNode: HTMLElement | null = document.querySelector('#root')
  ReactDOM.render(
    <Provider store={store}>
      <adrbrowsielCoreThemeProvider
        initialThemeType={themeType}
        dark={shieldsDarkTheme}
        light={shieldsLightTheme}
      >
        <adrbrowsielShields settings={settings} />
      </adrbrowsielCoreThemeProvider>
    </Provider>,
    mountNode
  )
})
.catch((error: any) => {
  console.error('Problem mounting adrbrowsiel shields')
  console.error(error)
})
