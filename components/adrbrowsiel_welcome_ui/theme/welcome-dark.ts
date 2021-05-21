/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import ITheme from 'adrbrowsiel-ui/theme/theme-interface'
import IThemeWelcomePage from './welcome-theme'
import defaultTheme from 'adrbrowsiel-ui/theme/adrbrowsiel-default'
import colors from 'adrbrowsiel-ui/theme/colors'

const welcomeDarkTheme: ITheme & IThemeWelcomePage = {
  ...defaultTheme,
  name: 'Welcome Dark',
  color: {
    ...defaultTheme.color,
    text: colors.white,
    panelBackground: colors.grey900,
    outlineColor: 'rgba(255,255,255,0.5)'
  }
}

export default welcomeDarkTheme
