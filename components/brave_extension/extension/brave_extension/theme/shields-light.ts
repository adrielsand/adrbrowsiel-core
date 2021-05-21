/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import ITheme from 'adrbrowsiel-ui/theme/theme-interface'
import defaultTheme from 'adrbrowsiel-ui/theme/adrbrowsiel-default'
import colors from 'adrbrowsiel-ui/theme/colors'

const shieldsLightTheme: ITheme = {
  ...defaultTheme,
  name: 'Shields Light',
  color: {
    ...defaultTheme.color,
    text: colors.neutral900,
    panelBackgroundSecondary: colors.neutral000,
    modalOverlayBackground: 'rgba(255, 255, 255, 70%)'
  }
}

export default shieldsLightTheme
