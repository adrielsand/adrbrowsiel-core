/* Copyright 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define InitializePageLoadMetricsForWebContents \
  InitializePageLoadMetricsForWebContents_Chromium
#include "../../../../../chrome/browser/page_load_metrics/page_load_metrics_initialize.cc"
#undef InitializePageLoadMetricsForWebContents

#include "adrbrowsiel/components/adrbrowsiel_perf_predictor/browser/buildflags.h"

#if BUILDFLAG(ENABLE_adrbrowsiel_PERF_PREDICTOR)
#include "adrbrowsiel/components/adrbrowsiel_perf_predictor/browser/perf_predictor_page_metrics_observer.h"
#endif

namespace chrome {

namespace {

class adrbrowsielPageLoadMetricsEmbedder : public chrome::PageLoadMetricsEmbedder {
 public:
  explicit adrbrowsielPageLoadMetricsEmbedder(content::WebContents* web_contents);
  ~adrbrowsielPageLoadMetricsEmbedder() override;

 protected:
  // page_load_metrics::PageLoadMetricsEmbedderBase:
  void RegisterEmbedderObservers(
      ::page_load_metrics::PageLoadTracker* tracker) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielPageLoadMetricsEmbedder);
};

adrbrowsielPageLoadMetricsEmbedder::adrbrowsielPageLoadMetricsEmbedder(
    content::WebContents* web_contents)
    : chrome::PageLoadMetricsEmbedder(web_contents) {}

adrbrowsielPageLoadMetricsEmbedder::~adrbrowsielPageLoadMetricsEmbedder() = default;

void adrbrowsielPageLoadMetricsEmbedder::RegisterEmbedderObservers(
    page_load_metrics::PageLoadTracker* tracker) {
  PageLoadMetricsEmbedder::RegisterEmbedderObservers(tracker);

#if BUILDFLAG(ENABLE_adrbrowsiel_PERF_PREDICTOR)
  tracker->AddObserver(
      std::make_unique<
          adrbrowsiel_perf_predictor::PerfPredictorPageMetricsObserver>());
#endif
}

}  // namespace

void InitializePageLoadMetricsForWebContents(
    content::WebContents* web_contents) {
  // TODO(bug https://github.com/adrielsand/adrbrowsiel-browser/issues/7784)
  // change
  // android_webview/browser/page_load_metrics/page_load_metrics_initialize.cc
  // as well to register Page Load Metrics Observers
  page_load_metrics::MetricsWebContentsObserver::CreateForWebContents(
      web_contents,
      std::make_unique<adrbrowsielPageLoadMetricsEmbedder>(web_contents));
}

}  // namespace chrome
