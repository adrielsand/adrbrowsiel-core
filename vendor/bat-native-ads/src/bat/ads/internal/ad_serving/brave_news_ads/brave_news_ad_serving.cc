/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ad_serving/brave_news_ads/brave_news_ad_serving.h"

#include <vector>
#include "base/rand_util.h"
#include "base/strings/stringprintf.h"
#include "bat/ads/ad_type.h"
#include "bat/ads/brave_news_ad_info.h"
#include "bat/ads/internal/ad_serving/ad_targeting/geographic/subdivision/subdivision_targeting.h"
#include "bat/ads/internal/ad_targeting/ad_targeting.h"
#include "bat/ads/internal/ad_targeting/ad_targeting_segment.h"
#include "bat/ads/internal/ads/brave_news_ads/brave_news_ad_builder.h"
#include "bat/ads/internal/bundle/creative_brave_news_ad_info.h"
#include "bat/ads/internal/eligible_ads/brave_news_ads/eligible_brave_news_ads.h"
#include "bat/ads/internal/logging.h"
#include "bat/ads/internal/p2a/p2a.h"
#include "bat/ads/internal/p2a/p2a_util.h"
#include "bat/ads/internal/resources/frequency_capping/anti_targeting_resource.h"

namespace ads {
namespace brave_news_ads {

namespace {

// TODO(tmancey): Refactor to be generic
void RecordAdOpportunityForSegments(const AdType& ad_type,
                                    const SegmentList& segments) {
  const std::string type_as_string = std::string(ad_type);

  const std::string name =
      base::StringPrintf("%s_opportunity", type_as_string.c_str());

  const std::vector<std::string> questions =
      p2a::CreateAdOpportunityQuestionList(segments);

  p2a::RecordEvent(name, questions);
}

}  // namespace

AdServing::AdServing(
    AdTargeting* ad_targeting,
    ad_targeting::geographic::SubdivisionTargeting* subdivision_targeting,
    resource::AntiTargeting* anti_targeting_resource)
    : ad_targeting_(ad_targeting),
      subdivision_targeting_(subdivision_targeting),
      anti_targeting_resource_(anti_targeting_resource),
      eligible_ads_(std::make_unique<EligibleAds>(subdivision_targeting,
                                                  anti_targeting_resource)) {
  DCHECK(ad_targeting_);
  DCHECK(subdivision_targeting_);
  DCHECK(anti_targeting_resource_);
}

AdServing::~AdServing() = default;

void AdServing::MaybeServeAd(const std::string& size,
                             GetBraveNewsAdCallback callback) {
  const SegmentList segments = ad_targeting_->GetSegments();

  DCHECK(eligible_ads_);
  eligible_ads_->GetForSegments(
      segments, size,
      [=](const bool was_allowed, const CreativeBraveNewsAdList& ads) {
        if (was_allowed) {
          RecordAdOpportunityForSegments(AdType::kBraveNewsAd, segments);
        }

        BraveNewsAdInfo brave_news_ad;

        if (ads.empty()) {
          BLOG(1, "Brave News ad not served: No eligible ads found");
          callback(/* success */ false, size, brave_news_ad);
          return;
        }

        BLOG(1, "Found " << ads.size() << " eligible ads");

        const int rand = base::RandInt(0, ads.size() - 1);
        const CreativeBraveNewsAdInfo ad = ads.at(rand);

        brave_news_ad = BuildBraveNewsAd(ad);

        callback(/* success */ true, size, brave_news_ad);
      });
}

}  // namespace brave_news_ads
}  // namespace ads
