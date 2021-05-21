/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_SEARCH_RENDERER_adrbrowsiel_SEARCH_SERVICE_WORKER_HOLDER_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_SEARCH_RENDERER_adrbrowsiel_SEARCH_SERVICE_WORKER_HOLDER_H_

#include <memory>
#include <vector>

#include "base/threading/thread_local.h"
#include "content/public/renderer/worker_thread.h"
#include "v8/include/v8.h"

class GURL;

namespace blink {
class WebServiceWorkerContextProxy;
class ThreadSafeBrowserInterfaceBrokerProxy;
}  // namespace blink

namespace adrbrowsiel_search {

class adrbrowsielSearchJSHandler;

class adrbrowsielSearchServiceWorkerHolder : public content::WorkerThread::Observer {
 public:
  adrbrowsielSearchServiceWorkerHolder();
  adrbrowsielSearchServiceWorkerHolder(const adrbrowsielSearchServiceWorkerHolder&) =
      delete;
  adrbrowsielSearchServiceWorkerHolder& operator=(
      const adrbrowsielSearchServiceWorkerHolder&) = delete;
  ~adrbrowsielSearchServiceWorkerHolder() override;

  void SetBrowserInterfaceBrokerProxy(
      blink::ThreadSafeBrowserInterfaceBrokerProxy* broker);
  void WillEvaluateServiceWorkerOnWorkerThread(
      blink::WebServiceWorkerContextProxy* context_proxy,
      v8::Local<v8::Context> v8_context,
      int64_t service_worker_version_id,
      const GURL& service_worker_scope,
      const GURL& script_url);
  void WillDestroyServiceWorkerContextOnWorkerThread(
      v8::Local<v8::Context> v8_context,
      int64_t service_worker_version_id,
      const GURL& service_worker_scope,
      const GURL& script_url);

 private:
  // WorkerThread::Observer:
  void WillStopCurrentWorkerThread() override;

  // Implement thread safety by storing each adrbrowsielSearchJSHandler in TLS. The
  // vector is called from worker threads.
  base::ThreadLocalPointer<std::vector<std::unique_ptr<adrbrowsielSearchJSHandler>>>
      js_handlers_tls_;
  blink::ThreadSafeBrowserInterfaceBrokerProxy* broker_;  // not owned
};

}  // namespace adrbrowsiel_search

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_SEARCH_RENDERER_adrbrowsiel_SEARCH_SERVICE_WORKER_HOLDER_H_
