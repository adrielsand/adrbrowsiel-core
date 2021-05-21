// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { render } from 'react-dom'
import { Provider } from 'react-redux'
import { initLocale } from 'adrbrowsiel-ui'

import Container from './container'
import * as WalletPageActions from './actions/wallet_page_actions'
import store from './store'

import 'emptykit.css'
import '../../../ui/webui/resources/fonts/poppins.css'
import '../../../ui/webui/resources/fonts/muli.css'

import LegacyApp from '../components/legacy_app'
import Theme from 'adrbrowsiel-ui/theme/adrbrowsiel-default'
import DarkTheme from 'adrbrowsiel-ui/theme/adrbrowsiel-dark'
import adrbrowsielCoreThemeProvider from '../../common/adrbrowsielCoreThemeProvider'
import walletDarkTheme from '../theme/wallet-dark'
import walletLightTheme from '../theme/wallet-light'

function App () {
  const [initialThemeType, setInitialThemeType] = React.useState<chrome.adrbrowsielTheme.ThemeType>()
  React.useEffect(() => {
    chrome.adrbrowsielTheme.getadrbrowsielThemeType(setInitialThemeType)
  }, [])
  return (
    <Provider store={store}>
      {initialThemeType &&
      <adrbrowsielCoreThemeProvider
        initialThemeType={initialThemeType}
        dark={walletDarkTheme}
        light={walletLightTheme}
      >
        <Container />
      </adrbrowsielCoreThemeProvider>
      }
    </Provider>
  )
}

function initialize () {
  chrome.adrbrowsielWallet.isNativeWalletEnabled((enabled: boolean) => {
    if (enabled) {
      store.dispatch(WalletPageActions.initialize())
      render(<App />, document.getElementById('root'))
    } else {
      initializeOldWallet()
    }
  })
}

function initializeOldWallet () {
  chrome.adrbrowsielWallet.shouldPromptForSetup((prompt: boolean) => {
    if (!prompt) {
      chrome.adrbrowsielWallet.loadUI(() => {
        window.location.href = 'chrome://wallet'
      })
      return
    }
    renderOldWebUIView()
  })
}

function renderOldWebUIView () {
  new Promise(resolve => chrome.adrbrowsielTheme.getadrbrowsielThemeType(resolve))
  .then((themeType: chrome.adrbrowsielTheme.ThemeType) => {
    window.i18nTemplate.process(window.document, window.loadTimeData)
    if (window.loadTimeData && window.loadTimeData.data_) {
      initLocale(window.loadTimeData.data_)
    }

    render(
      <adrbrowsielCoreThemeProvider
        initialThemeType={themeType}
        dark={DarkTheme}
        light={Theme}
      >
        <LegacyApp />
      </adrbrowsielCoreThemeProvider>,
      document.getElementById('root')
    )
  })
  .catch(({ message }) => {
    console.error(`Could not mount adrbrowsiel wallet: ${message}`)
  })
}

document.addEventListener('DOMContentLoaded', initialize)
