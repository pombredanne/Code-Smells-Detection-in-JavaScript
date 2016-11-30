/*
 * Copyright 2010 L�szl� Bal�zs-Cs�ki
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
package pixelitor.filters.jhlabsproxies;

import com.jhlabs.image.LensBlurFilter;
import pixelitor.filters.OperationWithParametrizedGUI;
import pixelitor.filters.Operations;
import pixelitor.filters.gui.BooleanParam;
import pixelitor.filters.gui.ParamSet;
import pixelitor.filters.gui.RangeParam;
import pixelitor.utils.ImageUtils;

import java.awt.image.BufferedImage;

/**
 * Lens Blur based on the JHLabs LensBlurFilter
 */
public class JHLensBlur extends OperationWithParametrizedGUI {
    private RangeParam amount = new RangeParam("Amount (Radius)", 0, 100, 0);
    private RangeParam numberOfSides = new RangeParam("Number of Sides of the Aperture", 3, 12, 5);
    private RangeParam bloomFactor = new RangeParam("Bloom Factor", 1, 8, 1);
    private RangeParam bloomThreshold = new RangeParam("Bloom Threshold", 0, 255, 200);

    private BooleanParam hpSharpening = BooleanParam.paramForHPSharpening();
    private BooleanParam showOriginalParam = BooleanParam.paramForShowOriginal();


    private LensBlurFilter filter;

    public JHLensBlur() {
        super("Lens Blur", false);
        paramSet = new ParamSet(
                amount,
                numberOfSides,
                bloomFactor,
                bloomThreshold,
                hpSharpening,
                showOriginalParam
        );
    }

    @Override
    public BufferedImage transform(BufferedImage src, BufferedImage dest) {
        int radius = amount.getValue();
        if ((radius == 0) || showOriginalParam.getValue()) {
            return Operations.getDefaultBufferedImage(src);
        }

        if (filter == null) {
            filter = new LensBlurFilter();
        }

        filter.setRadius(radius);
        filter.setSides(numberOfSides.getValue());
        filter.setBloom(bloomFactor.getValue());
        filter.setBloomThreshold(bloomThreshold.getValue());

        dest = filter.filter(src, dest);

        if (hpSharpening.getValue()) {
            dest = ImageUtils.getHighPassSharpenedImage(src, dest);
        }

        return dest;
    }
}