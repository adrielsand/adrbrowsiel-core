/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsielLEDGER_COMMON_SECURITY_UTIL_H_
#define adrbrowsielLEDGER_COMMON_SECURITY_UTIL_H_

#include <map>
#include <string>
#include <vector>

namespace ledger {
namespace util {

class Security {
 public:
  static std::vector<uint8_t> GetSHA256(const std::string& string);

  static std::string GetBase64(const std::vector<uint8_t>& data);

  static std::string Sign(
      const std::vector<std::map<std::string, std::string>>& headers,
      const std::string& key_id,
      const std::vector<uint8_t>& private_key);

  static std::vector<uint8_t> GenerateSeed();

  static std::string Uint8ToHex(const std::vector<uint8_t>& in);

  static bool GetPublicKeyFromSeed(
      const std::vector<uint8_t>& seed,
      std::vector<uint8_t>* public_key,
      std::vector<uint8_t>* secret_key);

  static std::vector<uint8_t> GetHKDF(const std::vector<uint8_t>& seed);

  static bool IsSeedValid(const std::vector<uint8_t>& seed);

  static std::string DigestValue(const std::string& body);

  static std::string GetPublicKeyHexFromSeed(const std::vector<uint8_t>& seed);
};

}  // namespace util
}  // namespace ledger

#endif  // adrbrowsielLEDGER_COMMON_SECURITY_UTIL_H_
