/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/os_crypt/key_storage_libsecret.h"

#include "base/command_line.h"

namespace {
const char* GetApplicationName();
}  // namespace

#define adrbrowsiel_KEY_STORAGE_LIBSECRET_GET_KEY_IMPL       \
  if (true) {                                          \
    attrs.Append("application", GetApplicationName()); \
  } else  // NOLINT

#define adrbrowsiel_KEY_STORAGE_LIBSECRET_K_APPLICATION_NAME \
  const char kApplicationName[] = "adrbrowsiel";

#include "../../../../components/os_crypt/key_storage_libsecret.cc"
#undef adrbrowsiel_KEY_STORAGE_LIBSECRET_K_APPLICATION_NAME
#undef adrbrowsiel_KEY_STORAGE_LIBSECRET_GET_KEY_IMPL

namespace {

const char* GetApplicationName() {
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch("import-chrome")) {
    return "chrome";
  } else if (command_line->HasSwitch("import-chromium") ||
             command_line->HasSwitch("import-adrbrowsiel")) {
    return "chromium";
  } else {
    return kApplicationName;
  }
}

}  // namespace
