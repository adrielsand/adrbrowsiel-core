/* global jest, expect, describe, it, afterEach */
import * as React from 'react'
import { shallow } from 'enzyme'
import { create } from 'react-test-renderer'
import GrantCaptcha from './index'
import { TestThemeProvider } from 'adrbrowsiel-ui/theme'

describe('Grant captcha tests', () => {
  const baseComponent = (props?: object) => <TestThemeProvider><GrantCaptcha id='captcha' {...props} /></TestThemeProvider>

  describe('basic tests', () => {
    it('matches the snapshot', () => {
      const component = baseComponent()
      const tree = create(component).toJSON()
      expect(tree).toMatchSnapshot()
    })

    it('renders the component', () => {
      const wrapper = shallow(baseComponent())
      const assertion = wrapper.find('#captcha').length
      expect(assertion).toBe(1)
    })
  })
})
