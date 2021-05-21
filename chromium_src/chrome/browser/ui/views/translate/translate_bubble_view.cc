/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/views/translate/adrbrowsiel_translate_bubble_view.h"
#include "adrbrowsiel/grit/adrbrowsiel_generated_resources.h"
#include "chrome/grit/generated_resources.h"

#if BUILDFLAG(ENABLE_adrbrowsiel_TRANSLATE_GO)
#undef IDS_TRANSLATE_BUBBLE_BEFORE_TRANSLATE_TITLE
#define IDS_TRANSLATE_BUBBLE_BEFORE_TRANSLATE_TITLE \
  IDS_adrbrowsiel_TRANSLATE_BUBBLE_BEFORE_TRANSLATE_TITLE
#elif BUILDFLAG(ENABLE_adrbrowsiel_TRANSLATE_EXTENSION)
#undef IDS_TRANSLATE_BUBBLE_BEFORE_TRANSLATE_TITLE
#define IDS_TRANSLATE_BUBBLE_BEFORE_TRANSLATE_TITLE \
  IDS_adrbrowsiel_TRANSLATE_BUBBLE_BEFORE_TRANSLATE_INSTALL_TITLE
#endif

#if BUILDFLAG(ENABLE_adrbrowsiel_TRANSLATE_EXTENSION)
#define adrbrowsiel_TRANSLATE_BUBBLE_VIEW_ adrbrowsielTranslateBubbleView
#else
#define adrbrowsiel_TRANSLATE_BUBBLE_VIEW_ TranslateBubbleView
#endif

#include "../../../../../../../chrome/browser/ui/views/translate/translate_bubble_view.cc"
