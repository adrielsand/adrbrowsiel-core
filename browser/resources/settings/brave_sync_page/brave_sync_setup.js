// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

/**
 * @fileoverview
 * 'adrbrowsiel-sync-setup' is the UI for starting or joining a sync chain
 * settings.
 */
import './adrbrowsiel_sync_code_dialog.js';

import {Polymer, html} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js';

import {adrbrowsielSyncBrowserProxy} from './adrbrowsiel_sync_browser_proxy.js';

Polymer({
  is: 'settings-adrbrowsiel-sync-setup',

  _template: html`{__html_template__}`,

  properties: {
    syncCode: {
      type: String,
      notify: true
    },
     /**
     * Sync code dialog type. Can only have 1 at a time, so use a single property.
     * 'qr' | 'words' | 'input' | 'choose' | null
     * @private
     */
    syncCodeDialogType_: String,
    isSubmittingSyncCode_: {
      type: Boolean,
      value: false,
    },
    isGettingSyncCode_: {
      type: Boolean,
      value: false,
    },
    isInvalidSyncCode_: {
      type: Boolean,
      value: false,
    }
  },

  /** @private {?adrbrowsielSyncBrowserProxy} */
  syncBrowserProxy_: null,

  created: function() {
    this.syncBrowserProxy_ = adrbrowsielSyncBrowserProxy.getInstance();
  },

  handleStartSyncChain_: async function () {
    this.isGettingSyncCode_ = true
    const syncCode = await this.syncBrowserProxy_.getSyncCode()
    this.isGettingSyncCode_ = false
    this.syncCode = syncCode;
    this.syncCodeDialogType_ = 'choose'
  },

  handleJoinSyncChain_: function () {
    this.syncCode = undefined
    this.syncCodeDialogType_ = 'input'
  },

  handleSyncCodeDialogDone_: function (e) {
    this.submitSyncCode_()
  },

  submitSyncCode_: async function () {
    this.isSubmittingSyncCode_ = true
    const syncCodeToSubmit = this.syncCode || ''
    let success = false
    try {
      success = await this.syncBrowserProxy_.setSyncCode(syncCodeToSubmit)
    } catch (e) {
      console.error("Error setting sync code")
      success = false
    }
    this.isSubmittingSyncCode_ = false
    if (!success) {
      this.isInvalidSyncCode_ = true
    } else {
      this.syncCodeDialogType_ = undefined
      this.fire('setup-success')
    }
  },
});
