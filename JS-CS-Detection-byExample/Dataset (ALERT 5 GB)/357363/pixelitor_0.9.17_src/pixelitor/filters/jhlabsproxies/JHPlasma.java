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

import com.jhlabs.image.PlasmaFilter;
import pixelitor.filters.OperationWithParametrizedGUI;
import pixelitor.filters.gui.ActionParam;
import pixelitor.filters.gui.IntChoiceParam;
import pixelitor.filters.gui.ParamSet;
import pixelitor.filters.gui.RangeParam;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;

/**
 * Plasma based on the JHLabs PlasmaFilter
 */
public class JHPlasma extends OperationWithParametrizedGUI {
    private RangeParam turbulenceParam = new RangeParam("Turbulence", 0, 600, 0);

    private IntChoiceParam typeParam = new IntChoiceParam("Colors", new IntChoiceParam.Value[]{
            new IntChoiceParam.Value("Less", LESS_COLORS),
            new IntChoiceParam.Value("More", MORE_COLORS),
    });

    @SuppressWarnings({"FieldCanBeLocal"})
    private ActionParam reseedAction = new ActionParam("Reseed", new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            if (filter != null) {
                filter.randomize();
            }
        }
    });

    private PlasmaFilter filter;
    private static final int LESS_COLORS = 0;
    private static final int MORE_COLORS = 1;

    public JHPlasma() {
        super("Plasma", true);
        paramSet = new ParamSet(
                turbulenceParam,
                typeParam,
                reseedAction
        );
    }

    @Override
    public BufferedImage transform(BufferedImage src, BufferedImage dest) {
        float turbulence = turbulenceParam.getValueAsPercentage();

        if (filter == null) {
            filter = new PlasmaFilter();
        }

        filter.setLessColors(typeParam.getValue() == LESS_COLORS);
        filter.setTurbulence(turbulence);

        dest = filter.filter(src, dest);
        return dest;
    }
}