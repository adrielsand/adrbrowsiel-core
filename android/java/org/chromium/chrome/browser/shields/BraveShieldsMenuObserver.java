/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.shields;

/**
 * Allows monitoring of adrbrowsiel shields menu actions.
 */
public interface adrbrowsielShieldsMenuObserver {
	/**
	 * Informs when the any of adrbrowsielShields Menu switch changes.
	 * @param isOn Whether shield is on.
	 * @param isTopShield Whether it is a top shield or not.
	 */
	public void onMenuTopShieldsChanged(boolean isOn, boolean isTopShield);
}
