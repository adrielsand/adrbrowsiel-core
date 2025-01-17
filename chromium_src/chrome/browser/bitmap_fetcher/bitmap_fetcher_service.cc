/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "../../../../../chrome/browser/bitmap_fetcher/bitmap_fetcher_service.cc"

BitmapFetcherService::RequestId BitmapFetcherService::RequestImage(
    const GURL& url,
    BitmapFetchedCallback callback,
    const net::NetworkTrafficAnnotationTag& ta) {
  return RequestImageImpl(url, std::move(callback), ta);
}
