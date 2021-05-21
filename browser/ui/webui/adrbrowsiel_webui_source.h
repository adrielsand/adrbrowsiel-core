/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_WEBUI_adrbrowsiel_WEBUI_SOURCE_H_
#define adrbrowsiel_BROWSER_UI_WEBUI_adrbrowsiel_WEBUI_SOURCE_H_

#include <string>

namespace content {
class WebUI;
class WebUIDataSource;
}  // namespace content

namespace webui {
struct ResourcePath;
}  // namespace webui

// Add adrbrowsiel resource path mapping and localized strings to new data source.
content::WebUIDataSource* CreateAndAddWebUIDataSource(
    content::WebUI* web_ui,
    const std::string& name,
    const webui::ResourcePath* resource_map,
    size_t resouece_map_size,
    int html_resource_id,
    bool disable_trusted_types_csp = false);

#endif  // adrbrowsiel_BROWSER_UI_WEBUI_adrbrowsiel_WEBUI_SOURCE_H_
