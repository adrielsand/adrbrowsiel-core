/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/frequency_capping/exclusion_rules/transferred_frequency_cap.h"

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

class BatAdsTransferredFrequencyCapTest : public UnitTestBase {
 protected:
  BatAdsTransferredFrequencyCapTest() = default;

  ~BatAdsTransferredFrequencyCapTest() override = default;
};

TEST_F(BatAdsTransferredFrequencyCapTest, AllowAdIfThereIsNoAdsHistory) {
  // Arrange
  CreativeAdInfo ad;
  ad.creative_instance_id = kCreativeInstanceId;
  ad.campaign_id = kCampaignIds.at(0);

  const AdEventList ad_events;

  // Act
  TransferredFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad);

  // Assert
  EXPECT_FALSE(should_exclude);
}

TEST_F(BatAdsTransferredFrequencyCapTest,
       AdAllowedForAdWithDifferentCampaignIdWithin48Hours) {
  // Arrange
  CreativeAdInfo ad_1;
  ad_1.creative_instance_id = kCreativeInstanceId;
  ad_1.campaign_id = kCampaignIds.at(0);

  CreativeAdInfo ad_2;
  ad_2.creative_instance_id = kCreativeInstanceId;
  ad_2.campaign_id = kCampaignIds.at(1);

  AdEventList ad_events;

  const AdEventInfo ad_event = GenerateAdEvent(AdType::kAdNotification, ad_2,
                                               ConfirmationType::kTransferred);

  ad_events.push_back(ad_event);

  task_environment_.FastForwardBy(base::TimeDelta::FromHours(47));

  // Act
  TransferredFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad_1);

  // Assert
  EXPECT_FALSE(should_exclude);
}

TEST_F(BatAdsTransferredFrequencyCapTest,
       AdAllowedForAdWithDifferentCampaignIdWithin48HoursForMultipleTypes) {
  // Arrange
  CreativeAdInfo ad_1;
  ad_1.creative_instance_id = kCreativeInstanceId;
  ad_1.campaign_id = kCampaignIds.at(0);

  CreativeAdInfo ad_2;
  ad_2.creative_instance_id = kCreativeInstanceId;
  ad_2.campaign_id = kCampaignIds.at(1);

  AdEventList ad_events;

  const AdEventInfo ad_event_1 = GenerateAdEvent(
      AdType::kAdNotification, ad_2, ConfirmationType::kTransferred);
  ad_events.push_back(ad_event_1);

  const AdEventInfo ad_event_2 = GenerateAdEvent(
      AdType::kNewTabPageAd, ad_2, ConfirmationType::kTransferred);
  ad_events.push_back(ad_event_2);

  const AdEventInfo ad_event_3 = GenerateAdEvent(
      AdType::kPromotedContentAd, ad_2, ConfirmationType::kTransferred);
  ad_events.push_back(ad_event_3);

  task_environment_.FastForwardBy(base::TimeDelta::FromHours(47));

  // Act
  TransferredFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad_1);

  // Assert
  EXPECT_FALSE(should_exclude);
}

TEST_F(BatAdsTransferredFrequencyCapTest,
       AdNotAllowedForAdWithSameCampaignIdWithin48Hours) {
  // Arrange
  CreativeAdInfo ad;
  ad.creative_instance_id = kCreativeInstanceId;
  ad.campaign_id = kCampaignIds.at(0);

  AdEventList ad_events;

  const AdEventInfo ad_event = GenerateAdEvent(AdType::kAdNotification, ad,
                                               ConfirmationType::kTransferred);

  ad_events.push_back(ad_event);

  task_environment_.FastForwardBy(base::TimeDelta::FromHours(47));

  // Act
  TransferredFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad);

  // Assert
  EXPECT_TRUE(should_exclude);
}

TEST_F(BatAdsTransferredFrequencyCapTest,
       AdAllowedForAdWithSameCampaignIdAfter48Hours) {
  // Arrange
  CreativeAdInfo ad;
  ad.creative_instance_id = kCreativeInstanceId;
  ad.campaign_id = kCampaignIds.at(0);

  AdEventList ad_events;

  const AdEventInfo ad_event = GenerateAdEvent(AdType::kAdNotification, ad,
                                               ConfirmationType::kTransferred);

  ad_events.push_back(ad_event);

  task_environment_.FastForwardBy(base::TimeDelta::FromHours(48));

  // Act
  TransferredFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad);

  // Assert
  EXPECT_FALSE(should_exclude);
}

TEST_F(BatAdsTransferredFrequencyCapTest,
       AdAllowedForAdWithDifferentCampaignIdAfter48Hours) {
  // Arrange
  CreativeAdInfo ad_1;
  ad_1.creative_instance_id = kCreativeInstanceId;
  ad_1.campaign_id = kCampaignIds.at(0);

  CreativeAdInfo ad_2;
  ad_2.creative_instance_id = kCreativeInstanceId;
  ad_2.campaign_id = kCampaignIds.at(1);

  AdEventList ad_events;

  const AdEventInfo ad_event = GenerateAdEvent(AdType::kAdNotification, ad_2,
                                               ConfirmationType::kTransferred);

  ad_events.push_back(ad_event);

  task_environment_.FastForwardBy(base::TimeDelta::FromHours(48));

  // Act
  TransferredFrequencyCap frequency_cap(ad_events);
  const bool should_exclude = frequency_cap.ShouldExclude(ad_1);

  // Assert
  EXPECT_FALSE(should_exclude);
}

}  // namespace ads
