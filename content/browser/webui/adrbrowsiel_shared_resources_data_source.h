/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CONTENT_BROWSER_WEBUI_adrbrowsiel_SHARED_RESOURCES_DATA_SOURCE_H_
#define adrbrowsiel_CONTENT_BROWSER_WEBUI_adrbrowsiel_SHARED_RESOURCES_DATA_SOURCE_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "base/single_thread_task_runner.h"
#include "content/public/browser/url_data_source.h"

namespace adrbrowsiel_content {

// A DataSource for chrome://adrbrowsiel-resources/ URLs.
class adrbrowsielSharedResourcesDataSource : public content::URLDataSource {
 public:
  adrbrowsielSharedResourcesDataSource();
  ~adrbrowsielSharedResourcesDataSource() override;

  // URLDataSource implementation.
  std::string GetSource() override;
  void StartDataRequest(
      const GURL& url,
      const content::WebContents::Getter& wc_getter,
      content::URLDataSource::GotDataCallback callback) override;
  bool AllowCaching() override;
  std::string GetMimeType(const std::string& path) override;
  bool ShouldServeMimeTypeAsContentTypeHeader() override;
  std::string GetAccessControlAllowOriginForOrigin(
      const std::string& origin) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielSharedResourcesDataSource);
};

}  // namespace adrbrowsiel_content

#endif  // adrbrowsiel_CONTENT_BROWSER_WEBUI_adrbrowsiel_SHARED_RESOURCES_DATA_SOURCE_H_
