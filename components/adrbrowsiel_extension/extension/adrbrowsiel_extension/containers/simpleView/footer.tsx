/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

// Feature-specific components
import { MainFooter, Link } from '../../components'

// API
import * as tabsAPI from '../../background/api/tabsAPI'

// Locale
import { getLocale } from '../../background/api/localeAPI'

export interface Props {
  enabled: boolean
  toggleAdvancedView: () => void
}

export default class Footer extends React.PureComponent<Props, {}> {
  openSettings = () => {
    tabsAPI.createTab({ url: 'chrome://settings/shields' })
      .catch((err) => console.log('[Unable to open a new tab from Shields]', err))
  }

  render () {
    const { enabled, toggleAdvancedView } = this.props
    return (
      <MainFooter>
        {enabled &&
        <Link onClick={toggleAdvancedView}>
          {getLocale('advancedView')}
        </Link>}
        <Link id='adrbrowsielShieldsFooter' onClick={this.openSettings}>
          {getLocale('changeDefaults')}
        </Link>
      </MainFooter>
    )
  }
}
