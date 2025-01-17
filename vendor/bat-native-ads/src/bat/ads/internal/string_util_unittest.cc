/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/string_util.h"

#include <string>

#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- adrbrowsiel_unit_tests --filter=BatAds*

namespace ads {

TEST(BatAdsStringUtilTest, StripNonAlphaCharactersFromEmptyContent) {
  // Arrange
  const std::string content = "";

  // Act
  const std::string stripped_content = StripNonAlphaCharacters(content);

  // Assert
  const std::string expected_stripped_content = "";

  EXPECT_EQ(expected_stripped_content, stripped_content);
}

TEST(BatAdsStringUtilTest, StripNonAlphaCharactersFromWhitespace) {
  // Arrange
  const std::string content = "   ";

  // Act
  const std::string stripped_content = StripNonAlphaCharacters(content);

  // Assert
  const std::string expected_stripped_content = "";

  EXPECT_EQ(expected_stripped_content, stripped_content);
}

TEST(BatAdsStringUtilTest, StripNonAlphaCharacters) {
  // Arrange
  const std::string content =
      "  The quick brown fox jumps over the lazy dog. "
      "$123,000.0 !\"#$%&'()*+,-./:<=>?@\\[]^_`{|}~ 0123456789 \t\n\v\f\r "
      "0x7F x123x a1b2c3 Les naïfs ægithales hâtifs pondant à Noël où il "
      "gèle sont sûrs d'être déçus en voyant leurs drôles d'œufs abîmés. "
      "Falsches Üben von Xylophonmusik quält jeden größeren Zwerg. ξεσκεπάζω "
      "την ψυχοφθόρα \\t\\n\\v\\f\\r βδελυγμία. いろはにほへど　ちりぬるを "
      "わがよたれぞ　つねならむ うゐのおくやま　けふこえて あさきゆめみじ　"
      "ゑひもせず  ";  // The Quick Brown Fox... Pangrams

  // Act
  const std::string stripped_content = StripNonAlphaCharacters(content);

  // Assert
  const std::string expected_stripped_content =
      "The quick brown fox jumps over the lazy dog Les naïfs ægithales hâtifs "
      "pondant à Noël où il gèle sont sûrs d être déçus en voyant leurs drôles "
      "d œufs abîmés Falsches Üben von Xylophonmusik quält jeden größeren "
      "Zwerg ξεσκεπάζω την ψυχοφθόρα βδελυγμία いろはにほへど ちりぬるを "
      "わがよたれぞ つねならむ うゐのおくやま けふこえて あさきゆめみじ "
      "ゑひもせず";

  EXPECT_EQ(expected_stripped_content, stripped_content);
}

TEST(BatAdsStringUtilTest, StripNonAlphaNumericCharactersFromEmptyContent) {
  // Arrange
  const std::string content = "";

  // Act
  const std::string stripped_content = StripNonAlphaNumericCharacters(content);

  // Assert
  const std::string expected_stripped_content = "";

  EXPECT_EQ(expected_stripped_content, stripped_content);
}

TEST(BatAdsStringUtilTest, StripNonAlphaNumericCharactersFromWhitespace) {
  // Arrange
  const std::string content = "   ";

  // Act
  const std::string stripped_content = StripNonAlphaNumericCharacters(content);

  // Assert
  const std::string expected_stripped_content = "";

  EXPECT_EQ(expected_stripped_content, stripped_content);
}

TEST(BatAdsStringUtilTest, StripNonAlphaNumericCharacters) {
  // Arrange
  const std::string content =
      "  The quick brown fox jumps over the lazy dog. "
      "$123,000.0 !\"#$%&'()*+,-./:<=>?@\\[]^_`{|}~ 0123456789 \t\n\v\f\r "
      "0x7F x123x a1b2c3 Les naïfs ægithales hâtifs pondant à Noël où il "
      "gèle sont sûrs d'être déçus en voyant leurs drôles d'œufs abîmés. "
      "Falsches Üben von Xylophonmusik quält jeden größeren Zwerg. ξεσκεπάζω "
      "την ψυχοφθόρα \\t\\n\\v\\f\\r βδελυγμία. いろはにほへど　ちりぬるを "
      "わがよたれぞ　つねならむ うゐのおくやま　けふこえて あさきゆめみじ　"
      "ゑひもせず  ";  // The Quick Brown Fox... Pangrams

  // Act
  const std::string stripped_content = StripNonAlphaNumericCharacters(content);

  // Assert
  const std::string expected_stripped_content =
      "The quick brown fox jumps over the lazy dog 123 000 0 0123456789 0x7F "
      "x123x a1b2c3 Les naïfs ægithales hâtifs pondant à Noël où il gèle sont "
      "sûrs d être déçus en voyant leurs drôles d œufs abîmés Falsches Üben "
      "von Xylophonmusik quält jeden größeren Zwerg ξεσκεπάζω την ψυχοφθόρα "
      "βδελυγμία いろはにほへど ちりぬるを わがよたれぞ つねならむ "
      "うゐのおくやま けふこえて あさきゆめみじ ゑひもせず";

  EXPECT_EQ(expected_stripped_content, stripped_content);
}

TEST(BatAdsStringUtilTest, IsLatinAlphaNumericForEmptyValue) {
  // Arrange
  const std::string value = "";

  // Act
  const bool is_alpha_numeric = IsLatinAlphaNumeric(value);

  // Assert
  EXPECT_EQ(true, is_alpha_numeric);
}

TEST(BatAdsStringUtilTest, IsLatinAlphaNumericForNonAlphaNumericValue) {
  // Arrange
  const std::string value = "abc123 ";

  // Act
  const bool is_alpha_numeric = IsLatinAlphaNumeric(value);

  // Assert
  EXPECT_EQ(false, is_alpha_numeric);
}

TEST(BatAdsStringUtilTest, IsLatinAlphaNumeric) {
  // Arrange
  const std::string value =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

  // Act
  const bool is_alpha_numeric = IsLatinAlphaNumeric(value);

  // Assert
  EXPECT_EQ(true, is_alpha_numeric);
}

}  // namespace ads
