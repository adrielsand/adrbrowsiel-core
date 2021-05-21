/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import {addSingletonGetter, sendWithPromise} from 'chrome://resources/js/cr.m.js';

/** @interface */
export class adrbrowsielAppearanceBrowserProxy {
  /**
   * @return {!Promise<Array>}
   */
  getadrbrowsielThemeList() {}
  /**
   * @return {!Promise<Number>}
   */
  getadrbrowsielThemeType() {}
  /**
   * @param {Number} type
   */
  setadrbrowsielThemeType(value) {}
}

/**
 * @implements {adrbrowsielAppearanceBrowserProxy}
 */
export class adrbrowsielAppearanceBrowserProxyImpl {
  /** @override */
  getadrbrowsielThemeList() {
    return new Promise(resolve => chrome.adrbrowsielTheme.getadrbrowsielThemeList(resolve))
  }
  getadrbrowsielThemeType() {
    return sendWithPromise('getadrbrowsielThemeType');
  }
  setadrbrowsielThemeType(value) {
    chrome.send('setadrbrowsielThemeType', [value]);
  }
}

addSingletonGetter(adrbrowsielAppearanceBrowserProxyImpl);
