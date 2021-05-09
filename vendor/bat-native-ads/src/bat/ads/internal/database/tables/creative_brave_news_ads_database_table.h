/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_DATABASE_TABLES_CREATIVE_BRAVE_NEWS_ADS_DATABASE_TABLE_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_DATABASE_TABLES_CREATIVE_BRAVE_NEWS_ADS_DATABASE_TABLE_H_

#include <memory>
#include <string>
#include <vector>

#include "bat/ads/ads_client.h"
#include "bat/ads/internal/ad_targeting/ad_targeting.h"
#include "bat/ads/internal/bundle/creative_brave_news_ad_info.h"
#include "bat/ads/internal/database/database_table.h"
#include "bat/ads/internal/database/tables/campaigns_database_table.h"
#include "bat/ads/internal/database/tables/creative_ads_database_table.h"
#include "bat/ads/internal/database/tables/dayparts_database_table.h"
#include "bat/ads/internal/database/tables/geo_targets_database_table.h"
#include "bat/ads/internal/database/tables/segments_database_table.h"
#include "bat/ads/mojom.h"
#include "bat/ads/result.h"

namespace ads {

using GetCreativeBraveNewsAdCallback =
    std::function<void(const Result result,
                       const std::string& creative_instance_id,
                       const CreativeBraveNewsAdInfo& ad)>;

using GetCreativeBraveNewsAdsCallback =
    std::function<void(const Result result,
                       const std::vector<std::string>& segments,
                       const CreativeBraveNewsAdList& ads)>;

namespace database {
namespace table {

class CreativeBraveNewsAds : public Table {
 public:
  CreativeBraveNewsAds();

  ~CreativeBraveNewsAds() override;

  void Save(const CreativeBraveNewsAdList& creative_brave_news_ads,
            ResultCallback callback);

  void Delete(ResultCallback callback);

  void GetForCreativeInstanceId(const std::string& creative_instance_id,
                                GetCreativeBraveNewsAdCallback callback);

  void GetForSegments(const SegmentList& segments,
                      GetCreativeBraveNewsAdsCallback callback);

  void GetAll(GetCreativeBraveNewsAdsCallback callback);

  void set_batch_size(const int batch_size);

  std::string get_table_name() const override;

  void Migrate(DBTransaction* transaction, const int to_version) override;

 private:
  void InsertOrUpdate(DBTransaction* transaction,
                      const CreativeBraveNewsAdList& creative__brave_news_ads);

  int BindParameters(DBCommand* command,
                     const CreativeBraveNewsAdList& creative__brave_news_ads);

  std::string BuildInsertOrUpdateQuery(
      DBCommand* command,
      const CreativeBraveNewsAdList& creative__brave_news_ads);

  void OnGetForCreativeInstanceId(DBCommandResponsePtr response,
                                  const std::string& creative_instance_id,
                                  GetCreativeBraveNewsAdCallback callback);

  void OnGetForSegments(DBCommandResponsePtr response,
                        const SegmentList& segments,
                        GetCreativeBraveNewsAdsCallback callback);

  void OnGetAll(DBCommandResponsePtr response,
                GetCreativeBraveNewsAdsCallback callback);

  CreativeBraveNewsAdInfo GetFromRecord(DBRecord* record) const;

  void CreateTableV14(DBTransaction* transaction);
  void MigrateToV14(DBTransaction* transaction);

  int batch_size_;

  std::unique_ptr<Campaigns> campaigns_database_table_;
  std::unique_ptr<CreativeAds> creative_ads_database_table_;
  std::unique_ptr<Dayparts> dayparts_database_table_;
  std::unique_ptr<GeoTargets> geo_targets_database_table_;
  std::unique_ptr<Segments> segments_database_table_;
};

}  // namespace table
}  // namespace database
}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_DATABASE_TABLES_CREATIVE_BRAVE_NEWS_ADS_DATABASE_TABLE_H_
