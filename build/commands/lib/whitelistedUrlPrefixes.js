// Before adding to this list, get approval from the security team
module.exports = [
  'http://update.googleapis.com/service/update2', // allowed because it 307's to go-updater.adrbrowsiel.com. should never actually connect to googleapis.com.
  'https://update.googleapis.com/service/update2', // allowed because it 307's to go-updater.adrbrowsiel.com. should never actually connect to googleapis.com.
  'https://safebrowsing.googleapis.com/v4/threatListUpdates', // allowed because it 307's to safebrowsing.adrbrowsiel.com
  'https://clients2.googleusercontent.com/crx/blobs/',
  'http://dl.google.com/', // allowed because it 307's to redirector.adrbrowsiel.com
  'https://dl.google.com/', // allowed because it 307's to redirector.adrbrowsiel.com
  'https://no-thanks.invalid/', // fake gaia URL
  'https://go-updater.adrbrowsiel.com/',
  'https://safebrowsing.adrbrowsiel.com/',
  'https://adrbrowsiel-core-ext.s3.adrbrowsiel.com/',
  'https://laptop-updates.adrbrowsiel.com/',
  'https://static.adrbrowsiel.com/',
  'https://static1.adrbrowsiel.com/',
  'http://componentupdater.adrbrowsiel.com/service/update2', // allowed because it 307's to https://componentupdater.adrbrowsiel.com
  'https://componentupdater.adrbrowsiel.com/service/update2',
  'https://crlsets.adrbrowsiel.com/',
  'https://crxdownload.adrbrowsiel.com/crx/blobs/',
  'https://updates.adrbrowsielsoftware.com/',
  'https://p3a.adrbrowsiel.com/',
  'https://dns.google/dns-query', // needed for DoH on Mac build machines
  'https://chrome.cloudflare-dns.com/dns-query', // needed for DoH on Mac build machines
  'https://tor.adrbrowsielsoftware.com/', // for fetching tor client updater component
  'https://redirector.adrbrowsiel.com/',
  'https://sync-v2.adrbrowsiel.com/v2', // adrbrowsiel sync v2 production
  'https://sync-v2.adrbrowsielsoftware.com/v2', // adrbrowsiel sync v2 staging
  'https://sync-v2.adrbrowsiel.software/v2', // adrbrowsiel sync v2 dev
  'https://variations.adrbrowsiel.com/seed', // adrbrowsiel A/B testing
  'https://adrbrowsiel-today-cdn.adrbrowsiel.com/', // adrbrowsiel Today (production)
  'https://pcdn.adrbrowsiel.com/', // adrbrowsiel's Privacy-focused CDN
]
