/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_search/renderer/adrbrowsiel_search_js_handler.h"

#include <utility>

#include "base/no_destructor.h"
#include "base/strings/utf_string_conversions.h"
#include "content/public/renderer/render_frame.h"
#include "gin/arguments.h"
#include "gin/function_template.h"
#include "third_party/blink/public/common/browser_interface_broker_proxy.h"
#include "third_party/blink/public/common/thread_safe_browser_interface_broker_proxy.h"
#include "third_party/blink/public/web/blink.h"
#include "third_party/blink/public/web/web_local_frame.h"
#include "third_party/blink/public/web/web_script_source.h"

namespace adrbrowsiel_search {

adrbrowsielSearchJSHandler::adrbrowsielSearchJSHandler(
    v8::Local<v8::Context> v8_context,
    blink::ThreadSafeBrowserInterfaceBrokerProxy* broker)
    : broker_(broker),
      context_(v8_context->GetIsolate(), v8_context),
      isolate_(v8_context->GetIsolate()) {}

adrbrowsielSearchJSHandler::~adrbrowsielSearchJSHandler() = default;

bool adrbrowsielSearchJSHandler::EnsureConnected() {
  if (!adrbrowsiel_search_fallback_.is_bound() && broker_) {
    broker_->GetInterface(adrbrowsiel_search_fallback_.BindNewPipeAndPassReceiver());
  }

  return adrbrowsiel_search_fallback_.is_bound();
}

v8::Local<v8::Context> adrbrowsielSearchJSHandler::Context() {
  return v8::Local<v8::Context>::New(isolate_, context_);
}

v8::Isolate* adrbrowsielSearchJSHandler::GetIsolate() {
  return isolate_;
}

void adrbrowsielSearchJSHandler::Invalidate() {
  context_.Reset();
}

void adrbrowsielSearchJSHandler::AddJavaScriptObject() {
  v8::HandleScope handle_scope(isolate_);
  if (Context().IsEmpty())
    return;

  v8::Context::Scope context_scope(Context());

  BindFunctionsToObject();
}

void adrbrowsielSearchJSHandler::BindFunctionsToObject() {
  v8::Local<v8::Object> global = Context()->Global();
  v8::Local<v8::Object> adrbrowsiel_obj;
  v8::Local<v8::Value> adrbrowsiel_value;
  if (!global->Get(Context(), gin::StringToV8(isolate_, "adrbrowsiel"))
           .ToLocal(&adrbrowsiel_value) ||
      !adrbrowsiel_value->IsObject()) {
    adrbrowsiel_obj = v8::Object::New(isolate_);
    global->Set(Context(), gin::StringToSymbol(isolate_, "adrbrowsiel"), adrbrowsiel_obj)
        .Check();
  } else {
    adrbrowsiel_obj = adrbrowsiel_value->ToObject(Context()).ToLocalChecked();
  }

  BindFunctionToObject(
      adrbrowsiel_obj, "fetchBackupResults",
      base::BindRepeating(&adrbrowsielSearchJSHandler::FetchBackupResults,
                          base::Unretained(this)));
}

template <typename Sig>
void adrbrowsielSearchJSHandler::BindFunctionToObject(
    v8::Local<v8::Object> javascript_object,
    const std::string& name,
    const base::RepeatingCallback<Sig>& callback) {
  javascript_object
      ->Set(Context(), gin::StringToSymbol(isolate_, name),
            gin::CreateFunctionTemplate(isolate_, callback)
                ->GetFunction(Context())
                .ToLocalChecked())
      .Check();
}

v8::Local<v8::Promise> adrbrowsielSearchJSHandler::FetchBackupResults(
    const std::string& query_string,
    const std::string& lang,
    const std::string& country,
    const std::string& geo,
    bool filter_explicit_results) {
  if (!EnsureConnected())
    return v8::Local<v8::Promise>();

  v8::MaybeLocal<v8::Promise::Resolver> resolver =
      v8::Promise::Resolver::New(Context());
  if (!resolver.IsEmpty()) {
    auto promise_resolver =
        std::make_unique<v8::Global<v8::Promise::Resolver>>();
    promise_resolver->Reset(isolate_, resolver.ToLocalChecked());
    adrbrowsiel_search_fallback_->FetchBackupResults(
        query_string, lang, country, geo, filter_explicit_results,
        base::BindOnce(&adrbrowsielSearchJSHandler::OnFetchBackupResults,
                       base::Unretained(this), std::move(promise_resolver)));

    return resolver.ToLocalChecked()->GetPromise();
  }

  return v8::Local<v8::Promise>();
}

void adrbrowsielSearchJSHandler::OnFetchBackupResults(
    std::unique_ptr<v8::Global<v8::Promise::Resolver>> promise_resolver,
    const std::string& response) {
  v8::HandleScope handle_scope(isolate_);
  v8::Local<v8::Context> context = context_.Get(isolate_);
  v8::Context::Scope context_scope(context);
  v8::MicrotasksScope microtasks(isolate_,
                                 v8::MicrotasksScope::kDoNotRunMicrotasks);

  v8::Local<v8::Promise::Resolver> resolver = promise_resolver->Get(isolate_);
  v8::Local<v8::String> result;
  result = v8::String::NewFromUtf8(isolate_, response.c_str()).ToLocalChecked();

  ALLOW_UNUSED_LOCAL(resolver->Resolve(context, result));
}

}  // namespace adrbrowsiel_search
