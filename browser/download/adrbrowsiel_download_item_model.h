/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_DOWNLOAD_adrbrowsiel_DOWNLOAD_ITEM_MODEL_H_
#define adrbrowsiel_BROWSER_DOWNLOAD_adrbrowsiel_DOWNLOAD_ITEM_MODEL_H_

#include <string>

#include "base/macros.h"

// The purpose of this class is to extend DonwloadItemModel's class
// functionality by adding a method for the origin URL text and
// a method that returns tool tip text that includes origin URL.
// This class, however, doesn't inherit from DownloadItemModel because
// DownloadItemView has a member of DownloadItemModel type. To use this
// extended functionality, adrbrowsielDownloadItemView will have this model as
// a member.
class DownloadUIModel;

class adrbrowsielDownloadItemModel {
 public:
  // Constructs a adrbrowsielDownloadItemModel that encapsulates DownloadItemModel.
  explicit adrbrowsielDownloadItemModel(DownloadUIModel* model);
  ~adrbrowsielDownloadItemModel();

  // Method that returns a string suitable for use as a tooltip. For
  // a regular download, the tooltip is the filename and the origin URL with an
  // indicator if the URL is secure. For an interrupted download, falls back on
  // the base class behavior.
  std::u16string GetTooltipText();

  // Returns a string suitable for use as the origin URL. |is_secure| is set to
  // true if the url is considered secure.
  std::u16string GetOriginURLText(bool* is_secure);

  // Encapsulated model.
  DownloadUIModel* model_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielDownloadItemModel);
};

#endif  // adrbrowsiel_BROWSER_DOWNLOAD_adrbrowsiel_DOWNLOAD_ITEM_MODEL_H_
