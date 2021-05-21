/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
 
(function() {
'use strict';

/**
 * 'settings-adrbrowsiel-default-extensions-page' is the settings page containing
 * adrbrowsiel's default extensions.
 */
Polymer({
  is: 'settings-adrbrowsiel-wallet-page',

  behaviors: [
    WebUIListenerBehavior,
  ],

  properties: {
    isNativeWalletEnabled_: Boolean
  },

  /** @private {?settings.adrbrowsielWalletBrowserProxy} */
  browserProxy_: null,

  /** @override */
  created: function() {
    this.browserProxy_ = settings.adrbrowsielWalletBrowserProxyImpl.getInstance();
  },

  /** @override */
  ready: function() {
    this.onadrbrowsielWalletEnabledChange_ = this.onadrbrowsielWalletEnabledChange_.bind(this)
    this.browserProxy_.getWeb3ProviderList().then(list => {
      this.adrbrowsielWeb3Providers_ = JSON.parse(list)
    });
    this.browserProxy_.isNativeWalletEnabled().then(val => {
      this.isNativeWalletEnabled_ = val;
    });
  },

  onadrbrowsielWalletEnabledChange_: function() {
    this.browserProxy_.setadrbrowsielWalletEnabled(this.$.adrbrowsielWalletEnabled.checked);
  },
});
})();
