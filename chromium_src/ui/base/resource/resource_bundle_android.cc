// Copyright (c) 2019 The adrbrowsiel Authors
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "../../../../../ui/base/resource/resource_bundle_android.cc"

namespace ui {

namespace {

int g_adrbrowsiel_resources_pack_fd = -1;
int g_adrbrowsiel_100_percent_pack_fd = -1;
base::MemoryMappedFile::Region g_adrbrowsiel_resources_pack_region;
base::MemoryMappedFile::Region g_adrbrowsiel_100_percent_pack_region;

}

void adrbrowsielLoadMainAndroidPackFile(const char* path_within_apk,
                                  const base::FilePath& disk_file_path) {
  if (LoadFromApkOrFile(path_within_apk,
                        &disk_file_path,
                        &g_adrbrowsiel_resources_pack_fd,
                        &g_adrbrowsiel_resources_pack_region)) {
    ResourceBundle::GetSharedInstance().AddDataPackFromFileRegion(
        base::File(g_adrbrowsiel_resources_pack_fd), g_adrbrowsiel_resources_pack_region,
        SCALE_FACTOR_NONE);
  }
}

void adrbrowsielLoadadrbrowsiel100PercentPackFile(const char* path_within_apk,
                                      const base::FilePath& disk_file_path) {
  if (LoadFromApkOrFile(path_within_apk,
                        &disk_file_path,
                        &g_adrbrowsiel_100_percent_pack_fd,
                        &g_adrbrowsiel_100_percent_pack_region)) {
    ResourceBundle::GetSharedInstance().AddDataPackFromFileRegion(
        base::File(g_adrbrowsiel_100_percent_pack_fd),
        g_adrbrowsiel_100_percent_pack_region, SCALE_FACTOR_100P);
  }
}

}  // namespace ui
