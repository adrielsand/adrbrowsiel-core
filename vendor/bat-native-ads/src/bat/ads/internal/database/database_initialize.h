/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_DATABASE_DATABASE_INITIALIZE_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_DATABASE_DATABASE_INITIALIZE_H_

#include <string>

#include "bat/ads/ads_client.h"
#include "bat/ads/mojom.h"

namespace ads {

namespace database {

class Initialize {
 public:
  Initialize();

  ~Initialize();

  void CreateOrOpen(ResultCallback callback);

  std::string get_last_message() const;

 private:
  void OnCreateOrOpen(DBCommandResponsePtr response, ResultCallback callback);

  std::string last_message_;
};

}  // namespace database
}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_DATABASE_DATABASE_INITIALIZE_H_
