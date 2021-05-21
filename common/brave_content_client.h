/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMMON_adrbrowsiel_CONTENT_CLIENT_H_
#define adrbrowsiel_COMMON_adrbrowsiel_CONTENT_CLIENT_H_

#include <vector>

#include "chrome/common/chrome_content_client.h"

class adrbrowsielContentClient : public ChromeContentClient {
 public:
  adrbrowsielContentClient();
  ~adrbrowsielContentClient() override;

 private:
  // ChromeContentClinet overrides:
  base::RefCountedMemory* GetDataResourceBytes(int resource_id) override;
  void AddAdditionalSchemes(Schemes* schemes) override;
  void AddContentDecryptionModules(
      std::vector<content::CdmInfo>* cdms,
      std::vector<media::CdmHostFilePath>* cdm_host_file_paths) override;
};

#endif  // adrbrowsiel_COMMON_adrbrowsiel_CONTENT_CLIENT_H_
