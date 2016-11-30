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
package pixelitor.layers;

import javax.swing.*;
import java.util.List;
import java.util.LinkedList;
import java.awt.Color;

/**
 * The container for LayerButton objects
 */
public class LayersPanel extends JPanel {
//    private List<LayerButton> layerButtons = new LinkedList<LayerButton>();
    private ButtonGroup buttonGroup = new ButtonGroup();


    public LayersPanel() {
        setLayout(new LayersLayout(1, 1));
//        setBorder(BorderFactory.createLineBorder(Color.BLUE));
    }

    public void addLayerButton(LayerButton layerButton) {
        buttonGroup.add(layerButton);
        add(layerButton);
        layerButton.setSelected(true);
        revalidate();
        repaint();

    }

    public void deleteLayerButton(LayerButton button) {
        buttonGroup.remove(button);
        remove(button);
        revalidate();
        repaint();
    }
}