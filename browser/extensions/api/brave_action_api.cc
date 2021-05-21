// Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.


#include "adrbrowsiel/browser/extensions/api/adrbrowsiel_action_api.h"

#include <memory>
#include <string>
#include <utility>

#include "base/no_destructor.h"
#include "components/keyed_service/core/dependency_manager.h"
#include "components/keyed_service/core/keyed_service_factory.h"
#include "chrome/browser/extensions/api/tabs/tabs_constants.h"
#include "chrome/browser/extensions/api/tabs/windows_util.h"
#include "chrome/browser/extensions/chrome_extension_function_details.h"
#include "chrome/browser/extensions/window_controller.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "extensions/browser/extension_function.h"

namespace {

class adrbrowsielActionAPIDependencyManager : public DependencyManager {
 public:
  static adrbrowsielActionAPIDependencyManager* GetInstance() {
    static base::NoDestructor<adrbrowsielActionAPIDependencyManager> factory;
    return factory.get();
  }
  adrbrowsielActionAPIDependencyManager() {}
  ~adrbrowsielActionAPIDependencyManager() override {}

#ifndef NDEBUG
void DumpContextDependencies(void* context) const override {}
#endif  // NDEBUG

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielActionAPIDependencyManager);
};

class adrbrowsielActionAPIFactory : public KeyedServiceFactory {
 public:
  adrbrowsielActionAPIFactory() : KeyedServiceFactory("adrbrowsielActionAPI",
      adrbrowsielActionAPIDependencyManager::GetInstance(), SIMPLE) { }

  extensions::adrbrowsielActionAPI* GetadrbrowsielActionAPI(Browser* context) {
    return static_cast<extensions::adrbrowsielActionAPI*>(
          GetServiceForContext(context, true));
  }

 private:
  // KeyedServiceFactory:
  std::unique_ptr<KeyedService> BuildServiceInstanceFor(
      void* context) const final {
    return base::WrapUnique(new extensions::adrbrowsielActionAPI());
  }
  bool IsOffTheRecord(void* context) const final {
    return static_cast<Browser*>(context)
        ->profile()->IsOffTheRecord();
  }
  void* GetContextToUse(void* context) const final {
    return context;
  }
  void CreateServiceNow(void* context) final {
    KeyedServiceFactory::GetServiceForContext(context, true);
  }
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielActionAPIFactory);
};

static adrbrowsielActionAPIFactory* GetFactoryInstance() {
  static base::NoDestructor<adrbrowsielActionAPIFactory> instance;
  return instance.get();
}

}  // namespace

namespace extensions {
//
// adrbrowsielActionAPI::Observer
//
adrbrowsielActionAPI::Observer::Observer() { }

adrbrowsielActionAPI::Observer::~Observer() { }

//
// adrbrowsielActionAPI
//
// static
adrbrowsielActionAPI* adrbrowsielActionAPI::Get(Browser* context) {
  return GetFactoryInstance()->GetadrbrowsielActionAPI(context);
}

// static
bool adrbrowsielActionAPI::ShowActionUI(
      ExtensionFunction* extension_function,
      const std::string& extension_id,
      std::unique_ptr<int> window_id_param,
      std::unique_ptr<std::string> ui_relative_path_param,
      std::string* error) {
  // Which browser should we send the action to
  Browser* browser = nullptr;
  // If the windowId is specified, find it. Otherwise get the active
  // window for the profile.
  if (!window_id_param.get()) {
    browser = ChromeExtensionFunctionDetails(extension_function)
        .GetCurrentBrowser();
    if (!browser) {
      *error = tabs_constants::kNoCurrentWindowError;
      return false;
    }
  } else {
    int window_id = *window_id_param;
    std::string get_browser_error;
    if (!windows_util::GetBrowserFromWindowID(
            extension_function,
            window_id,
            WindowController::GetAllWindowFilter(),
            &browser,
            &get_browser_error)) {
      *error = get_browser_error;
      return false;
    }
  }
  return ShowActionUI(browser, extension_id, std::move(ui_relative_path_param),
      error);
}

// static
bool adrbrowsielActionAPI::ShowActionUI(
        Browser* browser,
        const std::string& extension_id,
        std::unique_ptr<std::string> ui_relative_path_param,
        std::string* error) {
  bool did_notify = adrbrowsielActionAPI::Get(browser)->NotifyObservers(extension_id,
      std::move(ui_relative_path_param));
  if (!did_notify) {
    *error = "No toolbar is registered to observe adrbrowsielActionUI "
              "calls for this window";
    return false;
  }
  return true;
}

adrbrowsielActionAPI::adrbrowsielActionAPI() {}

adrbrowsielActionAPI::~adrbrowsielActionAPI() {
}

void adrbrowsielActionAPI::AddObserver(Observer* observer) {
  observers_.AddObserver(observer);
}

void adrbrowsielActionAPI::RemoveObserver(Observer* observer) {
  observers_.RemoveObserver(observer);
}

bool adrbrowsielActionAPI::NotifyObservers(const std::string& extension_id,
      std::unique_ptr<std::string> ui_relative_path_param) {
  bool did_notify = false;
  for (auto& observer : observers_) {
    observer.OnadrbrowsielActionShouldTrigger(extension_id,
        std::move(ui_relative_path_param));
    did_notify = true;
  }
  return did_notify;
}
}  // namespace extensions
