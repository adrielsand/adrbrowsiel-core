import * as React from 'react'

import {
  StyledWrapper,
  Title,
  Input,
  Column,
  PageIcon,
  IconBackground
} from './style'
import { NavButton } from '../'
import locale from '../../../constants/locale'

export interface Props {
  onSubmit: () => void
  onPasswordChanged: (value: string) => void
  disabled: boolean
}

function LockPanel (props: Props) {
  const { onSubmit, onPasswordChanged, disabled } = props

  const inputPassword = (event: React.ChangeEvent<HTMLInputElement>) => {
    onPasswordChanged(event.target.value)
  }

  return (
    <StyledWrapper>
      <IconBackground>
        <PageIcon />
      </IconBackground>
      <Title>{locale.lockScreenTitle}</Title>
      <Column>
        <Input type='password' placeholder={locale.createPasswordInput} onChange={inputPassword} />
      </Column>
      <NavButton buttonType='primary' text={locale.lockScreenButton} onSubmit={onSubmit} disabled={disabled} />
    </StyledWrapper>
  )
}

export default LockPanel
