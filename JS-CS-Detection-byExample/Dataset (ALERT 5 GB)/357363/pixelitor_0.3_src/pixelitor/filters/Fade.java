/*
 * Copyright 2009 L�szl� Bal�zs-Cs�ki
 *
 * This file is part of Pixelitor. Pixelitor is free software: you
 * can redistribute it and/or modify it under the terms of the GNU
 * General Public License, version 3 as published by the Free
 * Software Foundation.
 *
 * Pixelitor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pixelitor.  If not, see <http://www.gnu.org/licenses/>.
 */

package pixelitor.filters;

import java.awt.image.BufferedImage;

import pixelitor.History;
import pixelitor.ImageChangeReason;
import pixelitor.utils.SliderSpinner;
import pixelitor.utils.Utils;

public class Fade extends AbstractOperationWithDialog {
    private static final int FADE_MIN = 0;
    private static final int FADE_MAX = 100;
    private static final int FADE_INIT = 100;

    private LinearIntParam fadeParam = new LinearIntParam("Fade", FADE_MIN, FADE_MAX,
            FADE_INIT);
    private ParamSet paramSet = new ParamSet(fadeParam);

    public Fade() {
        super("Fade");
    }

    @Override
    public BufferedImage transform(BufferedImage src, BufferedImage dest, ImageChangeReason changeReason) {
        BufferedImage previous = History.getBackupBufferedImage();

        int[] srcData = Utils.getPixelsAsArray(src);
        int[] destData = Utils.getPixelsAsArray(dest);
        int[] prevData = Utils.getPixelsAsArray(previous);

        int length = srcData.length;
        if (length != prevData.length) {
            throw new IllegalArgumentException("the image and the previous are not the same size");
        }
        for (int i = 0; i < length; i++) {
            int rgb = srcData[i];
            int a = (rgb >>> 24) & 0xFF;
            int r = (rgb >>> 16) & 0xFF;
            int g = (rgb >>> 8) & 0xFF;
            int b = (rgb) & 0xFF;

            int prev_a = (prevData[i] >>> 24) & 0xFF;
            int prev_r = (prevData[i] >>> 16) & 0xFF;
            int prev_g = (prevData[i] >>> 8) & 0xFF;
            int prev_b = (prevData[i]) & 0xFF;

            float fadeFactor = fadeParam.getValueAsFloat();

            a = (int) (prev_a * (1.0 - fadeFactor) + a * fadeFactor);
            r = (int) (prev_r * (1.0 - fadeFactor) + r * fadeFactor);
            g = (int) (prev_g * (1.0 - fadeFactor) + g * fadeFactor);
            b = (int) (prev_b * (1.0 - fadeFactor) + b * fadeFactor);

            rgb = (a << 24) | (r << 16) | (g << 8) | b;
            destData[i] = rgb;
        }

        return dest;
    }

    @Override
    public AdjustPanel getAdjustPanel() {
        if (adjustPanel == null) {
            adjustPanel = new ParametrizedAdjustments(this, false, SliderSpinner.TextPosition.BORDER, false);
        } else {
            adjustPanel.setRunFiltersIfStateChanged(false);
            paramSet.reset();
            adjustPanel.setRunFiltersIfStateChanged(true);
        }

        return adjustPanel;

    }


    @Override
    public ParamSet getParams() {
        return paramSet;
    }

}