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
package pixelitor.filters;

import pixelitor.filters.gui.CoupledRangeParam;
import pixelitor.filters.gui.IntChoiceParam;
import pixelitor.filters.gui.ParamSet;
import pixelitor.filters.gui.RangeParam;
import pixelitor.filters.impl.TileFilter;

import java.awt.image.BufferedImage;

/**
 * Glass Tile filter
 */
public class GlassTile extends FilterWithParametrizedGUI {
    private CoupledRangeParam size = new CoupledRangeParam("Tile Size", 5, 500, 50);
    private RangeParam curvature = new RangeParam("Curvature", 0, 20, 10);
    private IntChoiceParam edgeAction = IntChoiceParam.getEdgeActionChoices();
    private IntChoiceParam interpolation = IntChoiceParam.getInterpolationChoices();

    private TileFilter filter;

    public GlassTile() {
        super("Glass Tile");
        paramSet = new ParamSet(
                size,
                curvature,
                edgeAction,
                interpolation
        );
    }

    @Override
    public BufferedImage transform(BufferedImage src, BufferedImage dest) {
        if (filter == null) {
            filter = new TileFilter();
        }

        filter.setSizeX(size.getFirstValue());
        filter.setSizeY(size.getSecondValue());
        filter.setCurvature(curvature.getValue());
        filter.setEdgeAction(edgeAction.getValue());
        filter.setInterpolation(interpolation.getValue());

        dest = filter.filter(src, dest);
        return dest;
    }
}
