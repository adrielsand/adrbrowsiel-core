/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { BatColorIcon } from 'adrbrowsiel-ui/components/icons'

import { PaymentKind } from '../lib/interfaces'

import { TokenAmount } from './token_amount'
import { ButtonSwitch, ButtonSwitchOption } from './button_switch'
import { BatString } from './bat_string'

import * as style from './payment_kind_switch.style'

interface Props {
  userBalance: number
  currentValue: PaymentKind
  onChange: (paymentKind: PaymentKind) => void
}

export function PaymentKindSwitch (props: Props) {
  const options: ButtonSwitchOption<PaymentKind>[] = [
    {
      value: 'bat',
      content: (
        <>
          <style.icon><BatColorIcon /></style.icon>&nbsp;
          <BatString stringKey='batFunds' />
        </>
      ),
      caption: <TokenAmount amount={props.userBalance} />
    }
  ]

  return (
    <style.root>
      <ButtonSwitch<PaymentKind>
        options={options}
        selectedValue={props.currentValue}
        onSelect={props.onChange}
      />
    </style.root>
  )
}
