/* global jest, expect, describe, it, afterEach */
import * as React from 'react'
import { shallow } from 'enzyme'
import { create } from 'react-test-renderer'
import { TestThemeProvider } from 'adrbrowsiel-ui/theme'
import AdRowsDetails from './index'

describe('Ad Rows Details tests', () => {
  const baseComponent = (props?: object) => <TestThemeProvider><AdRowsDetails id={'adRowsDetails'} /></TestThemeProvider>

  describe('basic tests', () => {
    it('matches the snapshot', () => {
      const component = baseComponent()
      const tree = create(component).toJSON()
      expect(tree).toMatchSnapshot()
    })

    it('renders the component', () => {
      const wrapper = shallow(baseComponent())
      const assertion = wrapper.find('#adRowsDetails').length
      expect(assertion).toBe(1)
    })
  })
})
