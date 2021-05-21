/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/extensions/api/adrbrowsiel_wallet_api.h"

#include <memory>
#include <string>

#include "base/json/json_writer.h"
#include "base/values.h"
#include "adrbrowsiel/browser/adrbrowsiel_wallet/adrbrowsiel_wallet_service_factory.h"
#include "adrbrowsiel/browser/extensions/adrbrowsiel_wallet_util.h"
#include "adrbrowsiel/browser/infobars/crypto_wallets_infobar_delegate.h"
#include "adrbrowsiel/common/extensions/api/adrbrowsiel_wallet.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_constants.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_service.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_utils.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/pref_names.h"
#include "adrbrowsiel/grit/adrbrowsiel_generated_resources.h"
#include "chrome/browser/extensions/api/tabs/tabs_constants.h"
#include "chrome/browser/extensions/extension_tab_util.h"
#include "chrome/browser/infobars/infobar_service.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_contents.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_util.h"
#include "extensions/common/constants.h"
#include "ui/base/l10n/l10n_util.h"

namespace {

adrbrowsielWalletService* GetadrbrowsielWalletService(
    content::BrowserContext* context) {
  return adrbrowsielWalletServiceFactory::GetInstance()->GetForContext(context);
}

base::Value MakeSelectValue(const std::u16string& name,
                            adrbrowsielWalletWeb3ProviderTypes value) {
  base::Value item(base::Value::Type::DICTIONARY);
  item.SetKey("value", base::Value(static_cast<int>(value)));
  item.SetKey("name", base::Value(name));
  return item;
}

}  // namespace

namespace extensions {
namespace api {

ExtensionFunction::ResponseAction
adrbrowsielWalletPromptToEnableWalletFunction::Run() {
  std::unique_ptr<adrbrowsiel_wallet::PromptToEnableWallet::Params> params(
      adrbrowsiel_wallet::PromptToEnableWallet::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  if (browser_context()->IsTor()) {
    return RespondNow(Error("Not available in Tor context"));
  }

  // Get web contents for this tab
  content::WebContents* contents = nullptr;
  if (!ExtensionTabUtil::GetTabById(
        params->tab_id,
        Profile::FromBrowserContext(browser_context()),
        include_incognito_information(),
        nullptr,
        nullptr,
        &contents,
        nullptr)) {
    return RespondNow(Error(tabs_constants::kTabNotFoundError,
                            base::NumberToString(params->tab_id)));
  }

  InfoBarService* infobar_service =
      InfoBarService::FromWebContents(contents);
  if (infobar_service) {
    CryptoWalletsInfoBarDelegate::InfobarSubType subtype =
        CryptoWalletsInfoBarDelegate::InfobarSubType::GENERIC_SETUP;
    auto* service = GetadrbrowsielWalletService(browser_context());
    if (service->ShouldShowLazyLoadInfobar()) {
      subtype = CryptoWalletsInfoBarDelegate::InfobarSubType::
          LOAD_CRYPTO_WALLETS;
    }
    CryptoWalletsInfoBarDelegate::Create(infobar_service, subtype);
  }

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
adrbrowsielWalletReadyFunction::Run() {
  if (browser_context()->IsTor()) {
    return RespondNow(Error("Not available in Tor context"));
  }

  auto* service = GetadrbrowsielWalletService(browser_context());
  service->CryptoWalletsExtensionReady();
  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
adrbrowsielWalletLoadUIFunction::Run() {
  auto* service = GetadrbrowsielWalletService(browser_context());
  // If the extension is already ready, respond right away
  if (service->IsCryptoWalletsReady()) {
    return RespondNow(NoArguments());
  }

  // If the user has opt-ed in and MetaMask is not installed, and
  // the new adrbrowsiel Wallet is not the default, then
  // set the Dapp provider to Crypto Wallets.
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto provider = static_cast<adrbrowsielWalletWeb3ProviderTypes>(
      profile->GetPrefs()->GetInteger(kadrbrowsielWalletWeb3Provider));
  auto* registry = extensions::ExtensionRegistry::Get(profile);
  if (!registry->ready_extensions().Contains(metamask_extension_id) &&
      provider != adrbrowsielWalletWeb3ProviderTypes::adrbrowsiel_WALLET) {
    profile->GetPrefs()->SetInteger(kadrbrowsielWalletWeb3Provider,
        static_cast<int>(adrbrowsielWalletWeb3ProviderTypes::CRYPTO_WALLETS));
  }
  profile->GetPrefs()->SetBoolean(kOptedIntoCryptoWallets, true);
  service->MaybeLoadCryptoWalletsExtension(
      base::BindOnce(&adrbrowsielWalletLoadUIFunction::OnLoaded, this));
  return RespondLater();
}

void adrbrowsielWalletLoadUIFunction::OnLoaded() {
  Respond(NoArguments());
}

ExtensionFunction::ResponseAction
adrbrowsielWalletShouldPromptForSetupFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* service = GetadrbrowsielWalletService(browser_context());
  bool should_prompt = !service->IsCryptoWalletsSetup() &&
      !profile->GetPrefs()->GetBoolean(kOptedIntoCryptoWallets);
  return RespondNow(OneArgument(base::Value(should_prompt)));
}

ExtensionFunction::ResponseAction
adrbrowsielWalletShouldCheckForDappsFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  if (browser_context()->IsTor()) {
    return RespondNow(OneArgument(base::Value(false)));
  }
  auto provider = static_cast<adrbrowsielWalletWeb3ProviderTypes>(
      profile->GetPrefs()->GetInteger(kadrbrowsielWalletWeb3Provider));
  if (provider == adrbrowsielWalletWeb3ProviderTypes::adrbrowsiel_WALLET) {
    return RespondNow(OneArgument(base::Value(false)));
  }
  auto* registry = extensions::ExtensionRegistry::Get(profile);
  bool has_metamask =
      registry->ready_extensions().Contains(metamask_extension_id);

  auto* service = GetadrbrowsielWalletService(browser_context());
  bool dappDetection = (
      provider == adrbrowsielWalletWeb3ProviderTypes::ASK && !has_metamask) ||
      (provider == adrbrowsielWalletWeb3ProviderTypes::CRYPTO_WALLETS &&
       !service->IsCryptoWalletsReady());

  return RespondNow(OneArgument(base::Value(dappDetection)));
}

ExtensionFunction::ResponseAction
adrbrowsielWalletGetWalletSeedFunction::Run() {
  // make sure the passed in enryption key is 32 bytes.
  std::unique_ptr<adrbrowsiel_wallet::GetWalletSeed::Params> params(
    adrbrowsiel_wallet::GetWalletSeed::Params::Create(*args_));
  if (params->key.size() != 32) {
    return RespondNow(Error("Invalid input key size"));
  }

  auto* service = GetadrbrowsielWalletService(browser_context());

  base::Value::BlobStorage blob;
  std::string derived = service->GetWalletSeed(params->key);

  if (derived.empty()) {
    return RespondNow(Error("Error getting wallet seed"));
  }

  blob.assign(derived.begin(), derived.end());

  return RespondNow(OneArgument(base::Value(blob)));
}

ExtensionFunction::ResponseAction
adrbrowsielWalletGetBitGoSeedFunction::Run() {
  // make sure the passed in enryption key is 32 bytes.
  std::unique_ptr<adrbrowsiel_wallet::GetBitGoSeed::Params> params(
    adrbrowsiel_wallet::GetBitGoSeed::Params::Create(*args_));
  if (params->key.size() != 32) {
    return RespondNow(Error("Invalid input key size"));
  }

  auto* service = GetadrbrowsielWalletService(browser_context());

  base::Value::BlobStorage blob;
  std::string derived = service->GetBitGoSeed(params->key);

  if (derived.empty()) {
    return RespondNow(Error("Error getting wallet seed"));
  }

  blob.assign(derived.begin(), derived.end());

  return RespondNow(OneArgument(base::Value(blob)));
}

ExtensionFunction::ResponseAction
adrbrowsielWalletGetProjectIDFunction::Run() {
  std::string project_id = extensions::GetInfuraProjectID();
  return RespondNow(OneArgument(base::Value(project_id)));
}

ExtensionFunction::ResponseAction
adrbrowsielWalletGetadrbrowsielKeyFunction::Run() {
  std::string adrbrowsiel_key = extensions::GetadrbrowsielKey();
  return RespondNow(OneArgument(base::Value(adrbrowsiel_key)));
}

ExtensionFunction::ResponseAction
adrbrowsielWalletResetWalletFunction::Run() {
  auto* service = GetadrbrowsielWalletService(browser_context());
  service->ResetCryptoWallets();
  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
adrbrowsielWalletGetWeb3ProviderFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto provider = static_cast<adrbrowsielWalletWeb3ProviderTypes>(
      profile->GetPrefs()->GetInteger(kadrbrowsielWalletWeb3Provider));
  std::string extension_id;
  if (provider == adrbrowsielWalletWeb3ProviderTypes::adrbrowsiel_WALLET) {
    // This API is used so an extension can know when to prompt to
    // be the default Dapp provider. Since the new wallet is not an
    // extension at all, we can just re-use the Crypto Wallets ID.
    // We also don't want to prompt in Crypto Wallets when it's set
    // to adrbrowsiel Wallet.
    extension_id = ethereum_remote_client_extension_id;
  } else if (provider == adrbrowsielWalletWeb3ProviderTypes::CRYPTO_WALLETS) {
    extension_id = ethereum_remote_client_extension_id;
  } else if (provider == adrbrowsielWalletWeb3ProviderTypes::METAMASK) {
    extension_id = metamask_extension_id;
  }
  return RespondNow(OneArgument(base::Value(extension_id)));
}

ExtensionFunction::ResponseAction
adrbrowsielWalletGetWeb3ProviderListFunction::Run() {
  base::Value list(base::Value::Type::LIST);
  // There is no Ask mode in the new wallet flow, instead it is
  // just defaulted to the new wallet since there is no overhead.
  bool new_wallet = ::adrbrowsiel_wallet::IsNativeWalletEnabled();
  if (new_wallet) {
    list.Append(MakeSelectValue(
        l10n_util::GetStringUTF16(IDS_adrbrowsiel_WALLET_WEB3_PROVIDER_adrbrowsiel),
        adrbrowsielWalletWeb3ProviderTypes::adrbrowsiel_WALLET));
  } else {
    list.Append(MakeSelectValue(
        l10n_util::GetStringUTF16(IDS_adrbrowsiel_WALLET_WEB3_PROVIDER_ASK),
        adrbrowsielWalletWeb3ProviderTypes::ASK));
  }
  list.Append(MakeSelectValue(
      l10n_util::GetStringUTF16(
          new_wallet ? IDS_adrbrowsiel_WALLET_WEB3_PROVIDER_CRYPTO_WALLETS_DEPRECATED
                     : IDS_adrbrowsiel_WALLET_WEB3_PROVIDER_CRYPTO_WALLETS),
      adrbrowsielWalletWeb3ProviderTypes::CRYPTO_WALLETS));
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* registry = extensions::ExtensionRegistry::Get(profile);
  if (registry->ready_extensions().Contains(metamask_extension_id)) {
    list.Append(MakeSelectValue(
        l10n_util::GetStringUTF16(IDS_adrbrowsiel_WALLET_WEB3_PROVIDER_METAMASK),
        adrbrowsielWalletWeb3ProviderTypes::METAMASK));
  }
  list.Append(MakeSelectValue(
      l10n_util::GetStringUTF16(IDS_adrbrowsiel_WALLET_WEB3_PROVIDER_NONE),
      adrbrowsielWalletWeb3ProviderTypes::NONE));
  std::string json_string;
  base::JSONWriter::Write(list, &json_string);
  return RespondNow(OneArgument(base::Value(json_string)));
}

ExtensionFunction::ResponseAction
adrbrowsielWalletIsNativeWalletEnabledFunction::Run() {
  return RespondNow(
      OneArgument(base::Value(::adrbrowsiel_wallet::IsNativeWalletEnabled())));
}

}  // namespace api
}  // namespace extensions
