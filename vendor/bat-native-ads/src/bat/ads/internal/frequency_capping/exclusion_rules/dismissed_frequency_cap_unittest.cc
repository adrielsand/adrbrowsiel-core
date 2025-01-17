/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/frequency_capping/exclusion_rules/dismissed_frequency_cap.h"

#include <vector>

#include "bat/ads/internal/frequency_capping/frequency_capping_unittest_util.h"
#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"

// npm run test -- adrbrowsiel_unit_tests --filter=BatAds*

namespace ads {

namespace {

const char kCreativeInstanceId[] = "9aea9a47-c6a0-4718-a0fa-706338bb2156";

const std::vector<std::string> kCampaignIds = {
    "60267cee-d5bb-4a0d-baaf-91cd7f18e07e",
    "90762cee-d5bb-4a0d-baaf-61cd7f18e07e"};

}  // namespace

class BatAdsDismissedFrequencyCapTest : public UnitTestBase {
 protected:
  BatAdsDismissedFrequencyCapTest() = default;

  ~BatAdsDismissedFrequencyCapTest() override = default;
};

TEST_F(BatAdsDismissedFrequencyCapTest, AllowAdIfThereIsNoAdsHistory) {
  // Arrange
  CreativeAdInfo ad;
  ad.creative_instance_id = kCreativeInstanceId;
  ad.campaign_id = kCampaignIds.at(0);

  const AdEventList ad_events;

  // Act
  DismissedFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad);

  // Assert
  EXPECT_FALSE(should_exclude);
}

TEST_F(BatAdsDismissedFrequencyCapTest,
       AdAllowedForAdWithSameCampaignIdWithin48HoursIfDismissed) {
  // Arrange
  CreativeAdInfo ad;
  ad.creative_instance_id = kCreativeInstanceId;
  ad.campaign_id = kCampaignIds.at(0);

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed,
      ConfirmationType::kDismissed,
  };

  AdEventList ad_events;

  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event =
        GenerateAdEvent(AdType::kAdNotification, ad, confirmation_type);

    ad_events.push_back(ad_event);

    FastForwardClockBy(base::TimeDelta::FromMinutes(5));
  }

  FastForwardClockBy(base::TimeDelta::FromHours(47));

  // Act
  DismissedFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad);

  // Assert
  EXPECT_FALSE(should_exclude);
}

TEST_F(
    BatAdsDismissedFrequencyCapTest,
    AdAllowedForAdWithSameCampaignIdWithin48HoursIfDismissedForMultipleTypes) {
  // Arrange
  CreativeAdInfo ad;
  ad.creative_instance_id = kCreativeInstanceId;
  ad.campaign_id = kCampaignIds.at(0);

  AdEventList ad_events;

  const AdEventInfo ad_event_1 = GenerateAdEvent(AdType::kAdNotification, ad,
                                                 ConfirmationType::kDismissed);
  ad_events.push_back(ad_event_1);

  const AdEventInfo ad_event_2 =
      GenerateAdEvent(AdType::kNewTabPageAd, ad, ConfirmationType::kDismissed);
  ad_events.push_back(ad_event_2);

  const AdEventInfo ad_event_3 = GenerateAdEvent(AdType::kPromotedContentAd, ad,
                                                 ConfirmationType::kDismissed);
  ad_events.push_back(ad_event_3);

  // Act
  DismissedFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad);

  // Assert
  EXPECT_FALSE(should_exclude);
}

TEST_F(BatAdsDismissedFrequencyCapTest,
       AdAllowedForAdWithSameCampaignIdWithin48HoursIfDismissedThenClicked) {
  // Arrange
  CreativeAdInfo ad;
  ad.creative_instance_id = kCreativeInstanceId;
  ad.campaign_id = kCampaignIds.at(0);

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kDismissed,
      ConfirmationType::kViewed, ConfirmationType::kClicked};

  AdEventList ad_events;

  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event =
        GenerateAdEvent(AdType::kAdNotification, ad, confirmation_type);

    ad_events.push_back(ad_event);

    FastForwardClockBy(base::TimeDelta::FromMinutes(5));
  }

  FastForwardClockBy(base::TimeDelta::FromHours(47));

  // Act
  DismissedFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad);

  // Assert
  EXPECT_FALSE(should_exclude);
}

TEST_F(BatAdsDismissedFrequencyCapTest,
       AdAllowedForAdWithSameCampaignIdAfter48HoursIfDismissedThenClicked) {
  // Arrange
  CreativeAdInfo ad;
  ad.creative_instance_id = kCreativeInstanceId;
  ad.campaign_id = kCampaignIds.at(0);

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kDismissed,
      ConfirmationType::kViewed, ConfirmationType::kClicked};

  AdEventList ad_events;

  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event =
        GenerateAdEvent(AdType::kAdNotification, ad, confirmation_type);

    ad_events.push_back(ad_event);

    FastForwardClockBy(base::TimeDelta::FromMinutes(5));
  }

  FastForwardClockBy(base::TimeDelta::FromHours(48));

  // Act
  DismissedFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad);

  // Assert
  EXPECT_FALSE(should_exclude);
}

TEST_F(BatAdsDismissedFrequencyCapTest,
       AdAllowedForAdWithSameCampaignIdWithin48HoursIfClickedThenDismissed) {
  // Arrange
  CreativeAdInfo ad;
  ad.creative_instance_id = kCreativeInstanceId;
  ad.campaign_id = kCampaignIds.at(0);

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kClicked,
      ConfirmationType::kViewed, ConfirmationType::kDismissed};

  AdEventList ad_events;

  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event =
        GenerateAdEvent(AdType::kAdNotification, ad, confirmation_type);

    ad_events.push_back(ad_event);

    FastForwardClockBy(base::TimeDelta::FromMinutes(5));
  }

  FastForwardClockBy(base::TimeDelta::FromHours(47));

  // Act
  DismissedFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad);

  // Assert
  EXPECT_FALSE(should_exclude);
}

TEST_F(BatAdsDismissedFrequencyCapTest,
       AdAllowedForAdWithSameCampaignIdAfter48HoursIfClickedThenDismissed) {
  // Arrange
  CreativeAdInfo ad;
  ad.creative_instance_id = kCreativeInstanceId;
  ad.campaign_id = kCampaignIds.at(0);

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kClicked,
      ConfirmationType::kViewed, ConfirmationType::kDismissed};

  AdEventList ad_events;

  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event =
        GenerateAdEvent(AdType::kAdNotification, ad, confirmation_type);

    ad_events.push_back(ad_event);

    FastForwardClockBy(base::TimeDelta::FromMinutes(5));
  }

  FastForwardClockBy(base::TimeDelta::FromHours(48));

  // Act
  DismissedFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad);

  // Assert
  EXPECT_FALSE(should_exclude);
}

TEST_F(
    BatAdsDismissedFrequencyCapTest,
    AdAllowedForAdWithSameCampaignIdAfter48HoursIfClickedThenDismissedTwice) {
  // Arrange
  CreativeAdInfo ad;
  ad.creative_instance_id = kCreativeInstanceId;
  ad.campaign_id = kCampaignIds.at(0);

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kClicked,
      ConfirmationType::kViewed, ConfirmationType::kDismissed,
      ConfirmationType::kViewed, ConfirmationType::kDismissed};

  AdEventList ad_events;

  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event =
        GenerateAdEvent(AdType::kAdNotification, ad, confirmation_type);

    ad_events.push_back(ad_event);

    FastForwardClockBy(base::TimeDelta::FromMinutes(5));
  }

  FastForwardClockBy(base::TimeDelta::FromHours(48));

  // Act
  DismissedFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad);

  // Assert
  EXPECT_FALSE(should_exclude);
}

TEST_F(
    BatAdsDismissedFrequencyCapTest,
    AdNotAllowedForAdWithSameCampaignIdWithin48HoursIfClickedThenDismissedTwice) {  // NOLINT
  // Arrange
  CreativeAdInfo ad;
  ad.creative_instance_id = kCreativeInstanceId;
  ad.campaign_id = kCampaignIds.at(0);

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kClicked,
      ConfirmationType::kViewed, ConfirmationType::kDismissed,
      ConfirmationType::kViewed, ConfirmationType::kDismissed};

  AdEventList ad_events;

  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event =
        GenerateAdEvent(AdType::kAdNotification, ad, confirmation_type);

    ad_events.push_back(ad_event);

    FastForwardClockBy(base::TimeDelta::FromMinutes(5));
  }

  FastForwardClockBy(base::TimeDelta::FromHours(47));

  // Act
  DismissedFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad);

  // Assert
  EXPECT_TRUE(should_exclude);
}

TEST_F(BatAdsDismissedFrequencyCapTest,
       AdAllowedForAdWithDifferentCampaignIdWithin48Hours) {
  // Arrange
  CreativeAdInfo ad_1;
  ad_1.creative_instance_id = kCreativeInstanceId;
  ad_1.campaign_id = kCampaignIds.at(0);

  CreativeAdInfo ad_2;
  ad_2.creative_instance_id = kCreativeInstanceId;
  ad_2.campaign_id = kCampaignIds.at(1);

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kDismissed,
      ConfirmationType::kViewed, ConfirmationType::kDismissed};

  AdEventList ad_events;

  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event =
        GenerateAdEvent(AdType::kAdNotification, ad_2, confirmation_type);

    ad_events.push_back(ad_event);

    FastForwardClockBy(base::TimeDelta::FromMinutes(5));
  }

  FastForwardClockBy(base::TimeDelta::FromHours(47));

  // Act
  DismissedFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad_1);

  // Assert
  EXPECT_FALSE(should_exclude);
}

TEST_F(BatAdsDismissedFrequencyCapTest,
       AdAllowedForAdWithDifferentCampaignIdAfter48Hours) {
  // Arrange
  CreativeAdInfo ad_1;
  ad_1.creative_instance_id = kCreativeInstanceId;
  ad_1.campaign_id = kCampaignIds.at(0);

  CreativeAdInfo ad_2;
  ad_2.creative_instance_id = kCreativeInstanceId;
  ad_2.campaign_id = kCampaignIds.at(1);

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kDismissed,
      ConfirmationType::kViewed, ConfirmationType::kDismissed};

  AdEventList ad_events;

  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event =
        GenerateAdEvent(AdType::kAdNotification, ad_2, confirmation_type);

    ad_events.push_back(ad_event);

    FastForwardClockBy(base::TimeDelta::FromMinutes(5));
  }

  FastForwardClockBy(base::TimeDelta::FromHours(48));

  // Act
  DismissedFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad_1);

  // Assert
  EXPECT_FALSE(should_exclude);
}

}  // namespace ads
