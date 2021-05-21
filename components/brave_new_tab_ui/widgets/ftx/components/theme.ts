import IadrbrowsielTheme from 'adrbrowsiel-ui/theme/theme-interface'

export default function customizeTheme (theme: IadrbrowsielTheme): IadrbrowsielTheme {
  return {
    ...theme,
    color: {
      ...theme.color,
      primaryBackground: 'rgba(2, 166, 194, 1)',
      secondaryBackground: 'rgba(33, 46, 60, 1)'
    }
  }
}
