/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <utility>

#include "bat/ledger/internal/constants.h"
#include "bat/ledger/internal/legacy/media/github.h"
#include "bat/ledger/internal/legacy/static_values.h"
#include "bat/ledger/ledger.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- adrbrowsiel_unit_tests --filter=MediaGitHubTest.*

namespace adrbrowsielledger_media {

class MediaGitHubTest : public testing::Test {
 public:
  static std::string CreateTestJSONString();
};

std::string MediaGitHubTest::CreateTestJSONString() {
  return R"({
    "login": "jdkuki",
    "id": 8422122,
    "avatar_url": "https://avatars0.githubusercontent.com/u/8422122?v=4",
    "url": "https://api.github.com/users/jdkuki",
    "html_url": "https://github.com/jdkuki",
    "name": "Jakob Kuki"
  })";
}

TEST(MediaGitHubTest, GetLinkType) {
  // empty
  std::string result =
      adrbrowsielledger_media::GitHub::GetLinkType("");
  ASSERT_TRUE(result.empty());

  // wrong website
  result = adrbrowsielledger_media::GitHub::GetLinkType("https://twitter.com");
  ASSERT_TRUE(result.empty());

  // correct site
  result = adrbrowsielledger_media::GitHub::GetLinkType("https://github.com");
  ASSERT_EQ(result, GITHUB_MEDIA_TYPE);

  // sub domain
  result = adrbrowsielledger_media::GitHub::GetLinkType("https://gist.github.com");
  ASSERT_EQ(result, GITHUB_MEDIA_TYPE);

  // profile page
  result = adrbrowsielledger_media::GitHub::GetLinkType("https://github.com/jdkuki");
  ASSERT_EQ(result, GITHUB_MEDIA_TYPE);
}

TEST(MediaGitHubTest, GetProfileURL) {
  // empty
  std::string result =
      adrbrowsielledger_media::GitHub::GetProfileURL("");
  ASSERT_TRUE(result.empty());

  result = adrbrowsielledger_media::GitHub::GetProfileURL("jdkuki");
  ASSERT_EQ(result, "https://github.com/jdkuki");
}

TEST(MediaGitHubTest, GetProfileAPIURL) {
  // empty
  std::string result =
      adrbrowsielledger_media::GitHub::GetProfileURL("");
  ASSERT_TRUE(result.empty());

  result = adrbrowsielledger_media::GitHub::GetProfileAPIURL("jdkuki");
  ASSERT_EQ(result, "https://api.github.com/users/jdkuki");
}

TEST(MediaGitHubTest, GetProfileImageURL) {
  // empty
  std::string result =
      adrbrowsielledger_media::GitHub::GetProfileImageURL("");
  ASSERT_TRUE(result.empty());

  std::string test_response = MediaGitHubTest::CreateTestJSONString();

  result = adrbrowsielledger_media::GitHub::GetProfileImageURL(test_response);
  ASSERT_EQ(result, "https://avatars0.githubusercontent.com/u/8422122?v=4");
}

TEST(MediaGitHubTest, GetPublisherKey) {
  // empty
  std::string result =
      adrbrowsielledger_media::GitHub::GetPublisherKey("");
  ASSERT_TRUE(result.empty());

  result =
      adrbrowsielledger_media::GitHub::GetPublisherKey("test_publisher_key");
  ASSERT_EQ(result, "github#channel:test_publisher_key");
}

TEST(MediaGitHubTest, GetUserNameFromURL) {
  // empty
  std::string result =
      adrbrowsielledger_media::GitHub::GetUserNameFromURL("");
  ASSERT_TRUE(result.empty());

  // empty path
  result = adrbrowsielledger_media::GitHub::
      GetUserNameFromURL("/");
  ASSERT_TRUE(result.empty());

  // short path
  result = adrbrowsielledger_media::GitHub::
      GetUserNameFromURL("/jdkuki");
  ASSERT_EQ(result, "jdkuki");

  // long path
  result = adrbrowsielledger_media::GitHub::
      GetUserNameFromURL("/jdkuki/adrbrowsiel-core");
  ASSERT_EQ(result, "jdkuki");
}

TEST(MediaGitHubTest, GetUserName) {
  std::string test_response = MediaGitHubTest::CreateTestJSONString();

  // empty response
  std::string result =
      adrbrowsielledger_media::GitHub::GetUserName("");
  ASSERT_TRUE(result.empty());

  // valid response
  result = adrbrowsielledger_media::GitHub::
      GetUserName(test_response);
  ASSERT_EQ(result, "jdkuki");
}

TEST(MediaGitHubTest, GetUserId) {
  std::string test_response = MediaGitHubTest::CreateTestJSONString();

  // empty
  std::string result =
      adrbrowsielledger_media::GitHub::GetUserId("");
  ASSERT_TRUE(result.empty());

  // incorrect scrape
  result =
      adrbrowsielledger_media::GitHub::GetUserId("Some random text");
  ASSERT_TRUE(result.empty());

  // correct response
  result =
      adrbrowsielledger_media::GitHub::GetUserId(test_response);
  ASSERT_EQ(result, "8422122");
}

TEST(MediaGitHubTest, GetPublisherName) {
  std::string test_response = MediaGitHubTest::CreateTestJSONString();

  // empty
  std::string result =
      adrbrowsielledger_media::GitHub::GetPublisherName("");
  ASSERT_TRUE(result.empty());

  // incorrect scrape
  result = adrbrowsielledger_media::GitHub::
      GetPublisherName("some random text");
  ASSERT_TRUE(result.empty());

  // correct response
  result = adrbrowsielledger_media::GitHub::
      GetPublisherName(test_response);
  ASSERT_EQ(result, "Jakob Kuki");
}

TEST(MediaGitHubTest, GetJSONStringValue) {
  std::string test_response = MediaGitHubTest::CreateTestJSONString();
  std::string result;

  // empty
  bool success =
      adrbrowsielledger_media::GitHub::GetJSONStringValue("login", "", &result);
  ASSERT_FALSE(success);
  ASSERT_TRUE(result.empty());

  // correct response
  success =
      adrbrowsielledger_media::GitHub::GetJSONStringValue("login", test_response,
          &result);
  ASSERT_TRUE(success);
  ASSERT_EQ(result, "jdkuki");
}

TEST(MediaGitHubTest, GetJSONIntValue) {
  std::string test_response = MediaGitHubTest::CreateTestJSONString();
  int64_t result;

  // empty
  bool success =
      adrbrowsielledger_media::GitHub::GetJSONIntValue("id", "", &result);
  ASSERT_FALSE(success);

  // correct response
  success =
      adrbrowsielledger_media::GitHub::GetJSONIntValue("id", test_response, &result);
  ASSERT_TRUE(success);
  ASSERT_EQ(result, 8422122);
}

}  // namespace adrbrowsielledger_media
