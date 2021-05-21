/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_RENDERER_adrbrowsiel_WALLET_JS_HANDLER_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_RENDERER_adrbrowsiel_WALLET_JS_HANDLER_H_

#include <memory>
#include <string>

#include "adrbrowsiel/components/adrbrowsiel_wallet/common/adrbrowsiel_wallet.mojom.h"
#include "content/public/renderer/render_frame.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "v8/include/v8.h"

namespace adrbrowsiel_wallet {

class adrbrowsielWalletJSHandler {
 public:
  explicit adrbrowsielWalletJSHandler(content::RenderFrame* render_frame);
  ~adrbrowsielWalletJSHandler();

  void AddJavaScriptObjectToFrame(v8::Local<v8::Context> context);
  void FireEvent(const std::string& event, const std::string& event_args);
  void ConnectEvent(const std::string& chain_id);
  void DisconnectEvent(const std::string& message);
  void ChainChangedEvent(const std::string& chain_id);
  void AccountsChangedEvent(const std::string& accounts);

 private:
  void BindFunctionsToObject(v8::Isolate* isolate,
                             v8::Local<v8::Context> context,
                             v8::Local<v8::Object> javascript_object);

  // Adds a function to the provided object.
  template <typename Sig>
  void BindFunctionToObject(v8::Isolate* isolate,
                            v8::Local<v8::Object> javascript_object,
                            const std::string& name,
                            const base::RepeatingCallback<Sig>& callback);
  void CreateEthereumObject(v8::Isolate* isolate,
                            v8::Local<v8::Context> context);
  bool EnsureConnected();
  void InjectInitScript();
  void ExecuteScript(const std::string script);

  // A function to be called from JS
  v8::Local<v8::Promise> Request(v8::Isolate* isolate,
                                 v8::Local<v8::Value> input);
  void OnRequest(v8::Global<v8::Promise::Resolver> promise_resolver,
                 v8::Isolate* isolate,
                 v8::Global<v8::Context> context_old,
                 const int http_code,
                 const std::string& response);

  content::RenderFrame* render_frame_;
  mojo::Remote<adrbrowsiel_wallet::mojom::adrbrowsielWalletProvider> adrbrowsiel_wallet_provider_;
};

}  // namespace adrbrowsiel_wallet

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_RENDERER_adrbrowsiel_WALLET_JS_HANDLER_H_
