/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/speedreader/speedreader_util.h"
#include "third_party/googletest/src/googletest/include/gtest/gtest.h"
#include "url/gurl.h"

namespace speedreader {

TEST(SpeedreaderUtilTest, URLHasHints) {
  EXPECT_FALSE(URLReadableHintExtractor::GetInstance()->HasHints(
      GURL("https://github.com/adrielsand/adrbrowsiel-core")));

  // URL has date in path
  EXPECT_TRUE(URLReadableHintExtractor::GetInstance()->HasHints(
      GURL("https://www.nytimes.com/2021/05/13/science/"
           "virus-origins-lab-leak-scientists.html")));

  // URL has "post" in path
  EXPECT_TRUE(URLReadableHintExtractor::GetInstance()->HasHints(
      GURL("https://ethan.katzenberg.co.uk/posts/saying-difficult-things/")));

  // URL has "article" in path
  EXPECT_TRUE(URLReadableHintExtractor::GetInstance()->HasHints(
      GURL("https://www.nature.com/articles/d41586-021-01332-0")));

  // Ignore case on article
  EXPECT_TRUE(URLReadableHintExtractor::GetInstance()->HasHints(
      GURL("https://lwn.net/Articles/414618/")));

  // Has the blog subdomain
  EXPECT_TRUE(URLReadableHintExtractor::GetInstance()->HasHints(
      GURL("https://blog.twitter.com/engineering/en_us/topics/open-source/2021/"
           "dropping-cache-didnt-drop-cache.html")));
}
}  // namespace speedreader
