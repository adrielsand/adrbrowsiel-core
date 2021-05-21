/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_SEARCH_RENDERER_adrbrowsiel_SEARCH_JS_HANDLER_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_SEARCH_RENDERER_adrbrowsiel_SEARCH_JS_HANDLER_H_

#include <memory>
#include <string>
#include <vector>

#include "adrbrowsiel/components/adrbrowsiel_search/common/adrbrowsiel_search.mojom.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "v8/include/v8.h"

namespace blink {
class ThreadSafeBrowserInterfaceBrokerProxy;
}

namespace adrbrowsiel_search {

class adrbrowsielSearchJSHandler {
 public:
  adrbrowsielSearchJSHandler(v8::Local<v8::Context> v8_context,
                       blink::ThreadSafeBrowserInterfaceBrokerProxy* broker);
  adrbrowsielSearchJSHandler(const adrbrowsielSearchJSHandler&) = delete;
  adrbrowsielSearchJSHandler& operator=(const adrbrowsielSearchJSHandler&) = delete;
  ~adrbrowsielSearchJSHandler();

  void AddJavaScriptObject();
  void Invalidate();
  v8::Local<v8::Context> Context();
  v8::Isolate* GetIsolate();

 private:
  // Adds a function to the provided object.
  template <typename Sig>
  void BindFunctionToObject(v8::Local<v8::Object> javascript_object,
                            const std::string& name,
                            const base::RepeatingCallback<Sig>& callback);
  void BindFunctionsToObject();
  bool EnsureConnected();

  // A function to be called from JS
  v8::Local<v8::Promise> FetchBackupResults(const std::string& query_string,
                                            const std::string& lang,
                                            const std::string& country,
                                            const std::string& geo,
                                            bool filter_explicit_results);
  void OnFetchBackupResults(
      std::unique_ptr<v8::Global<v8::Promise::Resolver>> promise_resolver,
      const std::string& response);

  blink::ThreadSafeBrowserInterfaceBrokerProxy* broker_;  // not owned
  mojo::Remote<adrbrowsiel_search::mojom::adrbrowsielSearchFallback> adrbrowsiel_search_fallback_;
  v8::Global<v8::Context> context_;
  v8::Isolate* isolate_;
};

}  // namespace adrbrowsiel_search

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_SEARCH_RENDERER_adrbrowsiel_SEARCH_JS_HANDLER_H_
