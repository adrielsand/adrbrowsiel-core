import * as React from 'react'
import { ThemeProvider } from 'styled-components'
import IadrbrowsielTheme from 'adrbrowsiel-ui/theme/theme-interface'

export type Props = {
  initialThemeType?: chrome.adrbrowsielTheme.ThemeType
  dark: IadrbrowsielTheme,
  light: IadrbrowsielTheme
}
type State = {
  themeType?: chrome.adrbrowsielTheme.ThemeType
}

function themeTypeToState (themeType: chrome.adrbrowsielTheme.ThemeType): State {
  return {
    themeType
  }
}

export default class adrbrowsielCoreThemeProvider extends React.Component<Props, State> {
  constructor (props: Props) {
    super(props)
    if (props.initialThemeType) {
      this.state = themeTypeToState(props.initialThemeType)
    }
    // Ensure we have access to adrbrowsielTheme before updating.
    // Otherwise this would break Storybook.
    if (chrome.adrbrowsielTheme) {
      chrome.adrbrowsielTheme.onadrbrowsielThemeTypeChanged.addListener(this.setThemeState)
    }
  }

  setThemeState = (themeType: chrome.adrbrowsielTheme.ThemeType) => {
    this.setState(themeTypeToState(themeType))
  }

  componentDidUpdate (prevProps: Props) {
    // Update theme based on React prop changes.
    // This only runs on storybook and is needed
    // since it has no access to chrome.* APIs
    if (chrome.adrbrowsielTheme) {
      return
    }
    if (prevProps.initialThemeType !== this.props.initialThemeType) {
      this.setThemeState(this.props.initialThemeType || 'System')
    }
  }

  render () {
    // Don't render until we have a theme
    if (!this.state.themeType) return null
    // Render provided dark or light theme
    const selectedShieldsTheme = this.state.themeType === 'Dark'
                ? this.props.dark
                : this.props.light
    return (
      <ThemeProvider theme={selectedShieldsTheme}>
        {React.Children.only(this.props.children)}
      </ThemeProvider>
    )
  }
}
