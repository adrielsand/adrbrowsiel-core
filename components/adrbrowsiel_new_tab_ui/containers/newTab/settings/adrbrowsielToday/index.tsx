// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { Button } from 'adrbrowsiel-ui'
import { getLocale } from '../../../../../common/locale'
import {
  SettingsRow,
  SettingsText
} from '../../../../components/default'
import { Toggle } from '../../../../components/toggle'
import * as Styled from './style'
import Sources from './sources'

export interface Props {
  publishers?: adrbrowsielToday.Publishers
  setPublisherPref: (publisherId: string, enabled: boolean) => any
  onDisplay: () => any
  onClearPrefs: () => any
  showToday: boolean
  toggleShowToday: () => any
}

export default function adrbrowsielTodayPrefs (props: Props) {
  // Ensure publishers data is fetched, which won't happen
  // if user has not interacted with adrbrowsiel Today on this page
  // view.
  React.useEffect(() => {
    if (props.showToday) {
      props.onDisplay()
    }
  }, [props.onDisplay, props.showToday])

  const [category, setCategory] = React.useState<string>('')

  const confirmAction = React.useCallback(() => {
    if (confirm(getLocale('adrbrowsielTodayResetConfirm'))) {
      props.onClearPrefs()
    }
  }, [props.onClearPrefs])

  return (
    <Styled.Section>
      {!category && (
        <SettingsRow>
          <SettingsText>{getLocale('adrbrowsielTodayOptInActionLabel')}</SettingsText>
          <Toggle
            checked={props.showToday}
            onChange={props.toggleShowToday}
            size='large'
          />
        </SettingsRow>
      )}
      {props.showToday && props.publishers && Object.keys(props.publishers).length &&
      <Sources category={category} setCategory={setCategory} {...props} />
      }
      {!category && (
      <SettingsRow>
        <Button type='warn' level='tertiary' onClick={confirmAction} text={getLocale('adrbrowsielTodayResetAction')} />
      </SettingsRow>
      )}
    </Styled.Section>
  )
}
