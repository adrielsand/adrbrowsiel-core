/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/views/infobars/adrbrowsiel_wayback_machine_infobar_throbber.h"

#include "base/bind.h"
#include "base/location.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/paint_throbber.h"

adrbrowsielWaybackMachineInfoBarThrobber::adrbrowsielWaybackMachineInfoBarThrobber() {
}

adrbrowsielWaybackMachineInfoBarThrobber::~adrbrowsielWaybackMachineInfoBarThrobber() {
  Stop();
}

void adrbrowsielWaybackMachineInfoBarThrobber::Start() {
  if (IsRunning())
    return;

  start_time_ = base::TimeTicks::Now();
  timer_.Start(
      FROM_HERE, base::TimeDelta::FromMilliseconds(30),
      base::BindRepeating(&adrbrowsielWaybackMachineInfoBarThrobber::SchedulePaint,
                          base::Unretained(this)));
  SchedulePaint();  // paint right away
}

void adrbrowsielWaybackMachineInfoBarThrobber::Stop() {
  if (!IsRunning())
    return;

  timer_.Stop();
  SchedulePaint();
}

void adrbrowsielWaybackMachineInfoBarThrobber::OnPaint(gfx::Canvas* canvas) {
  if (!IsRunning())
    return;

  base::TimeDelta elapsed_time = base::TimeTicks::Now() - start_time_;
  gfx::PaintThrobberSpinning(
      canvas, GetContentsBounds(), SK_ColorWHITE, elapsed_time);
}

bool adrbrowsielWaybackMachineInfoBarThrobber::IsRunning() const {
  return timer_.IsRunning();
}
