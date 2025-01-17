/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import {
  isHttpOrHttps,
  getOrigin,
  getHostname,
  stripProtocolFromUrl
} from '../../../adrbrowsiel_extension/extension/adrbrowsiel_extension/helpers/urlUtils'

describe('urlUtils test', () => {
  describe('isHttpOrHttps', () => {
    it('matches http when defined as a protocol type', () => {
      const url = 'http://some-boring-unsafe-website.com'
      expect(isHttpOrHttps(url)).toBe(true)
    })
    it('matches https when defined as a protocol type', () => {
      const url = 'https://some-nice-safe-website.com'
      expect(isHttpOrHttps(url)).toBe(true)
    })
    it('does not match http when defined as an origin', () => {
      const url = 'file://http.some-website-tricking-you.com'
      expect(isHttpOrHttps(url)).toBe(false)
    })
    it('does not match https when defined as an origin', () => {
      const url = 'file://https.some-website-tricking-you.com'
      expect(isHttpOrHttps(url)).toBe(false)
    })
    it('does not match other protocol', () => {
      const url = 'ftp://some-old-website.com'
      expect(isHttpOrHttps(url)).toBe(false)
    })
    it('does not match when url is not defined', () => {
      const url = undefined
      expect(isHttpOrHttps(url)).toBe(false)
    })
    it('matches uppercase http', () => {
      const url = 'HTTP://SCREAMING-SAFE-WEBSITE.COM'
      expect(isHttpOrHttps(url)).toBe(true)
    })
    it('matches uppercase https', () => {
      const url = 'HTTP://SCREAMING-UNSAFE-WEBSITE.COM'
      expect(isHttpOrHttps(url)).toBe(true)
    })
  })
  describe('getOrigin', () => {
    it('properly gets the origin from an URL', () => {
      const url = 'https://pokemons-invading-tests-breaking-stuff.com/you-knew-that.js'
      expect(getOrigin(url)).toBe('https://pokemons-invading-tests-breaking-stuff.com')
    })
    it('returns the full URL if origin is not valid', () => {
      const url = 'data:application/javascript;base64,c3z4r4vgvst0n00b'
      expect(getOrigin(url)).toBe('data:application/javascript;base64,c3z4r4vgvst0n00b')
    })
  })
  describe('getHostname', () => {
    it('properly gets the hostname from an URL', () => {
      const url = 'https://pokemons-invading-tests-breaking-stuff.com/you-knew-that.js'
      expect(getHostname(url)).toBe('pokemons-invading-tests-breaking-stuff.com')
    })
    it('returns the full URL if origin is not valid', () => {
      const url = 'data:application/javascript;base64,c3z4r4vgvst0n00b'
      expect(getHostname(url)).toBe('data:application/javascript;base64,c3z4r4vgvst0n00b')
    })
  })
  describe('stripProtocolFromUrl', () => {
    it('properly strips out an HTTP protocol', () => {
      const url = 'http://adrbrowsiel.com'
      expect(stripProtocolFromUrl(url)).toBe('adrbrowsiel.com')
    })
    it('properly strips out an HTTPS protocol', () => {
      const url = 'https://adrbrowsiel.com'
      expect(stripProtocolFromUrl(url)).toBe('adrbrowsiel.com')
    })
    it('properly strips out an HTTP protocol when domain has HTTP as its name', () => {
      const url = 'https://breakthis.http.com'
      expect(stripProtocolFromUrl(url)).toBe('breakthis.http.com')
    })
    it('properly strips out an HTTPS protocol when domain has HTTPS as its name', () => {
      const url = 'https://breakthis.https.com'
      expect(stripProtocolFromUrl(url)).toBe('breakthis.https.com')
    })
  })
})
