/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import { ComponentType } from 'react'
import styled from 'styled-components'
import DefaultModal, { Props } from 'adrbrowsiel-ui/components/popupModals/modal'
import Heading from 'adrbrowsiel-ui/components/text/heading'

export const Modal = styled(DefaultModal as ComponentType<Props>)`
  height: 100%;
`

export const LimitedBounds = styled('article')<{}>`
  overflow: auto;
  height: 500px;
  padding: 20px;
`

export const HeadingText = styled(Heading)`
  font-size: 20px;
  font-weight: 500;
`

export const Paragraph = styled('p')<{}>`
  font-size: 15px;
  font-family: ${p => p.theme.fontFamily.body};
  letter-spacing: .3px;
  line-height: 24px;
`

export const Footer = styled('footer')<{}>`
  display: flex;
  justify-content: flex-end;
`
