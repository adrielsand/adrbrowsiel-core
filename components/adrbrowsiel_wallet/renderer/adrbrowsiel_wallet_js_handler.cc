/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_wallet/renderer/adrbrowsiel_wallet_js_handler.h"

#include <utility>
#include <vector>

#include "base/json/json_writer.h"
#include "base/no_destructor.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/web3_provider_constants.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/renderer/adrbrowsiel_wallet_response_helpers.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/resources/grit/adrbrowsiel_wallet_script_generated.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/v8_value_converter.h"
#include "gin/arguments.h"
#include "gin/function_template.h"
#include "third_party/blink/public/common/browser_interface_broker_proxy.h"
#include "third_party/blink/public/web/blink.h"
#include "third_party/blink/public/web/web_local_frame.h"
#include "third_party/blink/public/web/web_script_source.h"
#include "ui/base/resource/resource_bundle.h"

namespace {

static base::NoDestructor<std::string> g_provider_script("");

// Hardcode id to 1 as it is unused
const uint32_t kRequestId = 1;
const char kRequestJsonRPC[] = "2.0";

std::string LoadDataResource(const int id) {
  auto& resource_bundle = ui::ResourceBundle::GetSharedInstance();
  if (resource_bundle.IsGzipped(id)) {
    return resource_bundle.LoadDataResourceString(id);
  }

  return resource_bundle.GetRawDataResource(id).as_string();
}

v8::MaybeLocal<v8::Value> GetProperty(v8::Local<v8::Context> context,
                                      v8::Local<v8::Value> object,
                                      const std::u16string& name) {
  v8::Isolate* isolate = context->GetIsolate();
  v8::Local<v8::String> name_str =
      gin::ConvertToV8(isolate, name).As<v8::String>();
  v8::Local<v8::Object> object_obj;
  if (!object->ToObject(context).ToLocal(&object_obj)) {
    return v8::MaybeLocal<v8::Value>();
  }

  return object_obj->Get(context, name_str);
}

void CallMethodOfObject(blink::WebLocalFrame* web_frame,
                        const std::u16string& object_name,
                        const std::u16string& method_name,
                        base::Value arguments) {
  if (web_frame->IsProvisional())
    return;
  v8::Local<v8::Context> context = web_frame->MainWorldScriptContext();
  v8::Context::Scope context_scope(context);
  v8::Local<v8::Value> object;
  v8::Local<v8::Value> method;
  if (!GetProperty(context, context->Global(), object_name).ToLocal(&object) ||
      !GetProperty(context, object, method_name).ToLocal(&method)) {
     return;
  }
  std::vector<v8::Local<v8::Value>> args;
  for (auto const& argument : arguments.GetList()) {
    args.push_back(content::V8ValueConverter::Create()->ToV8Value(&argument,
                                                                  context));
  }

  web_frame->ExecuteMethodAndReturnValue(
      v8::Local<v8::Function>::Cast(method), object,
      static_cast<int>(args.size()), args.data()).ToLocalChecked();
}

}  // namespace

namespace adrbrowsiel_wallet {

adrbrowsielWalletJSHandler::adrbrowsielWalletJSHandler(content::RenderFrame* render_frame)
    : render_frame_(render_frame) {
  if (g_provider_script->empty()) {
    *g_provider_script =
        LoadDataResource(IDR_adrbrowsiel_WALLET_SCRIPT_adrbrowsiel_WALLET_SCRIPT_BUNDLE_JS);
  }
}

adrbrowsielWalletJSHandler::~adrbrowsielWalletJSHandler() = default;

bool adrbrowsielWalletJSHandler::EnsureConnected() {
  if (!adrbrowsiel_wallet_provider_.is_bound()) {
    render_frame_->GetBrowserInterfaceBroker()->GetInterface(
        adrbrowsiel_wallet_provider_.BindNewPipeAndPassReceiver());
  }

  return adrbrowsiel_wallet_provider_.is_bound();
}

void adrbrowsielWalletJSHandler::AddJavaScriptObjectToFrame(
    v8::Local<v8::Context> context) {
  v8::Isolate* isolate = blink::MainThreadIsolate();
  v8::HandleScope handle_scope(isolate);
  if (context.IsEmpty())
    return;

  v8::Context::Scope context_scope(context);

  CreateEthereumObject(isolate, context);
  InjectInitScript();
}

void adrbrowsielWalletJSHandler::CreateEthereumObject(
    v8::Isolate* isolate, v8::Local<v8::Context> context) {
  v8::Local<v8::Object> global = context->Global();
  v8::Local<v8::Object> cosmetic_filters_obj;
  v8::Local<v8::Value> cosmetic_filters_value;
  if (!global->Get(context, gin::StringToV8(isolate, "ethereum"))
           .ToLocal(&cosmetic_filters_value) ||
      !cosmetic_filters_value->IsObject()) {
    cosmetic_filters_obj = v8::Object::New(isolate);
    global
        ->Set(context, gin::StringToSymbol(isolate, "ethereum"),
              cosmetic_filters_obj)
        .Check();
    BindFunctionsToObject(isolate, context, cosmetic_filters_obj);
  }
}

void adrbrowsielWalletJSHandler::BindFunctionsToObject(
    v8::Isolate* isolate,
    v8::Local<v8::Context> context,
    v8::Local<v8::Object> javascript_object) {
  BindFunctionToObject(isolate, javascript_object, "request",
                       base::BindRepeating(&adrbrowsielWalletJSHandler::Request,
                                           base::Unretained(this), isolate));
}

template <typename Sig>
void adrbrowsielWalletJSHandler::BindFunctionToObject(
    v8::Isolate* isolate,
    v8::Local<v8::Object> javascript_object,
    const std::string& name,
    const base::RepeatingCallback<Sig>& callback) {
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  // Get the isolate associated with this object.
  javascript_object
      ->Set(context, gin::StringToSymbol(isolate, name),
            gin::CreateFunctionTemplate(isolate, callback)
                ->GetFunction(context)
                .ToLocalChecked())
      .Check();
}

v8::Local<v8::Promise> adrbrowsielWalletJSHandler::Request(
    v8::Isolate* isolate,
    v8::Local<v8::Value> input) {
  if (!EnsureConnected() || !input->IsObject())
    return v8::Local<v8::Promise>();

  std::unique_ptr<base::Value> out(
      content::V8ValueConverter::Create()->FromV8Value(
          input, isolate->GetCurrentContext()));

  base::DictionaryValue* out_dict;
  if (!out || !out->is_dict() || !out->GetAsDictionary(&out_dict))
    return v8::Local<v8::Promise>();

  // Hardcode id to 1 as it is unused
  ALLOW_UNUSED_LOCAL(out_dict->SetIntPath("id", kRequestId));
  ALLOW_UNUSED_LOCAL(out_dict->SetStringPath("jsonrpc", kRequestJsonRPC));
  std::string formed_input;
  if (!base::JSONWriter::Write(*out_dict, &formed_input))
    return v8::Local<v8::Promise>();

  v8::MaybeLocal<v8::Promise::Resolver> resolver =
      v8::Promise::Resolver::New(isolate->GetCurrentContext());
  if (!resolver.IsEmpty()) {
    auto promise_resolver(
        v8::Global<v8::Promise::Resolver>(isolate, resolver.ToLocalChecked()));
    auto context_old(
        v8::Global<v8::Context>(isolate, isolate->GetCurrentContext()));
    adrbrowsiel_wallet_provider_->Request(
        formed_input,
        base::BindOnce(&adrbrowsielWalletJSHandler::OnRequest, base::Unretained(this),
                       std::move(promise_resolver), isolate,
                       std::move(context_old)));

    return resolver.ToLocalChecked()->GetPromise();
  }

  return v8::Local<v8::Promise>();
}

void adrbrowsielWalletJSHandler::OnRequest(
    v8::Global<v8::Promise::Resolver> promise_resolver,
    v8::Isolate* isolate,
    v8::Global<v8::Context> context_old,
    const int http_code,
    const std::string& response) {
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Context> context = context_old.Get(isolate);
  v8::Context::Scope context_scope(context);

  v8::Local<v8::Promise::Resolver> resolver = promise_resolver.Get(isolate);
  bool reject = http_code != 200;
  ProviderErrors code = ProviderErrors::kDisconnected;
  std::string message;
  std::unique_ptr<base::Value> formed_response;
  if (reject) {
    code = ProviderErrors::kUnsupportedMethod;
    message = "HTTP Status code: " + base::NumberToString(http_code);
    formed_response = FormProviderResponse(code, message);
  } else {
    formed_response = FormProviderResponse(response, &reject);
  }
  v8::Local<v8::Value> result;
  if (formed_response) {
    result = content::V8ValueConverter::Create()->ToV8Value(
        formed_response.get(), context);
  }

  if (reject) {
    ALLOW_UNUSED_LOCAL(resolver->Reject(context, result));
  } else {
    ALLOW_UNUSED_LOCAL(resolver->Resolve(context, result));
  }
}

void adrbrowsielWalletJSHandler::ExecuteScript(const std::string script) {
  blink::WebLocalFrame* web_frame = render_frame_->GetWebFrame();
  if (web_frame->IsProvisional())
    return;

  web_frame->ExecuteScript(blink::WebString::FromUTF8(script));
}

void adrbrowsielWalletJSHandler::InjectInitScript() {
  ExecuteScript(*g_provider_script);
}

void adrbrowsielWalletJSHandler::FireEvent(const std::string& event,
                                     const std::string& event_args) {
  base::Value args = base::Value(base::Value::Type::LIST);
  args.Append(event);
  args.Append(event_args);
  CallMethodOfObject(render_frame_->GetWebFrame(),
                     u"ethereum",
                     u"emit",
                     std::move(args));
}

void adrbrowsielWalletJSHandler::ConnectEvent(const std::string& chain_id) {
  FireEvent(kConnectEvent, chain_id);
}

void adrbrowsielWalletJSHandler::DisconnectEvent(const std::string& message) {
  FireEvent(kDisconnectEvent, message);
}

void adrbrowsielWalletJSHandler::ChainChangedEvent(const std::string& chain_id) {
  FireEvent(kChainChangedEvent, chain_id);
}

void adrbrowsielWalletJSHandler::AccountsChangedEvent(const std::string& accounts) {
  FireEvent(kAccountsChangedEvent, accounts);
}

}  // namespace adrbrowsiel_wallet
