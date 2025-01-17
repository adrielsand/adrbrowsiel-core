/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_P3A_adrbrowsiel_P3A_UPLOADER_H_
#define adrbrowsiel_COMPONENTS_P3A_adrbrowsiel_P3A_UPLOADER_H_

#include <memory>
#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "url/gurl.h"

namespace network {
class SharedURLLoaderFactory;
class SimpleURLLoader;
}  // namespace network

namespace adrbrowsiel {

class adrbrowsielP3AUploader {
 public:
  using UploadCallback = base::RepeatingCallback<void(int, int, bool)>;

  adrbrowsielP3AUploader(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      const GURL& p3a_endpoint,
      const GURL& p2a_endpoint,
      const UploadCallback& on_upload_complete);

  ~adrbrowsielP3AUploader();

  // From metrics::MetricsLogUploader
  void UploadLog(const std::string& compressed_log_data,
                 const std::string& upload_type);

  void OnUploadComplete(std::unique_ptr<std::string> response_body);

 private:
  scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory_;
  const GURL p3a_endpoint_;
  const GURL p2a_endpoint_;
  const UploadCallback on_upload_complete_;
  std::unique_ptr<network::SimpleURLLoader> url_loader_;
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielP3AUploader);
};

}  // namespace adrbrowsiel

#endif  // adrbrowsiel_COMPONENTS_P3A_adrbrowsiel_P3A_UPLOADER_H_
