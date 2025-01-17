/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import {
  StyledSelect
} from './style'
import { getLocale } from 'adrbrowsiel-ui/helpers'

export type Amount = {
  value: string
  dataValue: string
  converted: string
}

export type Option = {
  value: string
  text: string
}

export interface Props {
  value?: string
  options?: Option[]
  floating?: boolean
  amountOptions?: Amount[]
  onlyAnonWallet?: boolean
  onChange?: (value: string) => void
}

export default class SelectMobile extends React.PureComponent<Props, {}> {

  onChange = (event: React.ChangeEvent<HTMLSelectElement>) => {
    if (this.props.onChange) {
      this.props.onChange(event.target.value)
    }
  }

  generateOptions = (options?: Option[]) => {
    if (!options) {
      return null
    }

    return (
      <>
        {options.map((option: Option) => {
          return (
            <option
              value={option.value}
              key={`k-${option.value}`}
            >
              {option.text}
            </option>
          )
        })}
      </>
    )
  }

  generateAmountOptions = (amountOptions?: Amount[]) => {
    const { onlyAnonWallet } = this.props

    if (!amountOptions) {
      return null
    }

    const batFormatString = onlyAnonWallet ? getLocale('batPoints') : getLocale('bat')

    return (
      <>
        {amountOptions.map((amount: Amount) => {
          return (
            <option
              key={`k-${amount.value}`}
              value={amount.dataValue}
            >
              {amount.value} {batFormatString} ({amount.converted} USD)
            </option>
          )
        })}
      </>
    )
  }

  render () {
    const { options, floating, amountOptions, value } = this.props

    return (
      <StyledSelect value={value} onChange={this.onChange} floating={floating}>
        {
          amountOptions
          ? this.generateAmountOptions(amountOptions)
          : this.generateOptions(options)
        }
      </StyledSelect>
    )
  }
}
