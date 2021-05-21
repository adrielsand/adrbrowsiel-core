/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_NUMBER_UTIL_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_NUMBER_UTIL_H_

namespace ads {

bool DoubleEquals(const double lhs, const double rhs);
bool DoubleIsGreaterEqual(const double lhs, const double rhs);
bool DoubleIsGreater(const double lhs, const double rhs);
bool DoubleIsLessEqual(const double lhs, const double rhs);
bool DoubleIsLess(const double lhs, const double rhs);

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_NUMBER_UTIL_H_
