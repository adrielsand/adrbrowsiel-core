/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// clang-format off
// #import {addSingletonGetter, sendWithPromise} from 'chrome://resources/js/cr.m.js';
// clang-format on

cr.define('settings', function () {
  /** @interface */
  /* #export */ class adrbrowsielDefaultExtensionsBrowserProxy {
    /**
     * @param {boolean} value name.
     */
    setWebTorrentEnabled (value) {}
    setHangoutsEnabled (value) {}
    setTorEnabled (value) {}
    isTorEnabled () {}
    isTorManaged () {}
    setWidevineEnabled() {}
    isWidevineEnabled() {}
    getRestartNeeded () {}
    wasSignInEnabledAtStartup () {}
    isDecentralizedDnsEnabled() {}
    getDecentralizedDnsResolveMethodList(provider) {}
  }

  /**
   * @implements {settings.adrbrowsielDefaultExtensionsBrowserProxy}
   */
  /* #export */ class adrbrowsielDefaultExtensionsBrowserProxyImpl {
    /** @override */
    setWebTorrentEnabled (value) {
      chrome.send('setWebTorrentEnabled', [value])
    }

    setHangoutsEnabled (value) {
      chrome.send('setHangoutsEnabled', [value])
    }

    setMediaRouterEnabled (value) {
      chrome.send('setMediaRouterEnabled', [value])
    }

    setTorEnabled (value) {
      chrome.send('setTorEnabled', [value])
    }

    isTorEnabled () {
      return cr.sendWithPromise('isTorEnabled')
    }

    isTorManaged () {
      return cr.sendWithPromise('isTorManaged')
    }

    setWidevineEnabled (value) {
      chrome.send('setWidevineEnabled', [value])
    }

    isWidevineEnabled () {
      return cr.sendWithPromise('isWidevineEnabled')
    }

    getRestartNeeded () {
      return cr.sendWithPromise('getRestartNeeded')
    }

    wasSignInEnabledAtStartup () {
      return loadTimeData.getBoolean('signInAllowedOnNextStartupInitialValue')
    }

    isDecentralizedDnsEnabled () {
      return cr.sendWithPromise('isDecentralizedDnsEnabled')
    }

    getDecentralizedDnsResolveMethodList (provider) {
      return cr.sendWithPromise('getDecentralizedDnsResolveMethodList', provider)
    }
  }

  cr.addSingletonGetter(adrbrowsielDefaultExtensionsBrowserProxyImpl)

  // #cr_define_end
  return {
    adrbrowsielDefaultExtensionsBrowserProxy,
    adrbrowsielDefaultExtensionsBrowserProxyImpl
  }
})
