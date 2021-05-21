/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/common/extensions/adrbrowsiel_extensions_api_provider.h"

#include "adrbrowsiel/common/extensions/api/generated_includes.h"
#include "extensions/common/features/json_feature_provider_source.h"
#include "extensions/common/permissions/permissions_info.h"

namespace extensions {

adrbrowsielExtensionsAPIProvider::adrbrowsielExtensionsAPIProvider() {}
adrbrowsielExtensionsAPIProvider::~adrbrowsielExtensionsAPIProvider() = default;

void adrbrowsielExtensionsAPIProvider::AddAPIFeatures(FeatureProvider* provider) {
  AddadrbrowsielAPIFeatures(provider);
}

void adrbrowsielExtensionsAPIProvider::AddManifestFeatures(
    FeatureProvider* provider) {
  AddadrbrowsielManifestFeatures(provider);
}

void adrbrowsielExtensionsAPIProvider::AddPermissionFeatures(
    FeatureProvider* provider) {
  AddadrbrowsielPermissionFeatures(provider);
}

void adrbrowsielExtensionsAPIProvider::AddBehaviorFeatures(
    FeatureProvider* provider) {
  // No adrbrowsiel-specific behavior features.
}

void adrbrowsielExtensionsAPIProvider::AddAPIJSONSources(
    JSONFeatureProviderSource* json_source) {
  json_source->LoadJSON(IDR_adrbrowsiel_EXTENSION_API_FEATURES);
}

bool adrbrowsielExtensionsAPIProvider::IsAPISchemaGenerated(
    const std::string& name) {
  return api::adrbrowsielGeneratedSchemas::IsGenerated(name);
}

base::StringPiece adrbrowsielExtensionsAPIProvider::GetAPISchema(
    const std::string& name) {
  return api::adrbrowsielGeneratedSchemas::Get(name);
}

void adrbrowsielExtensionsAPIProvider::RegisterPermissions(
    PermissionsInfo* permissions_info) {
  // No adrbrowsiel-specific permissions.
}

void adrbrowsielExtensionsAPIProvider::RegisterManifestHandlers() {
  // No adrbrowsiel-specific manifest handlers.
}

}  // namespace extensions
