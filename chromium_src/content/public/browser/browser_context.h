/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CONTENT_PUBLIC_BROWSER_BROWSER_CONTEXT_H_
#define adrbrowsiel_CHROMIUM_SRC_CONTENT_PUBLIC_BROWSER_BROWSER_CONTEXT_H_

#define IsOffTheRecord \
  IsTor() const;       \
  virtual bool IsOffTheRecord
#include "../../../../../content/public/browser/browser_context.h"
#undef IsOffTheRecord

#include <string>

#include "base/memory/ref_counted.h"
#include "base/optional.h"
#include "content/common/content_export.h"
#include "content/public/browser/tld_ephemeral_lifetime.h"

namespace content {

class WebContents;
class SessionStorageNamespace;
class StoragePartition;

CONTENT_EXPORT scoped_refptr<content::SessionStorageNamespace>
CreateSessionStorageNamespace(
    content::StoragePartition* partition,
    const std::string& namespace_id,
    base::Optional<std::string> clone_from_namespace_id);

CONTENT_EXPORT std::string GetSessionStorageNamespaceId(WebContents*);

}  // namespace content

#endif  // adrbrowsiel_CHROMIUM_SRC_CONTENT_PUBLIC_BROWSER_BROWSER_CONTEXT_H_
