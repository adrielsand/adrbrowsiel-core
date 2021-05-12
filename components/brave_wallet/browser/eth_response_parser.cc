/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_wallet/browser/eth_response_parser.h"

#include <utility>

#include "base/json/json_reader.h"
#include "base/logging.h"
#include "brave/components/brave_wallet/browser/brave_wallet_utils.h"

namespace {

bool ParseSingleStringResult(const std::string& json, std::string* result) {
  DCHECK(result);
  base::JSONReader::ValueWithError value_with_error =
      base::JSONReader::ReadAndReturnValueWithError(
          json, base::JSONParserOptions::JSON_PARSE_RFC);
  base::Optional<base::Value>& records_v = value_with_error.value;
  if (!records_v) {
    LOG(ERROR) << "Invalid response, could not parse JSON, JSON is: " << json;
    return false;
  }

  const base::DictionaryValue* response_dict;
  if (!records_v->GetAsDictionary(&response_dict)) {
    return false;
  }

  return response_dict->GetString("result", result);
}

bool ParseDictionaryResult(const std::string& json,
                           const base::DictionaryValue* dict) {
  base::JSONReader::ValueWithError value_with_error =
      base::JSONReader::ReadAndReturnValueWithError(
          json, base::JSONParserOptions::JSON_PARSE_RFC);
  base::Optional<base::Value>& records_v = value_with_error.value;
  if (!records_v) {
    LOG(ERROR) << "Invalid response, could not parse JSON, JSON is: " << json;
    return false;
  }

  const base::DictionaryValue* response_dict;
  if (!records_v->GetAsDictionary(&response_dict)) {
    return false;
  }

  const base::Value* result = response_dict->FindPath("result");
  if (!result)
    return false;

  return result->GetAsDictionary(&dict);
}

}  // namespace

namespace brave_wallet {

bool ParseEthGetBalance(const std::string& json, std::string* hex_balance) {
  return ParseSingleStringResult(json, hex_balance);
}

bool ParseEthGetTransactionCount(const std::string& json, uint256_t* count) {
  std::string count_str;
  if (!ParseSingleStringResult(json, &count_str))
    return false;

  if (!HexValueToUint256(count_str, count))
    return false;

  return true;
}

bool ParseEthGetTransactionReceipt(const std::string& json,
                                   TransactionReceipt* receipt) {
  DCHECK(receipt);
  const base::DictionaryValue* result = nullptr;
  if (!ParseDictionaryResult(json, result))
    return false;
  DCHECK(result);

  if (!result->GetString("transactionHash", &receipt->transaction_hash))
    return false;

  std::string transaction_index;
  if (!result->GetString("transactionIndex", &transaction_index))
    return false;
  if (!HexValueToUint256(transaction_index, &receipt->transaction_index))
    return false;

  std::string block_number;
  if (!result->GetString("blockNumber", &block_number))
    return false;
  if (!HexValueToUint256(block_number, &receipt->block_number))
    return false;

  if (!result->GetString("blockHash", &receipt->block_hash))
    return false;

  std::string cumulative_gas_used;
  if (!result->GetString("cumulativeGasUsed", &cumulative_gas_used))
    return false;
  if (!HexValueToUint256(cumulative_gas_used, &receipt->cumulative_gas_used))
    return false;

  std::string gas_used;
  if (!result->GetString("gas_used", &gas_used))
    return false;
  if (!HexValueToUint256(gas_used, &receipt->gas_used))
    return false;

  if (!result->GetString("contractAddress", &receipt->contract_address))
    return false;

  // TODO(darkdh): logs
#if 0
  const base::ListValue* logs = nullptr;
  if (!result->GetList("logs", &logs))
    return false;
  for (const std::string& entry : logs->GetList())
    receipt->logs.push_back(entry);
#endif
  
  if (!result->GetString("logsBloom", &receipt->logs_bloom))
    return false;
 
  std::string status;
  if (!result->GetString("status", &status))
    return false;
  if (status == "0x01")
    receipt->status = true;
  else if (status == "0x00")
    receipt->status = false;


  return true;
}

bool ParseEthCall(const std::string& json, std::string* result) {
  return ParseSingleStringResult(json, result);
}

}  // namespace brave_wallet
