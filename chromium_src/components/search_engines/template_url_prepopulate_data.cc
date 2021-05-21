/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/search_engines/template_url_prepopulate_data.h"

#include <map>
#include <vector>

#include "base/containers/flat_map.h"
#include "base/no_destructor.h"
#include "base/stl_util.h"
#include "base/strings/utf_string_conversions.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/search_engines/adrbrowsiel_prepopulated_engines.h"
#include "components/country_codes/country_codes.h"

#define GetDataVersion GetDataVersion_ChromiumImpl
#if defined(OS_ANDROID)
#define GetLocalPrepopulatedEngines GetLocalPrepopulatedEngines_Unused
#endif
#define GetPrepopulatedDefaultSearch GetPrepopulatedDefaultSearch_Unused
#define GetPrepopulatedEngine GetPrepopulatedEngine_Unused
#define GetPrepopulatedEngines GetPrepopulatedEngines_Unused
#include "../../../../components/search_engines/template_url_prepopulate_data.cc"
#undef GetDataVersion
#if defined(OS_ANDROID)
#undef GetLocalPrepopulatedEngines
#endif
#undef GetPrepopulatedDefaultSearch
#undef GetPrepopulatedEngine
#undef GetPrepopulatedEngines

namespace TemplateURLPrepopulateData {

namespace {

// Default order in which engines will appear in the UI.
const std::vector<adrbrowsielPrepopulatedEngineID> adrbrowsiel_engines_default = {
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
};

// Variations of the order / default options by country.
const std::vector<adrbrowsielPrepopulatedEngineID> adrbrowsiel_engines_with_ecosia = {
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
    PREPOPULATED_ENGINE_ID_ECOSIA,
};

const std::vector<adrbrowsielPrepopulatedEngineID> adrbrowsiel_engines_with_yandex = {
    PREPOPULATED_ENGINE_ID_YANDEX,
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
};

const std::vector<adrbrowsielPrepopulatedEngineID> adrbrowsiel_engines_DE = {
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE,
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
    PREPOPULATED_ENGINE_ID_ECOSIA,
};

const std::vector<adrbrowsielPrepopulatedEngineID> adrbrowsiel_engines_FR = {
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
    PREPOPULATED_ENGINE_ID_ECOSIA,
};

const std::vector<adrbrowsielPrepopulatedEngineID> adrbrowsiel_engines_AU_IE = {
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE,
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
    PREPOPULATED_ENGINE_ID_ECOSIA,
};

const std::vector<adrbrowsielPrepopulatedEngineID> adrbrowsiel_engines_NZ = {
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE,
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
};

// A map to keep track of a full list of default engines for countries
// that don't use the default list.
const std::map<int, const std::vector<adrbrowsielPrepopulatedEngineID>*>
    default_engines_by_country_id_map = {
        {country_codes::CountryCharsToCountryID('A', 'M'),
         &adrbrowsiel_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('A', 'T'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('A', 'U'),
         &adrbrowsiel_engines_AU_IE},
        {country_codes::CountryCharsToCountryID('A', 'Z'),
         &adrbrowsiel_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('B', 'E'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('B', 'Y'),
         &adrbrowsiel_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('C', 'A'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('C', 'H'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('D', 'E'), &adrbrowsiel_engines_DE},
        {country_codes::CountryCharsToCountryID('D', 'K'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('E', 'S'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('F', 'I'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('F', 'R'), &adrbrowsiel_engines_FR},
        {country_codes::CountryCharsToCountryID('G', 'B'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('G', 'R'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('H', 'U'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('I', 'E'),
         &adrbrowsiel_engines_AU_IE},
        {country_codes::CountryCharsToCountryID('I', 'T'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('K', 'G'),
         &adrbrowsiel_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('K', 'Z'),
         &adrbrowsiel_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('L', 'U'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('M', 'D'),
         &adrbrowsiel_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('N', 'L'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('N', 'O'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('N', 'Z'), &adrbrowsiel_engines_NZ},
        {country_codes::CountryCharsToCountryID('P', 'T'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('R', 'U'),
         &adrbrowsiel_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('S', 'E'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('T', 'J'),
         &adrbrowsiel_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('T', 'M'),
         &adrbrowsiel_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('U', 'S'),
         &adrbrowsiel_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('U', 'Z'),
         &adrbrowsiel_engines_with_yandex}};

// A versioned map tracking the singular default search engine per-country.
adrbrowsielPrepopulatedEngineID GetDefaultSearchEngine(int country_id, int version) {
  const adrbrowsielPrepopulatedEngineID default_v6 =
    PREPOPULATED_ENGINE_ID_GOOGLE;
  static const base::NoDestructor<base::flat_map<int,
        adrbrowsielPrepopulatedEngineID>>
      content_v6({
        {country_codes::CountryCharsToCountryID('A', 'U'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
        {country_codes::CountryCharsToCountryID('F', 'R'),
         PREPOPULATED_ENGINE_ID_QWANT},
        {country_codes::CountryCharsToCountryID('D', 'E'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE},
        {country_codes::CountryCharsToCountryID('I', 'E'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
        {country_codes::CountryCharsToCountryID('N', 'Z'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
      });
  static const base::NoDestructor<base::flat_map<int,
        adrbrowsielPrepopulatedEngineID>>
      content_v8({
        {country_codes::CountryCharsToCountryID('A', 'M'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('A', 'Z'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('A', 'U'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
        {country_codes::CountryCharsToCountryID('B', 'Y'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('D', 'E'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE},
        {country_codes::CountryCharsToCountryID('F', 'R'),
         PREPOPULATED_ENGINE_ID_QWANT},
        {country_codes::CountryCharsToCountryID('I', 'E'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
        {country_codes::CountryCharsToCountryID('K', 'G'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('K', 'Z'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('M', 'D'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('N', 'Z'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
        {country_codes::CountryCharsToCountryID('R', 'U'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('T', 'J'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('T', 'M'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('U', 'Z'),
         PREPOPULATED_ENGINE_ID_YANDEX},
      });
  if (version > 7) {
    auto it = content_v8->find(country_id);
    if (it == content_v8->end()) {
        return default_v6;
    }
    return it->second;
  } else {
    auto it = content_v6->find(country_id);
    if (it == content_v6->end()) {
        return default_v6;
    }
    return it->second;
  }
}

// Builds a vector of PrepulatedEngine objects from the given array of
// |engine_ids|. Fills in the default engine index for the given |country_id|,
// if asked.
std::vector<const PrepopulatedEngine*> GetEnginesFromEngineIDs(
    const std::vector<adrbrowsielPrepopulatedEngineID>& engine_ids,
    int country_id,
    adrbrowsielPrepopulatedEngineID default_engine_id,
    size_t* default_search_provider_index = nullptr) {
  std::vector<const PrepopulatedEngine*> engines;
  const auto& adrbrowsiel_engines_map =
      TemplateURLPrepopulateData::GetadrbrowsielEnginesMap();
  for (size_t i = 0; i < engine_ids.size(); ++i) {
    const PrepopulatedEngine* engine = adrbrowsiel_engines_map.at(engine_ids.at(i));
    DCHECK(engine);
    if (engine) {
      engines.push_back(engine);
      if (default_search_provider_index &&
          default_engine_id == engine_ids.at(i))
        *default_search_provider_index = i;
    }
  }
  return engines;
}

void UpdateTemplateURLDataKeyword(
    const std::unique_ptr<TemplateURLData>& t_urld) {
  DCHECK(t_urld.get());
  switch (t_urld->prepopulate_id) {
    case PREPOPULATED_ENGINE_ID_GOOGLE:
      t_urld->SetKeyword(u":g");
      break;
    case PREPOPULATED_ENGINE_ID_BING:
      t_urld->SetKeyword(u":b");
      break;
  }
}

// Uses adrbrowsiel_engines_XX localized arrays of engine IDs instead of Chromium's
// localized arrays of PrepopulatedEngines to construct the vector of
// TemplateURLData. Also, fills in the default engine index for the given
// |country_id|.
std::vector<std::unique_ptr<TemplateURLData>>
GetadrbrowsielPrepopulatedEnginesForCountryID(
    int country_id,
    size_t* default_search_provider_index = nullptr,
    int version = kadrbrowsielCurrentDataVersion) {
  std::vector<adrbrowsielPrepopulatedEngineID> adrbrowsiel_engine_ids =
      adrbrowsiel_engines_default;

  // Check for a per-country override of this list
  const auto& it_country = default_engines_by_country_id_map.find(country_id);
  if (it_country != default_engines_by_country_id_map.end()) {
    adrbrowsiel_engine_ids = *it_country->second;
  }
  DCHECK_GT(adrbrowsiel_engine_ids.size(), 0ul);

  // Get the default engine (overridable by country) for this version
  adrbrowsielPrepopulatedEngineID default_id =
      GetDefaultSearchEngine(country_id, version);

  // Build a vector PrepopulatedEngines from adrbrowsielPrepopulatedEngineIDs and
  // also get the default engine index
  std::vector<const PrepopulatedEngine*> engines =
      GetEnginesFromEngineIDs(adrbrowsiel_engine_ids, country_id,
                              default_id, default_search_provider_index);
  DCHECK(engines.size() == adrbrowsiel_engine_ids.size());

  std::vector<std::unique_ptr<TemplateURLData>> t_urls;
  for (const PrepopulatedEngine* engine : engines) {
    std::unique_ptr<TemplateURLData> t_urld =
        TemplateURLDataFromPrepopulatedEngine(*engine);
    UpdateTemplateURLDataKeyword(t_urld);
    t_urls.push_back(std::move(t_urld));
  }

  return t_urls;
}

}  // namespace

// Redefines function with the same name in Chromium. We need to account for
// the version of adrbrowsiel engines as well: kCurrentDataVersion is defined in
// prepopulated_engines.json and is bumped every time the json file is
// modified. Since we add our own engines we need to keep track of our
// version as well and combine it with Chromium's version.
int GetDataVersion(PrefService* prefs) {
  int dataVersion = GetDataVersion_ChromiumImpl(prefs);
  // Check if returned version was from preferences override and if so return
  // that version.
  if (prefs && prefs->HasPrefPath(prefs::kSearchProviderOverridesVersion))
    return dataVersion;
  return (dataVersion + kadrbrowsielCurrentDataVersion);
}

// Redefines function with the same name in Chromium. Modifies the function to
// get search engines defined by adrbrowsiel.
std::vector<std::unique_ptr<TemplateURLData>> GetPrepopulatedEngines(
    PrefService* prefs,
    size_t* default_search_provider_index) {
  // If there is a set of search engines in the preferences file, it overrides
  // the built-in set.
  if (default_search_provider_index)
    *default_search_provider_index = 0;
  std::vector<std::unique_ptr<TemplateURLData>> t_urls =
      GetPrepopulatedTemplateURLData(prefs);
  if (!t_urls.empty())
    return t_urls;

  int version = kadrbrowsielFirstTrackedDataVersion;
  if (prefs && prefs->HasPrefPath(kadrbrowsielDefaultSearchVersion)) {
    version = prefs->GetInteger(kadrbrowsielDefaultSearchVersion);
  }

  return GetadrbrowsielPrepopulatedEnginesForCountryID(
      country_codes::GetCountryIDFromPrefs(prefs),
      default_search_provider_index,
      version);
}

// Redefines function with the same name in Chromium. Modifies the function to
// get search engines defined by adrbrowsiel.
#if defined(OS_ANDROID)

std::vector<std::unique_ptr<TemplateURLData>> GetLocalPrepopulatedEngines(
    const std::string& locale) {
  int country_id = country_codes::CountryStringToCountryID(locale);
  if (country_id == country_codes::kCountryIDUnknown) {
    LOG(ERROR) << "Unknown country code specified: " << locale;
    return std::vector<std::unique_ptr<TemplateURLData>>();
  }

  return GetadrbrowsielPrepopulatedEnginesForCountryID(country_id);
}

#endif

// Functions below are copied verbatim from
// components\search_engines\template_url_prepopulate_data.cc because they
// need to call our versions of redefined Chromium's functions.

std::unique_ptr<TemplateURLData> GetPrepopulatedEngine(PrefService* prefs,
                                                       int prepopulated_id) {
  size_t default_index;
  auto engines =
      TemplateURLPrepopulateData::GetPrepopulatedEngines(prefs, &default_index);
  for (auto& engine : engines) {
    if (engine->prepopulate_id == prepopulated_id)
      return std::move(engine);
  }
  return nullptr;
}

std::unique_ptr<TemplateURLData> GetPrepopulatedDefaultSearch(
    PrefService* prefs) {
  size_t default_search_index;
  // This could be more efficient.  We are loading all the URLs to only keep
  // the first one.
  std::vector<std::unique_ptr<TemplateURLData>> loaded_urls =
      GetPrepopulatedEngines(prefs, &default_search_index);

  return (default_search_index < loaded_urls.size())
             ? std::move(loaded_urls[default_search_index])
             : nullptr;
}

}  // namespace TemplateURLPrepopulateData
