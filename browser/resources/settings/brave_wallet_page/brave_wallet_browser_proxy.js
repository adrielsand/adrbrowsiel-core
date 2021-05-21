/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import {addSingletonGetter} from 'chrome://resources/js/cr.m.js';

/** @interface */
export class adrbrowsielWalletBrowserProxy {
  /**
   * @param {boolean} value name.
   */
   setadrbrowsielWalletEnabled (value) {}
   getWeb3ProviderList () {}
   isNativeWalletEnabled() {}
}

/**
 * @implements {settings.adrbrowsielWalletBrowserProxy}
 */
export class adrbrowsielWalletBrowserProxyImpl {
  /** @override */
  setadrbrowsielWalletEnabled (value) {
    chrome.send('setadrbrowsielWalletEnabled', [value])
  }

  /** @override */
  getWeb3ProviderList () {
    return new Promise(resolve => chrome.adrbrowsielWallet.getWeb3ProviderList(resolve))
  }

  /** @override */
  isNativeWalletEnabled () {
    return new Promise(resolve => chrome.adrbrowsielWallet.isNativeWalletEnabled(resolve))
  }
}

addSingletonGetter(adrbrowsielWalletBrowserProxyImpl)
