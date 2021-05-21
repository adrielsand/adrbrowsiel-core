/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_COMPONENTS_SYNC_PROTOCOL_PROTO_VISITORS_H_
#define adrbrowsiel_CHROMIUM_SRC_COMPONENTS_SYNC_PROTOCOL_PROTO_VISITORS_H_

#define adrbrowsiel_VISIT_DEVICE_INFO_SPECIFICS_adrbrowsiel_FIELDS \
VISIT(adrbrowsiel_fields);

#define adrbrowsiel_VISIT_PROTO_FIELDS_adrbrowsiel_SPECIFIC_FIELD                  \
VISIT_PROTO_FIELDS(const sync_pb::adrbrowsielSpecificFields& proto) {        \
  VISIT(is_self_delete_supported);                                     \
}

#include "../../../../../components/sync/protocol/proto_visitors.h"

#endif  // adrbrowsiel_CHROMIUM_SRC_COMPONENTS_SYNC_PROTOCOL_PROTO_VISITORS_H_
