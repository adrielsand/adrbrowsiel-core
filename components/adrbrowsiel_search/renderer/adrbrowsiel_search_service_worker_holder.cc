/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_search/renderer/adrbrowsiel_search_service_worker_holder.h"

#include <string>
#include <utility>

#include "base/no_destructor.h"
#include "adrbrowsiel/components/adrbrowsiel_search/renderer/adrbrowsiel_search_js_handler.h"
#include "url/gurl.h"

namespace {

static base::NoDestructor<std::vector<std::string>> g_vetted_hosts(
    {"search.adrbrowsiel.com", "search-dev.adrbrowsiel.com", "search-dev-local.adrbrowsiel.com",
     "search.adrbrowsiel.software", "search.adrbrowsielsoftware.com"});

bool IsAllowedHost(const GURL& url) {
  std::string host = url.host();
  for (size_t i = 0; i < g_vetted_hosts->size(); i++) {
    if ((*g_vetted_hosts)[i] == host)
      return true;
  }

  return false;
}

}  // namespace

namespace adrbrowsiel_search {

using JSHandlersVector = std::vector<std::unique_ptr<adrbrowsielSearchJSHandler>>;

JSHandlersVector::iterator FindContext(JSHandlersVector* contexts,
                                       v8::Local<v8::Context> v8_context) {
  auto context_matches =
      [&v8_context](const std::unique_ptr<adrbrowsielSearchJSHandler>& context) {
        v8::HandleScope handle_scope(context->GetIsolate());
        v8::Context::Scope context_scope(context->Context());

        return context->Context() == v8_context;
      };

  return std::find_if(contexts->begin(), contexts->end(), context_matches);
}

adrbrowsielSearchServiceWorkerHolder::adrbrowsielSearchServiceWorkerHolder()
    : broker_(nullptr) {}

adrbrowsielSearchServiceWorkerHolder::~adrbrowsielSearchServiceWorkerHolder() = default;

void adrbrowsielSearchServiceWorkerHolder::SetBrowserInterfaceBrokerProxy(
    blink::ThreadSafeBrowserInterfaceBrokerProxy* broker) {
  broker_ = broker;
}

void adrbrowsielSearchServiceWorkerHolder::WillEvaluateServiceWorkerOnWorkerThread(
    blink::WebServiceWorkerContextProxy* context_proxy,
    v8::Local<v8::Context> v8_context,
    int64_t service_worker_version_id,
    const GURL& service_worker_scope,
    const GURL& script_url) {
  DCHECK(broker_);
  if (!service_worker_scope.is_valid() ||
      !service_worker_scope.SchemeIsHTTPOrHTTPS() ||
      !IsAllowedHost(service_worker_scope))
    return;

  std::unique_ptr<adrbrowsielSearchJSHandler> js_handler(
      new adrbrowsielSearchJSHandler(v8_context, broker_));
  js_handler->AddJavaScriptObject();

  JSHandlersVector* js_handlers = js_handlers_tls_.Get();
  if (!js_handlers) {
    js_handlers = new JSHandlersVector();
    js_handlers_tls_.Set(js_handlers);
    content::WorkerThread::AddObserver(this);
  }
  js_handlers->push_back(std::move(js_handler));
}

void adrbrowsielSearchServiceWorkerHolder::
    WillDestroyServiceWorkerContextOnWorkerThread(
        v8::Local<v8::Context> v8_context,
        int64_t service_worker_version_id,
        const GURL& service_worker_scope,
        const GURL& script_url) {
  if (!service_worker_scope.is_valid() ||
      !service_worker_scope.SchemeIsHTTPOrHTTPS() ||
      !IsAllowedHost(service_worker_scope))
    return;

  JSHandlersVector* js_handlers = js_handlers_tls_.Get();
  if (!js_handlers)
    return;

  auto context_it = FindContext(js_handlers, v8_context);
  js_handlers->erase(context_it);
}

void adrbrowsielSearchServiceWorkerHolder::WillStopCurrentWorkerThread() {
  content::WorkerThread::RemoveObserver(this);
  JSHandlersVector* js_handlers = js_handlers_tls_.Get();
  DCHECK(js_handlers);
  for (const auto& context : *js_handlers)
    context->Invalidate();
  js_handlers_tls_.Set(nullptr);
  delete js_handlers;
}

}  // namespace adrbrowsiel_search
