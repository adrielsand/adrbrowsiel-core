import styled, * as Styled from 'styled-components'
import IadrbrowsielTheme from 'adrbrowsiel-ui/theme/theme-interface'
import IWelcomeTheme from './welcome-theme'

type Theme = IadrbrowsielTheme & IWelcomeTheme

export default styled as unknown as Styled.ThemedStyledInterface<Theme>
export const css = Styled.css as Styled.ThemedCssFunction<Theme>
