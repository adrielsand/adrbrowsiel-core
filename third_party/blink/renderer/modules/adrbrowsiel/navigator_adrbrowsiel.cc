/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/third_party/blink/renderer/modules/adrbrowsiel/navigator_adrbrowsiel.h"

#include "third_party/blink/renderer/core/frame/navigator.h"
#include "adrbrowsiel/third_party/blink/renderer/modules/adrbrowsiel/adrbrowsiel.h"

namespace blink {

Navigatoradrbrowsiel::Navigatoradrbrowsiel(Navigator& navigator)
    : Supplement<Navigator>(navigator) {}

// static
const char Navigatoradrbrowsiel::kSupplementName[] = "Navigatoradrbrowsiel";

Navigatoradrbrowsiel& Navigatoradrbrowsiel::From(Navigator& navigator) {
  Navigatoradrbrowsiel* supplement =
      Supplement<Navigator>::From<Navigatoradrbrowsiel>(navigator);
  if (!supplement) {
    supplement = MakeGarbageCollected<Navigatoradrbrowsiel>(navigator);
    ProvideTo(navigator, supplement);
  }
  return *supplement;
}

adrbrowsiel* Navigatoradrbrowsiel::adrbrowsiel(Navigator& navigator) {
  return Navigatoradrbrowsiel::From(navigator).adrbrowsiel();
}

adrbrowsiel* Navigatoradrbrowsiel::adrbrowsiel() {
  if (!adrbrowsiel_) {
    adrbrowsiel_ = MakeGarbageCollected<adrbrowsiel>();
  }
  return adrbrowsiel_;
}

void Navigatoradrbrowsiel::Trace(blink::Visitor* visitor) const {
  visitor->Trace(adrbrowsiel_);
  Supplement<Navigator>::Trace(visitor);
}

}  // namespace blink
