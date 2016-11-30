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

import com.jhlabs.image.MarbleFilter;
import pixelitor.filters.FilterWithParametrizedGUI;
import pixelitor.filters.gui.IntChoiceParam;
import pixelitor.filters.gui.ParamSet;
import pixelitor.filters.gui.RangeParam;

import java.awt.image.BufferedImage;

/**
 * Turbulent Distortion based on the JHlabs MarbleFilter
 */
public class JHTurbulentDistortion extends FilterWithParametrizedGUI {
    private RangeParam scale = new RangeParam("Scale", 2, 100, 20);
    private RangeParam amount = new RangeParam("Amount", 1, 100, 10);
    private RangeParam turbulence = new RangeParam("Turbulence", 1, 100, 50);

    private IntChoiceParam edgeAction = IntChoiceParam.getEdgeActionChoices();
    private IntChoiceParam interpolation = IntChoiceParam.getInterpolationChoices();

    private MarbleFilter filter;

    public JHTurbulentDistortion() {
        super("Turbulent Distortion", true);

        edgeAction.setDefaultChoice(IntChoiceParam.EDGE_CLAMP);
        paramSet = new ParamSet(
                scale,
                amount,
                turbulence,
                edgeAction,
                interpolation
        );
    }

    @Override
    public BufferedImage transform(BufferedImage src, BufferedImage dest) {
        if (filter == null) {
            filter = new MarbleFilter();
        }

        filter.setTurbulence(turbulence.getValueAsPercentage());
        filter.setXScale(scale.getValue());
        filter.setYScale(amount.getValue());

        filter.setEdgeAction(edgeAction.getValue());
        filter.setInterpolation(interpolation.getValue());


        dest = filter.filter(src, dest);
        return dest;
    }
}