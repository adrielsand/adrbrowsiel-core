// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'

import {
  SettingsMenu,
  SettingsTitle,
  SettingsWrapper,
  SettingsSidebar,
  SettingsFeatureBody,
  SettingsContent,
  SettingsCloseIcon,
  SettingsSidebarButton,
  SettingsSidebarActiveButtonSlider,
  SettingsSidebarButtonText,
  SettingsSidebarSVGContent
} from '../../components/default'

import { getLocale } from '../../../common/locale'

// Icons
import { CloseStrokeIcon } from 'adrbrowsiel-ui/components/icons'
import BackgroundImageIcon from './settings/icons/backgroundImage.svg'
import NraveStatsIcon from './settings/icons/adrbrowsielStats.svg'
import TopSitesIcon from './settings/icons/topSites.svg'
import ClockIcon from './settings/icons/clock.svg'
import CardsIcon from './settings/icons/cards.svg'
import TodayIcon from './settings/icons/adrbrowsielToday.svg'

// Tabs
const BackgroundImageSettings = React.lazy(() => import('./settings/backgroundImage'))
const adrbrowsielStatsSettings = React.lazy(() => import('./settings/adrbrowsielStats'))
const TopSitesSettings = React.lazy(() => import('./settings/topSites'))
const ClockSettings = React.lazy(() => import('./settings/clock'))
const CardsSettings = React.lazy(() => import('./settings/cards'))
const adrbrowsielTodaySettings = React.lazy(() => import('./settings/adrbrowsielToday'))

// Types
import { NewTabActions } from '../../constants/new_tab_types'

export interface Props {
  actions: NewTabActions
  textDirection: string
  showSettingsMenu: boolean
  onClose: () => void
  onDisplayTodaySection: () => any
  onClearTodayPrefs: () => any
  toggleShowBackgroundImage: () => void
  toggleShowClock: () => void
  toggleShowStats: () => void
  toggleShowToday: () => any
  toggleShowTopSites: () => void
  setMostVisitedSettings: (show: boolean, customize: boolean) => void
  toggleShowRewards: () => void
  toggleShowTogether: () => void
  toggleShowBinance: () => void
  toggleShowGemini: () => void
  toggleShowCryptoDotCom: () => void
  toggleShowFTX: () => void
  toggleBrandedWallpaperOptIn: () => void
  toggleCards: (show: boolean) => void
  showBackgroundImage: boolean
  showStats: boolean
  showToday: boolean
  showClock: boolean
  clockFormat: string
  showTopSites: boolean
  customLinksEnabled: boolean
  brandedWallpaperOptIn: boolean
  allowSponsoredWallpaperUI: boolean
  showRewards: boolean
  showTogether: boolean
  showBinance: boolean
  binanceSupported: boolean
  togetherSupported: boolean
  showGemini: boolean
  geminiSupported: boolean
  showCryptoDotCom: boolean
  cryptoDotComSupported: boolean
  showFTX: boolean
  ftxSupported: boolean
  todayPublishers?: adrbrowsielToday.Publishers
  setActiveTab?: TabType
  cardsHidden: boolean
}

export enum TabType {
  BackgroundImage = 'backgroundImage',
  adrbrowsielStats = 'adrbrowsielStats',
  TopSites = 'topSites',
  adrbrowsielToday = 'adrbrowsielToday',
  Clock = 'clock',
  Cards = 'cards'
}

interface State {
  activeTab: TabType
}

const allTabTypes = [...Object.values(TabType)]
const allTabTypesWithoutBackground = [...allTabTypes]
allTabTypesWithoutBackground.splice(allTabTypesWithoutBackground.indexOf(TabType.BackgroundImage), 1)

export default class Settings extends React.PureComponent<Props, State> {
  settingsMenuRef: React.RefObject<any>
  constructor (props: Props) {
    super(props)
    this.settingsMenuRef = React.createRef()
    this.state = {
      activeTab: this.getInitialTab()
    }
  }

  handleClickOutside = (event: Event) => {
    if (
      this.settingsMenuRef &&
      this.settingsMenuRef.current &&
      !this.settingsMenuRef.current.contains(event.target)
    ) {
      this.props.onClose()
    }
  }

  componentDidMount () {
    document.addEventListener('mousedown', this.handleClickOutside)
    document.addEventListener('keydown', this.onKeyPressSettings)
  }

  componentWillUnmount () {
    document.removeEventListener('mousedown', this.handleClickOutside)
  }

  componentDidUpdate (prevProps: Props) {
    if (prevProps.setActiveTab !== this.props.setActiveTab && this.props.setActiveTab) {
      this.setActiveTab(this.props.setActiveTab)
    }
    const isNewlyShown = (!prevProps.showSettingsMenu && this.props.showSettingsMenu)
    if (isNewlyShown) {
      this.setActiveTab(this.getInitialTab())
    }
  }

  onKeyPressSettings = (event: KeyboardEvent) => {
    if (event.key === 'Escape') {
      this.props.onClose()
    }
  }

  getInitialTab () {
    let tab = this.props.allowSponsoredWallpaperUI
      ? TabType.BackgroundImage
      : TabType.adrbrowsielStats
    if (this.props.setActiveTab) {
      if (this.getActiveTabTypes().includes(this.props.setActiveTab)) {
        tab = this.props.setActiveTab
      }
    }
    return tab
  }

  toggleShowBackgroundImage = () => {
    this.props.toggleShowBackgroundImage()
  }

  setActiveTab (activeTab: TabType) {
    this.setState({ activeTab })
  }

  getActiveTabTypes (): TabType[] {
    // TODO(petemill): We're not allowing
    // any background image changes when user is not
    // in a sponsored image region, which is weird.
    // Seems like this should actually only be for
    // super referral users, where the bg image is
    // mandatory. Maybe that's the only case
    // allowSponsoredWallpaperUI is false?
    if (!this.props.allowSponsoredWallpaperUI) {
      return allTabTypesWithoutBackground
    } else {
      return allTabTypes
    }
  }

  getTabIcon (tab: TabType, isActiveTab: boolean) {
    let srcUrl
    switch (tab) {
      case TabType.BackgroundImage:
        srcUrl = BackgroundImageIcon
        break
      case TabType.adrbrowsielStats:
        srcUrl = NraveStatsIcon
        break
      case TabType.TopSites:
        srcUrl = TopSitesIcon
        break
      case TabType.adrbrowsielToday:
        srcUrl = TodayIcon
        break
      case TabType.Clock:
        srcUrl = ClockIcon
        break
      case TabType.Cards:
        srcUrl = CardsIcon
        break
      default:
        srcUrl = BackgroundImageIcon
        break
    }
    return <SettingsSidebarSVGContent isActive={isActiveTab} src={srcUrl} />
  }

  getTabTitleKey = (tab: TabType) => {
    switch (tab) {
      case TabType.BackgroundImage:
        return 'backgroundImageTitle'
      case TabType.adrbrowsielStats:
        return 'statsTitle'
      case TabType.TopSites:
        return 'topSitesTitle'
      case TabType.adrbrowsielToday:
        return 'adrbrowsielTodayTitle'
      case TabType.Clock:
        return 'clockTitle'
      case TabType.Cards:
        return 'cards'
      default:
        return ''
    }
  }

  render () {
    const {
      textDirection,
      showSettingsMenu,
      toggleShowClock,
      toggleShowStats,
      toggleShowTopSites,
      setMostVisitedSettings,
      toggleShowRewards,
      toggleShowTogether,
      toggleBrandedWallpaperOptIn,
      showBackgroundImage,
      showStats,
      showClock,
      clockFormat,
      showTopSites,
      customLinksEnabled,
      showRewards,
      showTogether,
      brandedWallpaperOptIn,
      toggleShowBinance,
      showBinance,
      binanceSupported,
      togetherSupported,
      toggleShowGemini,
      geminiSupported,
      showGemini,
      toggleShowCryptoDotCom,
      cryptoDotComSupported,
      showCryptoDotCom,
      toggleShowFTX,
      ftxSupported,
      showFTX,
      toggleCards,
      cardsHidden
    } = this.props
    const { activeTab } = this.state

    if (!showSettingsMenu) {
      return null
    }

    const tabTypes = this.getActiveTabTypes()
    return (
      <SettingsWrapper textDirection={textDirection}>
        <SettingsMenu
          ref={this.settingsMenuRef}
          textDirection={textDirection}
          title={getLocale('dashboardSettingsTitle')}
        >
          <SettingsTitle id='settingsTitle'>
            <h1>{getLocale('dashboardSettingsTitle')}</h1>
            <SettingsCloseIcon onClick={this.props.onClose}>
              <CloseStrokeIcon />
            </SettingsCloseIcon>
          </SettingsTitle>
          <SettingsContent id='settingsBody'>
            <SettingsSidebar id='sidebar'>
              <SettingsSidebarActiveButtonSlider
                translateTo={tabTypes.indexOf(activeTab)}
              />
              {
                tabTypes.map((tabType, index) => {
                  const titleKey = this.getTabTitleKey(tabType)
                  const isActive = (activeTab === tabType)
                  return (
                    <SettingsSidebarButton
                      tabIndex={0}
                      key={`sidebar-button-${index}`}
                      activeTab={isActive}
                      onClick={this.setActiveTab.bind(this, tabType)}
                    >
                      {this.getTabIcon(tabType, isActive)}
                      <SettingsSidebarButtonText
                        isActive={isActive}
                        data-text={getLocale(titleKey)}
                      >
                        {getLocale(titleKey)}
                      </SettingsSidebarButtonText>
                    </SettingsSidebarButton>
                  )
                })
              }
            </SettingsSidebar>
            <SettingsFeatureBody id='content'>
              {/* Empty loading fallback is ok here since we are loading from local disk. */}
              <React.Suspense fallback={(<div/>)}>
              {
                activeTab === TabType.BackgroundImage
                  ? (
                  <BackgroundImageSettings
                    toggleBrandedWallpaperOptIn={toggleBrandedWallpaperOptIn}
                    toggleShowBackgroundImage={this.toggleShowBackgroundImage}
                    brandedWallpaperOptIn={brandedWallpaperOptIn}
                    showBackgroundImage={showBackgroundImage}
                  />
                ) : null
              }
              {
                activeTab === TabType.adrbrowsielStats
                  ? (
                    <adrbrowsielStatsSettings
                      toggleShowStats={toggleShowStats}
                      showStats={showStats}
                    />
                  ) : null
              }
              {
                activeTab === TabType.TopSites
                  ? (
                    <TopSitesSettings
                      toggleShowTopSites={toggleShowTopSites}
                      showTopSites={showTopSites}
                      customLinksEnabled={customLinksEnabled}
                      setMostVisitedSettings={setMostVisitedSettings}
                    />
                  ) : null
              }
              {
                activeTab === TabType.adrbrowsielToday
                ? (
                  <adrbrowsielTodaySettings
                    publishers={this.props.todayPublishers}
                    setPublisherPref={this.props.actions.today.setPublisherPref}
                    onDisplay={this.props.onDisplayTodaySection}
                    onClearPrefs={this.props.onClearTodayPrefs}
                    showToday={this.props.showToday}
                    toggleShowToday={this.props.toggleShowToday}
                  />
                ) : null
              }
              {
                activeTab === TabType.Clock
                  ? (
                    <ClockSettings
                      actions={this.props.actions}
                      toggleShowClock={toggleShowClock}
                      showClock={showClock}
                      clockFormat={clockFormat}
                    />
                  ) : null
              }
              {
                activeTab === TabType.Cards
                  ? (
                    <CardsSettings
                      toggleCards={toggleCards}
                      cardsHidden={cardsHidden}
                      toggleShowBinance={toggleShowBinance}
                      showBinance={showBinance}
                      binanceSupported={binanceSupported}
                      toggleShowTogether={toggleShowTogether}
                      showTogether={showTogether}
                      togetherSupported={togetherSupported}
                      toggleShowRewards={toggleShowRewards}
                      showRewards={showRewards}
                      showGemini={showGemini}
                      toggleShowGemini={toggleShowGemini}
                      geminiSupported={geminiSupported}
                      toggleShowCryptoDotCom={toggleShowCryptoDotCom}
                      cryptoDotComSupported={cryptoDotComSupported}
                      showCryptoDotCom={showCryptoDotCom}
                      toggleShowFTX={toggleShowFTX}
                      ftxSupported={ftxSupported}
                      showFTX={showFTX}
                    />
                  ) : null
              }
              </React.Suspense>
            </SettingsFeatureBody>
          </SettingsContent>
        </SettingsMenu>
      </SettingsWrapper>
    )
  }
}
