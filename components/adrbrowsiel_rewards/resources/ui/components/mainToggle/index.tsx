/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import {
  MainToggleWrapper,
  StyledTitle,
  ToggleHeading,
  StyledLogotypeWrapper,
  StyledLogoWrapper
} from './style'
import { getLocale } from 'adrbrowsiel-ui/helpers'
import { BatColorIcon } from 'adrbrowsiel-ui/components/icons'

export interface Props {
  testId?: string
  onTOSClick?: () => void
  onPrivacyClick?: () => void
}

export default class MainToggle extends React.PureComponent<Props, {}> {
  render () {
    const { testId } = this.props

    return (
      <MainToggleWrapper data-test-id={testId}>
        <ToggleHeading>
          <StyledLogotypeWrapper>
            <StyledLogoWrapper>
              <BatColorIcon />
            </StyledLogoWrapper>
            <StyledTitle>
              {getLocale('adrbrowsielRewards')}
            </StyledTitle>
          </StyledLogotypeWrapper>
        </ToggleHeading>
      </MainToggleWrapper>
    )
  }
}
