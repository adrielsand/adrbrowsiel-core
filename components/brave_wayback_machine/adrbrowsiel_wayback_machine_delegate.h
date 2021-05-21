/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_WAYBACK_MACHINE_adrbrowsiel_WAYBACK_MACHINE_DELEGATE_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_WAYBACK_MACHINE_adrbrowsiel_WAYBACK_MACHINE_DELEGATE_H_


namespace content {
class WebContents;
}  // namespace content

class adrbrowsielWaybackMachineInfoBarDelegate;

class adrbrowsielWaybackMachineDelegate {
 public:
  virtual ~adrbrowsielWaybackMachineDelegate() = default;

  virtual void CreateInfoBar(content::WebContents* web_contents) = 0;
};

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_WAYBACK_MACHINE_adrbrowsiel_WAYBACK_MACHINE_DELEGATE_H_
