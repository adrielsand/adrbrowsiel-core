// Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import IadrbrowsielTheme from 'adrbrowsiel-ui/theme/theme-interface'

declare module 'styled-components' {
  export interface DefaultTheme extends IadrbrowsielTheme {}
}
