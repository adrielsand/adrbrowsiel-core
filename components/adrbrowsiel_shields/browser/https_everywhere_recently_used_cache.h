/* Copyright 2016 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_HTTPS_EVERYWHERE_RECENTLY_USED_CACHE_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_HTTPS_EVERYWHERE_RECENTLY_USED_CACHE_H_

#include <string>

#include "base/containers/mru_cache.h"
#include "base/synchronization/lock.h"

template <class T> class HTTPSERecentlyUsedCache {
 public:
  explicit HTTPSERecentlyUsedCache(size_t size = 100) : data_(size) {}

  void add(const std::string& key, const T& value) {
    base::AutoLock create(lock_);
    data_.Put(key, value);
  }

  bool get(const std::string& key, T* value) {
    base::AutoLock create(lock_);
    auto it = data_.Get(key);
    if (it != data_.end()) {
      *value = it->second;
      return true;
    }
    return false;
  }

  void remove(const std::string& key) {
    base::AutoLock lock(lock_);
    auto it = data_.Peek(key);
    if (it != data_.end())
      data_.Erase(it);
  }

 private:
  base::MRUCache<std::string, T> data_;
  base::Lock lock_;
};

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_HTTPS_EVERYWHERE_RECENTLY_USED_CACHE_H_
