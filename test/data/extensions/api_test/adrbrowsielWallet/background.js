/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

function arrayBufsMatch(ab1, ab2) {
  const dataView1 = new DataView(ab1)
  const dataView2 = new DataView(ab2)
  console.log('dataView1 byte length: ', dataView1.byteLength)
  console.log('dataView2 byte length: ', dataView2.byteLength)
  if (dataView1.byteLength === 32 &&
      dataView1.byteLength === dataView2.byteLength) {
    for (let i = 0; i < dataView1.byteLength; i++) {
      if (dataView1.getUint8(i) !== dataView2.getUint8(i)) {
        console.log('Mismatch at position i: ', i)
        return false;
      }
    }
  }
  return true;
}

function testKnownSeedValuesEndToEnd() {
  chrome.test.runTests([
    function testKnownSeedValues() {
      const buf = new Uint8Array([
        196, 34, 104, 152, 91, 63, 78, 171,
        234, 163, 25, 221, 80, 73, 158, 89,
        52, 53, 227, 231, 152, 214, 61, 210,
        33, 54, 68, 171, 140, 239, 3, 158])
      const expectedResult = new Uint8Array([
        142, 147, 10, 180, 36, 89, 142, 110,
        52, 85, 216, 222, 83, 56, 38, 206,
        104, 133, 77, 246, 219, 90, 105, 35,
        52, 76, 223, 24, 183, 138, 244, 72])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      const expectedResultArrayBuf =
          expectedResult.buffer.slice(expectedResult.byteOffset,
            expectedResult.byteLength + expectedResult.byteOffset)
      chrome.adrbrowsielWallet.getWalletSeed(buf, (resultArrayBuf) => {
        if (arrayBufsMatch(resultArrayBuf, expectedResultArrayBuf)) {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testKnownBitGoSeedValuesEndToEnd() {
  chrome.test.runTests([
    function testKnownBitGoSeedValues() {
      const buf = new Uint8Array([
        196, 34, 104, 152, 91, 63, 78, 171,
        234, 163, 25, 221, 80, 73, 158, 89,
        52, 53, 227, 231, 152, 214, 61, 210,
        33, 54, 68, 171, 140, 239, 3, 158])
      const expectedResult = new Uint8Array([
        101, 6, 89, 61, 129, 81, 104, 13,
        48, 59, 117, 46, 73, 177, 168, 248,
        91, 84, 145, 54, 61, 157, 27, 254,
        45, 203, 71, 123, 188, 29, 224, 203])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      const expectedResultArrayBuf =
          expectedResult.buffer.slice(expectedResult.byteOffset,
            expectedResult.byteLength + expectedResult.byteOffset)
      chrome.adrbrowsielWallet.getBitGoSeed(buf, (resultArrayBuf) => {
        if (arrayBufsMatch(resultArrayBuf, expectedResultArrayBuf)) {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testProviderIsCryptoWallets() {
  chrome.test.runTests([
    function CryptoWalletsIsProvider() {
      chrome.adrbrowsielWallet.getWeb3Provider((provider) => {
        if (provider === 'odbfpeeihdkbihmopkbjmoonfanlbfcl') {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testProviderIsMetaMask() {
  chrome.test.runTests([
    function CryptoWalletsIsProvider() {
      chrome.adrbrowsielWallet.getWeb3Provider((provider) => {
        if (provider === 'nkbihfbeogaeaoehlefnkodbefgpgknn') {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testProviderIsAsk() {
  chrome.test.runTests([
    function CryptoWalletsIsProvider() {
      chrome.adrbrowsielWallet.getWeb3Provider((provider) => {
        if (provider.length === 0) {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testProviderIsNone() {
  chrome.test.runTests([
    function CryptoWalletsIsProvider() {
      chrome.adrbrowsielWallet.getWeb3Provider((provider) => {
        if (provider.length === 0) {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testProviderIsadrbrowsielWallet() {
  chrome.test.runTests([
    function CryptoWalletsIsProvider() {
      chrome.adrbrowsielWallet.getWeb3Provider((provider) => {
        if (provider === 'odbfpeeihdkbihmopkbjmoonfanlbfcl') {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testBasics() {
  chrome.test.runTests([
    function adrbrowsielWalletExtensionHasAccess() {
      if (chrome.adrbrowsielWallet &&
          chrome.adrbrowsielWallet.shouldPromptForSetup &&
          chrome.adrbrowsielWallet.loadUI &&
          chrome.adrbrowsielWallet.promptToEnableWallet) {
        chrome.test.succeed();
      } else {
        chrome.test.fail();
      }
    },
    function adrbrowsielWalletExtensionSeedFunctionHasAccess() {
      if (chrome.adrbrowsielWallet && chrome.adrbrowsielWallet.getWalletSeed &&
          chrome.adrbrowsielWallet.getBitGoSeed &&
          chrome.adrbrowsielWallet.getProjectID &&
          chrome.adrbrowsielWallet.getadrbrowsielKey &&
          chrome.adrbrowsielWallet.getWeb3Provider) {
        chrome.test.succeed();
      } else {
        chrome.test.fail();
      }
    },
    function adrbrowsielWalletWrongInputKeySizeFails() {
      const buf = new Uint8Array([...Array(3).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      chrome.adrbrowsielWallet.getWalletSeed(arraybuf, (seed) => {
        if (!seed) {
          chrome.test.succeed();
          return
        }
        chrome.test.fail();
      })
    },
    function adrbrowsielBitGoWrongInputKeySizeFails() {
      const buf = new Uint8Array([...Array(3).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      chrome.adrbrowsielWallet.getBitGoSeed(arraybuf, (seed) => {
        if (!seed) {
          chrome.test.succeed();
          return
        }
        chrome.test.fail();
      })
    },
    function adrbrowsielWalletExtensionGetWalletSeedSameSeedOnMultipleRequests() {
      const buf = new Uint8Array([...Array(32).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      chrome.adrbrowsielWallet.getWalletSeed(arraybuf, (firstSeed) => {
        chrome.adrbrowsielWallet.getWalletSeed(arraybuf, (secondSeed) => {
          if (arrayBufsMatch(firstSeed, secondSeed)) {
            chrome.test.succeed();
            return
          }
          console.error('Seeds differ across calls!')
          chrome.test.fail();
        })
      })
    },
    function adrbrowsielWalletExtensionGetBitGoSeedSameSeedOnMultipleRequests() {
      const buf = new Uint8Array([...Array(32).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      chrome.adrbrowsielWallet.getBitGoSeed(arraybuf, (firstSeed) => {
        chrome.adrbrowsielWallet.getBitGoSeed(arraybuf, (secondSeed) => {
          if (arrayBufsMatch(firstSeed, secondSeed)) {
            chrome.test.succeed();
            return
          }
          console.error('Seeds differ across calls!')
          chrome.test.fail();
        })
      })
    },
    function adrbrowsielWalletExtensionSeedsDiffValues() {
      const buf = new Uint8Array([...Array(32).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      chrome.adrbrowsielWallet.getWalletSeed(arraybuf, (firstSeed) => {
        chrome.adrbrowsielWallet.getBitGoSeed(arraybuf, (secondSeed) => {
          if (!arrayBufsMatch(firstSeed, secondSeed)) {
            chrome.test.succeed();
            return
          }
          console.error('Seeds are the same but should not be!')
          chrome.test.fail();
        })
      })
    },
    function adrbrowsielWalletExtensionGetWalletSeedDifferentKeysDoesNotWork() {
      const buf = new Uint8Array([...Array(32).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      const buf2 = new Uint8Array([11, ...Array(31).keys()])
      const arraybuf2 = buf2.buffer.slice(buf2.byteOffset, buf2.byteLength + buf2.byteOffset)
      chrome.adrbrowsielWallet.getWalletSeed(arraybuf, (firstSeed) => {
        chrome.adrbrowsielWallet.getWalletSeed(arraybuf2, (secondSeed) => {
          if (firstSeed && !secondSeed) {
            chrome.test.succeed();
            return
          }
          chrome.test.fail();
        })
      })
    },
    function adrbrowsielWalletExtensionGetBitGoSeedDifferentKeysDoesNotWork() {
      const buf = new Uint8Array([...Array(32).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      const buf2 = new Uint8Array([11, ...Array(31).keys()])
      const arraybuf2 = buf2.buffer.slice(buf2.byteOffset, buf2.byteLength + buf2.byteOffset)
      chrome.adrbrowsielWallet.getBitGoSeed(arraybuf, (firstSeed) => {
        chrome.adrbrowsielWallet.getBitGoSeed(arraybuf2, (secondSeed) => {
          if (firstSeed && !secondSeed) {
            chrome.test.succeed();
            return
          }
          chrome.test.fail();
        })
      })
    },
    function adrbrowsielWalletExtensionGetProjectIDWorks() {
      chrome.adrbrowsielWallet.getProjectID((projectID) => {
        if (projectID === "test-project-id") {
          chrome.test.succeed();
          return
        }
        console.log('Failed project ID is: ' + projectID)
        chrome.test.fail();
      })
    },
    function adrbrowsielWalletExtensionGetadrbrowsielKeyWorks() {
      chrome.adrbrowsielWallet.getadrbrowsielKey((key) => {
        if (key === "test-adrbrowsiel-key") {
          chrome.test.succeed();
          return
        }
        console.log('Failure, adrbrowsiel key is: ' + key)
        chrome.test.fail();
      })
    }
  ]);
}
