/*
 * Copyright 2010 Laszlo Balazs-Csiki
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
package pixelitor.filters.gui;

import com.jhlabs.math.Noise;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 *
 */
public class ReseedNoiseAction extends ActionParam {
    public ReseedNoiseAction() {
        this("Reseed");
    }

    public ReseedNoiseAction(String name) {
        super(name, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Noise.reseed();
            }
        });
    }
}