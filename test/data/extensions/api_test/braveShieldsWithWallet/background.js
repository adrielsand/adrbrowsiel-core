/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

function testBasics() {
  chrome.test.runTests([
    function adrbrowsielShieldsHasWalletAccessButNotSeed() {
      if (chrome.adrbrowsielWallet && !chrome.adrbrowsielWallet.getWalletSeed &&
          !chrome.adrbrowsielWallet.getBitGoSeed &&
          !chrome.adrbrowsielWallet.getProjectID &&
          !chrome.adrbrowsielWallet.getadrbrowsielKey) {
        chrome.test.succeed();
      } else {
        chrome.test.fail();
      }
    },
  ]);
}

function testNoDappCheck() {
  chrome.test.runTests([
    function testNotENabled() {
      chrome.adrbrowsielWallet.shouldCheckForDapps((dappDetection) => {
        if (dappDetection) {
          chrome.test.fail();
        } else {
          chrome.test.succeed();
        }
      })
    },
  ]);
}

function testDappCheck() {
  chrome.test.runTests([
    function testNotENabled() {
      chrome.adrbrowsielWallet.shouldCheckForDapps((dappDetection) => {
        if (dappDetection) {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    },
  ]);
}
