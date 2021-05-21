/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/toolbar/adrbrowsiel_location_bar_model_delegate.h"

#include "base/strings/utf_string_conversions.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "extensions/buildflags/buildflags.h"
#include "testing/gtest/include/gtest/gtest.h"

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_constants.h"
#endif

using adrbrowsielLocationBarModelDelegateTest = testing::Test;

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
TEST_F(adrbrowsielLocationBarModelDelegateTest, ResolvesEthereumRemoteClient) {
  GURL url(ethereum_remote_client_base_url);
  std::u16string formatted_url = base::UTF8ToUTF16(url.spec());
  adrbrowsielLocationBarModelDelegate::FormattedStringFromURL(url, &formatted_url);
  ASSERT_STREQ(base::UTF16ToASCII(formatted_url).c_str(), "adrbrowsiel://wallet");
}
TEST_F(adrbrowsielLocationBarModelDelegateTest,
    ResolvesEthereumRemoteClientPhishingRoute) {
  GURL url(ethereum_remote_client_phishing_url);
  std::u16string formatted_url = base::UTF8ToUTF16(url.spec());
  adrbrowsielLocationBarModelDelegate::FormattedStringFromURL(url, &formatted_url);
  ASSERT_STREQ(base::UTF16ToASCII(formatted_url).c_str(), "adrbrowsiel://wallet");
}
TEST_F(adrbrowsielLocationBarModelDelegateTest,
    ResolvesEthereumRemoteClientENSRoute) {
  GURL url(ethereum_remote_client_ens_redirect_url);
  std::u16string formatted_url = base::UTF8ToUTF16(url.spec());
  adrbrowsielLocationBarModelDelegate::FormattedStringFromURL(url, &formatted_url);
  ASSERT_STREQ(base::UTF16ToASCII(formatted_url).c_str(), "adrbrowsiel://wallet");
}
#endif

TEST_F(adrbrowsielLocationBarModelDelegateTest, ResolvesChromeSchemeToadrbrowsiel) {
  GURL url("chrome://sync/");
  std::u16string formatted_url = base::UTF8ToUTF16(url.spec());
  adrbrowsielLocationBarModelDelegate::FormattedStringFromURL(url, &formatted_url);
  ASSERT_STREQ(base::UTF16ToASCII(formatted_url).c_str(), "adrbrowsiel://sync/");
}
