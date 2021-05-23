import { addons } from '@storybook/addons'
import { create } from '@storybook/theming'

const adrbrowsielTheme = create({
  base: 'dark',
  brandTitle: 'adrbrowsiel Browser UI',
  brandUrl: 'https://github.com/adrielsand/adrbrowsiel-core'
})

addons.setConfig({
  isFullscreen: false,
  showNav: true,
  showPanel: true,
  panelPosition: 'right',
  theme: adrbrowsielTheme
})
