/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.ntp_background_images.model;

public class BackgroundImage extends NTPImage{
    private int imageDrawable;
    private int centerPointX;
    private int centerPointY;
    private ImageCredit imageCredit;

    public BackgroundImage(
            int imageDrawable, int centerPointX, int centerPointY, ImageCredit imageCredit) {
        this.imageDrawable = imageDrawable;
        this.centerPointX = centerPointX;
        this.centerPointY = centerPointY;
        this.imageCredit = imageCredit;
    }

    public BackgroundImage(int imageDrawable, int centerPointX, int centerPointY) {
        this.imageDrawable = imageDrawable;
        this.centerPointX = centerPointX;
        this.centerPointY = centerPointY;
    }

    public int getImageDrawable() {
        return imageDrawable;
    }

    public int getCenterPointX() {
        return centerPointX;
    }

    public int getCenterPointY() {
        return centerPointY;
    }

    public ImageCredit getImageCredit() {
        return imageCredit;
    }
}