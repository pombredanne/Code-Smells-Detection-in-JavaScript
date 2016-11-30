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

package pixelitor.menus;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.image.BufferedImage;

import javax.swing.AbstractAction;

import pixelitor.AppLogic;
import pixelitor.ImageChangeReason;
import pixelitor.ImageComponent;
import pixelitor.tools.Tool;
import pixelitor.utils.GUIUtils;
import pixelitor.utils.MarchingAntsSelection;

public class CropAction extends AbstractAction {

    public CropAction() {
        super("Crop");
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        ImageComponent activeIC = AppLogic.getActiveImageComponent();
        MarchingAntsSelection selection = Tool.CROP_SELECTION.getSelection();
        if (selection != null) {
            activeIC.cropImage(selection);
        }
    }
}