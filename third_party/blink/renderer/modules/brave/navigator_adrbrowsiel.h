// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef adrbrowsiel_THIRD_PARTY_BLINK_RENDERER_MODULES_adrbrowsiel_NAVIGATOR_adrbrowsiel_H_
#define adrbrowsiel_THIRD_PARTY_BLINK_RENDERER_MODULES_adrbrowsiel_NAVIGATOR_adrbrowsiel_H_

#include "third_party/blink/renderer/core/frame/navigator.h"
#include "third_party/blink/renderer/platform/bindings/name_client.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/supplementable.h"

namespace blink {

class adrbrowsiel;
class Navigator;

class Navigatoradrbrowsiel final
    : public GarbageCollected<Navigatoradrbrowsiel>,
      public Supplement<Navigator>,
      public NameClient {

 public:
  static const char kSupplementName[];

  static Navigatoradrbrowsiel& From(Navigator&);
  static adrbrowsiel* adrbrowsiel(Navigator&);
  adrbrowsiel* adrbrowsiel();

  explicit Navigatoradrbrowsiel(Navigator&);

  void Trace(blink::Visitor*) const override;
  const char* NameInHeapSnapshot() const override {
    return "Navigatoradrbrowsiel";
  }

 private:
  Member<adrbrowsiel> adrbrowsiel_;
};

}  // namespace blink

#endif  // adrbrowsiel_THIRD_PARTY_BLINK_RENDERER_MODULES_adrbrowsiel_NAVIGATOR_adrbrowsiel_H_
