/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "testing/gtest/include/gtest/gtest.h"

#include "adrbrowsiel/components/l10n/browser/locale.h"

// npm run test -- adrbrowsiel_unit_tests --filter=adrbrowsielAds*


namespace ads {

class adrbrowsielAdsLocaleTest : public ::testing::Test {
 protected:
  adrbrowsielAdsLocaleTest() {
    // You can do set-up work for each test here
  }

  ~adrbrowsielAdsLocaleTest() override {
    // You can do clean-up work that doesn't throw exceptions here
  }

  // If the constructor and destructor are not enough for setting up and
  // cleaning up each test, you can use the following methods

  void SetUp() override {
    // Code here will be called immediately after the constructor (right before
    // each test)
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right before the
    // destructor)
  }
};

TEST_F(adrbrowsielAdsLocaleTest,
    LanguageCodeForEnglish) {
  // Arrange
  const std::string locale = "en";

  // Act
  const std::string language_code =
      adrbrowsiel_l10n::Locale::GetLanguageCode(locale);

  // Assert
  const std::string expected_language_code = "en";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(adrbrowsielAdsLocaleTest,
    CountryCodeForEnglish) {
  // Arrange
  const std::string locale = "en";

  // Act
  const std::string country_code =
      adrbrowsiel_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "US";
  EXPECT_EQ(expected_country_code, country_code);
}

TEST_F(adrbrowsielAdsLocaleTest,
    LanguageCodeForLanguageCodeDashCountryCode) {
  // Arrange
  const std::string locale = "en-US";

  // Act
  const std::string language_code =
      adrbrowsiel_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_language_code = "en";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(adrbrowsielAdsLocaleTest,
    CountryCodeForLanguageCodeDashCountryCode) {
  // Arrange
  const std::string locale = "en-US";

  // Act
  const std::string country_code =
      adrbrowsiel_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "US";
  EXPECT_EQ(expected_country_code, country_code);
}

TEST_F(adrbrowsielAdsLocaleTest,
    LanguageCodeForLanguageCodeDashWorld) {
  // Arrange
  const std::string locale = "en-101";

  // Act
  const std::string language_code =
      adrbrowsiel_l10n::Locale::GetLanguageCode(locale);

  // Assert
  const std::string expected_language_code = "en";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(adrbrowsielAdsLocaleTest,
    CountryCodeForLanguageCodeDashWorld) {
  // Arrange
  const std::string locale = "en-101";

  // Act
  const std::string country_code =
      adrbrowsiel_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "101";
  EXPECT_EQ(expected_country_code, country_code);
}

TEST_F(adrbrowsielAdsLocaleTest,
    LanguageCodeForLanguageCodeDashCountryCodeDotEncoding) {
  // Arrange
  const std::string locale = "en-US.UTF-8";

  // Act
  const std::string language_code =
      adrbrowsiel_l10n::Locale::GetLanguageCode(locale);

  // Assert
  const std::string expected_language_code = "en";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(adrbrowsielAdsLocaleTest,
    CountryCodeForLanguageCodeDashCountryCodeDotEncoding) {
  // Arrange
  const std::string locale = "en-US.UTF-8";

  // Act
  const std::string country_code =
      adrbrowsiel_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "US";
  EXPECT_EQ(expected_country_code, country_code);
}

TEST_F(adrbrowsielAdsLocaleTest,
    LanguageCodeForLanguageCodeDashScriptDashCountryCode) {
  // Arrange
  const std::string locale = "az-Latn-AZ";

  // Act
  const std::string language_code =
      adrbrowsiel_l10n::Locale::GetLanguageCode(locale);

  // Assert
  const std::string expected_language_code = "az";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(adrbrowsielAdsLocaleTest,
    CountryCodeForLanguageCodeDashScriptDashCountryCode) {
  // Arrange
  const std::string locale = "az-Latn-AZ";

  // Act
  const std::string country_code =
      adrbrowsiel_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "AZ";
  EXPECT_EQ(expected_country_code, country_code);
}

TEST_F(adrbrowsielAdsLocaleTest,
    LanguageCodeForLanguageCodeUnderscoreCountryCode) {
  // Arrange
  const std::string locale = "en_US";

  // Act
  const std::string language_code =
      adrbrowsiel_l10n::Locale::GetLanguageCode(locale);

  // Assert
  const std::string expected_language_code = "en";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(adrbrowsielAdsLocaleTest,
    CountryCodeForLanguageCodeUnderscoreCountryCode) {
  // Arrange
  const std::string locale = "en_US";

  // Act
  const std::string country_code =
      adrbrowsiel_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "US";
  EXPECT_EQ(expected_country_code, country_code);
}

TEST_F(adrbrowsielAdsLocaleTest,
    LanguageCodeForLanguageCodeUnderscoreCountryCodeDotEncoding) {
  // Arrange
  const std::string locale = "en_US.UTF-8";

  // Act
  const std::string language_code =
      adrbrowsiel_l10n::Locale::GetLanguageCode(locale);

  // Assert
  const std::string expected_language_code = "en";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(adrbrowsielAdsLocaleTest,
    CountryCodeForLanguageCodeUnderscoreCountryCodeDotEncoding) {
  // Arrange
  const std::string locale = "en_US.UTF-8";

  // Act
  const std::string country_code =
      adrbrowsiel_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "US";
  EXPECT_EQ(expected_country_code, country_code);
}

TEST_F(adrbrowsielAdsLocaleTest,
    LanguageCodeForLanguageCodeUnderscoreScriptUnderscoreCountryCode) {
  // Arrange
  const std::string locale = "az_Latn_AZ";

  // Act
  const std::string language_code =
      adrbrowsiel_l10n::Locale::GetLanguageCode(locale);

  // Assert
  const std::string expected_language_code = "az";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(adrbrowsielAdsLocaleTest,
    CountryCodeForLanguageCodeUnderscoreScriptUnderscoreCountryCode) {
  // Arrange
  const std::string locale = "az_Latn_AZ";

  // Act
  const std::string country_code =
      adrbrowsiel_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "AZ";
  EXPECT_EQ(expected_country_code, country_code);
}

}  // namespace ads
