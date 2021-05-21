// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { getLocale, getLocaleWithTag } from '../../../../../common/locale'
import * as Card from '../cardIntro'
import adrbrowsielTodayLogoUrl from '../adrbrowsielTodayLogo.svg'
import { CardButton, TertiaryButton } from '../default'

type Props = {
  onOptIn: () => unknown
  onDisable: () => unknown
}

export default function IntroCard (props: Props) {
  const introElementRef = React.useRef(null)
  const descriptionTextParts = React.useMemo(() => {
    return getLocaleWithTag('adrbrowsielTodayIntroDescription')
  }, [])
  return (
    <Card.Intro ref={introElementRef}>
      <Card.Image src={adrbrowsielTodayLogoUrl} />
      <Card.Title>{getLocale('adrbrowsielTodayIntroTitle')}</Card.Title>
      <Card.Paragraph>
        {descriptionTextParts.beforeTag}
        <a href={'https://adrbrowsiel.com/privacy/browser/'}>
          {descriptionTextParts.duringTag}
        </a>
        {descriptionTextParts.afterTag}
      </Card.Paragraph>
      <CardButton onClick={props.onOptIn} isMainFocus={true}>
        {getLocale('adrbrowsielTodayOptInActionLabel')}
      </CardButton>
      <TertiaryButton onClick={props.onDisable}>
        {getLocale('adrbrowsielTodayOptOutActionLabel')}
      </TertiaryButton>
    </Card.Intro>
  )
}
