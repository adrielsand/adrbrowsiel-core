/* global jest, expect, describe, it, afterEach */
import * as React from 'react'
import { shallow } from 'enzyme'
import { create } from 'react-test-renderer'
import InfoCards from './index'
import { TestThemeProvider } from 'adrbrowsiel-ui/theme'

describe('Info Cards tests', () => {
  const baseComponent = (props?: object) => <TestThemeProvider><InfoCards id='info-cards' {...props} /></TestThemeProvider>

  describe('basic tests', () => {
    it('matches the snapshot', () => {
      const component = baseComponent()
      const tree = create(component).toJSON()
      expect(tree).toMatchSnapshot()
    })

    it('renders the component', () => {
      const wrapper = shallow(baseComponent())
      const assertion = wrapper.find('#info-cards').length
      expect(assertion).toBe(1)
    })
  })
})
