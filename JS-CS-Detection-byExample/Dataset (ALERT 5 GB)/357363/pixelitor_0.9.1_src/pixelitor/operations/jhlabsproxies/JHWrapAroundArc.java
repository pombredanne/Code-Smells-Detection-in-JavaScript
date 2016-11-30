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
package pixelitor.operations.jhlabsproxies;

import com.jhlabs.image.CircleFilter;
import pixelitor.operations.OperationWithParametrizedGUI;
import pixelitor.operations.gui.ImagePositionParam;
import pixelitor.operations.gui.IntChoiceParam;
import pixelitor.operations.gui.ParamSet;
import pixelitor.operations.gui.RangeParam;

import java.awt.image.BufferedImage;

/**
 * Wrap Around Arc based on the JHLabs CircleFilter
 */
public class JHWrapAroundArc extends OperationWithParametrizedGUI {
    private CircleFilter filter;

    private RangeParam radius = new RangeParam("Radius", 0, 400, 50);
    private RangeParam thickness = new RangeParam("Thickness", 0, 400, 150);
    private RangeParam rotateResult = new RangeParam("Rotate Result", 0, 360, 0);
    private RangeParam spread = new RangeParam("Spread", 0, 360, 180);

    private ImagePositionParam center = new ImagePositionParam("Center");
    private IntChoiceParam edgeAction = IntChoiceParam.getEdgeActionChoices();
    private IntChoiceParam interpolation = IntChoiceParam.getInterpolationChoices();

    public JHWrapAroundArc() {
        super("Wrap Around Arc", true);
        paramSet = new ParamSet(
                center,
                radius,
                thickness,
                rotateResult,
                spread,
                edgeAction,
                interpolation
        );
    }

    @Override
    public BufferedImage transform(BufferedImage src, BufferedImage dest) {
        if (filter == null) {
            filter = new CircleFilter();
        }

        filter.setCentreX(center.getRelativeX());
        filter.setCentreY(center.getRelativeY());
        filter.setRadius(radius.getValue());
        filter.setHeight(thickness.getValue());
        filter.setAngle(rotateResult.getValueInRadians());
        filter.setSpreadAngle(spread.getValueInRadians());

        filter.setInterpolation(interpolation.getCurrentInt());
        filter.setEdgeAction(edgeAction.getCurrentInt());

        dest = filter.filter(src, dest);
        return dest;
    }
}