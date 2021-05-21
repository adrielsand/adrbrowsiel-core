/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_BASE_THREADING_THREAD_RESTRICTIONS_H_
#define adrbrowsiel_CHROMIUM_SRC_BASE_THREADING_THREAD_RESTRICTIONS_H_

class adrbrowsielBrowsingDataRemoverDelegate;

#define adrbrowsiel_SCOPED_ALLOW_BASE_SYNC_PRIMITIVES_H \
  friend class ::adrbrowsielBrowsingDataRemoverDelegate;

#include "../../../../base/threading/thread_restrictions.h"
#undef adrbrowsiel_SCOPED_ALLOW_BASE_SYNC_PRIMITIVES_H

#endif  // adrbrowsiel_CHROMIUM_SRC_BASE_THREADING_THREAD_RESTRICTIONS_H_
