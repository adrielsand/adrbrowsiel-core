/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <utility>

#include "bat/ledger/internal/legacy/media/reddit.h"
#include "bat/ledger/internal/constants.h"
#include "bat/ledger/ledger.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- adrbrowsiel_unit_tests --filter=MediaRedditTest.*

namespace adrbrowsielledger_media {

class MediaRedditTest : public testing::Test {
};

TEST(MediaRedditTest, GetProfileUrl) {
  // empty
  std::string result =
      adrbrowsielledger_media::Reddit::GetProfileUrl(std::string());
  ASSERT_TRUE(result.empty());

  result = adrbrowsielledger_media::Reddit::GetProfileUrl("jsadler-adrbrowsiel");
  ASSERT_EQ(result, "https://reddit.com/user/jsadler-adrbrowsiel/");
}

TEST(MediaRedditTest, GetProfileImageUrl) {
  // empty
  std::string result =
      adrbrowsielledger_media::Reddit::GetProfileImageUrl(std::string());
  ASSERT_TRUE(result.empty());

  result = adrbrowsielledger_media::Reddit::GetProfileImageUrl(
      "\"accountIcon\":\"https://www.someredditmediacdn.com/somephoto.png?"
      "somequerystringparams");
  ASSERT_EQ(result, "https://www.someredditmediacdn.com/somephoto.png");
}

TEST(MediaRedditTest, GetPublisherKey) {
  // empty
  std::string result =
      adrbrowsielledger_media::Reddit::GetPublisherKey(std::string());
  ASSERT_TRUE(result.empty());

  result =
      adrbrowsielledger_media::Reddit::GetPublisherKey("test_publisher_key");
  ASSERT_EQ(result, "reddit#channel:test_publisher_key");
}

TEST(MediaRedditTest, GetUserNameFromUrl) {
  // empty
  std::string result =
      adrbrowsielledger_media::Reddit::GetUserNameFromUrl(std::string());
  ASSERT_TRUE(result.empty());

  // empty path
  result = adrbrowsielledger_media::Reddit::
      GetUserNameFromUrl("/");
  ASSERT_TRUE(result.empty());

  // simple path
  result = adrbrowsielledger_media::Reddit::
      GetUserNameFromUrl("/jsadler-adrbrowsiel");
  ASSERT_TRUE(result.empty());

  // long path
  result = adrbrowsielledger_media::Reddit::
      GetUserNameFromUrl("/user/jsadler-adrbrowsiel");
  ASSERT_EQ(result, "jsadler-adrbrowsiel");
}

TEST(MediaRedditTest, GetUserId) {
  const char reddit_new[] =
      "\"hideFromRobots\":false,\"id\":\"t2_78910\"";
  const char reddit_old[] =
      "\"target_fullname\": \"t2_123456\"";

  // empty
  std::string result =
      adrbrowsielledger_media::Reddit::GetUserId(std::string());
  ASSERT_TRUE(result.empty());

  // incorrect scrape
  result =
      adrbrowsielledger_media::Reddit::GetUserId("Some random text");
  ASSERT_TRUE(result.empty());

  // support for current Reddit
  result =
      adrbrowsielledger_media::Reddit::GetUserId(reddit_old);
  ASSERT_EQ(result, "123456");

  // support for new Reddit
  result =
      adrbrowsielledger_media::Reddit::GetUserId(reddit_new);
  ASSERT_EQ(result, "78910");
}

TEST(MediaRedditTest, GetPublisherName) {
  const char reddit_new[] = "\"username\":\"jsadler-adrbrowsiel\"";
  const char reddit_old[] = "\"target_name\": \"jsadler-adrbrowsiel\"";
  // empty
  std::string result =
      adrbrowsielledger_media::Reddit::GetPublisherName(std::string());
  ASSERT_TRUE(result.empty());

  // incorrect scrape
  result = adrbrowsielledger_media::Reddit::
      GetPublisherName("some random text");
  ASSERT_TRUE(result.empty());

  // current reddit
  result = adrbrowsielledger_media::Reddit::
      GetPublisherName(reddit_new);
  ASSERT_EQ(result, "jsadler-adrbrowsiel");

  // old reddit
  result = adrbrowsielledger_media::Reddit::
      GetPublisherName(reddit_old);
  ASSERT_EQ(result, "jsadler-adrbrowsiel");
}

}  // namespace adrbrowsielledger_media
