/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define adrbrowsiel_STATEMENT_TO_FORMS \
    if (result == ENCRYPTION_RESULT_SERVICE_FAILURE) \
      result = ENCRYPTION_RESULT_ITEM_FAILURE;

#include "../../../../../../components/password_manager/core/browser/login_database.cc"

#undef adrbrowsiel_STATEMENT_TO_FORMS
