// Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import icons from './assets/icons'

type Props = {
  iconKey: string
  size?: number
}

export default function IconAsset (props: Props) {
  const { iconKey, size = 25 } = props

  return (
    <>
      <img width={size} src={icons[iconKey]} />
    </>
  )
}
