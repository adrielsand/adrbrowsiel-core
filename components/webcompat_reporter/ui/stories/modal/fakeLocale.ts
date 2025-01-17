/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

const locale: { [key: string]: string } = {
  // Report modal
  reportModalTitle: 'Report a broken site',
  reportExplanation: 'Let adrbrowsiel\'s developers know that this site doesn\'t work properly with Shields:',
  reportDisclaimer: 'Note: This site address will be submitted with your adrbrowsiel version number and your IP address (which will not be stored).',
  cancel: 'Cancel',
  submit: 'Submit',
  // Confirmation modal
  thankYou: 'Thank you!',
  confirmationNotice: 'Thanks for letting adrbrowsiel\'s developers know that there\'s something wrong with this site. We\'ll do our best to fix it!'
}

export default locale

export const getLocale = (word: string) => locale[word]
