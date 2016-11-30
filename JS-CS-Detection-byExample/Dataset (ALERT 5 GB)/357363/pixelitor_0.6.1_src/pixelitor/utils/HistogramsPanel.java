/*
 * Copyright 2009-2010 L�szl� Bal�zs-Cs�ki
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

package pixelitor.utils;

import pixelitor.ImageComponent;

import javax.swing.*;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.image.BufferedImage;

public class HistogramsPanel extends JPanel implements ImageChangeListener {
    public static final HistogramsPanel INSTANCE = new HistogramsPanel();

    private HistogramPainter red;
    private HistogramPainter green;
    private HistogramPainter blue;

    private HistogramsPanel() {
        setLayout(new BorderLayout());

        red = new HistogramPainter(Color.red);
        green = new HistogramPainter(Color.green);
        blue = new HistogramPainter(Color.blue);
//        Box box = Box.createVerticalBox();
//        add(box);

        JPanel box = new JPanel();
        box.setLayout(new BoxLayout(box, BoxLayout.Y_AXIS));
        box.setPreferredSize(new Dimension(256, 300));
        box.setMinimumSize(new Dimension(256, 300));

        box.add(red);
        box.add(green);
        box.add(blue);
        setBorder(BorderFactory.createTitledBorder("Histograms"));

        box.setBorder(BorderFactory.createLineBorder(Color.BLUE));
        JScrollPane scrollPane = new JScrollPane(box);
        add(scrollPane, BorderLayout.CENTER);
    }

    public boolean areHistogramsShown() {
        return (getParent() != null);
    }

    public void updateWithImage(BufferedImage input) {
        if (!areHistogramsShown()) {
            return;
        }
        if (input == null) {
            throw new IllegalArgumentException("trying to update with null image");
        }

        int[] redValues = new int[256];
        int[] blueValues = new int[256];
        int[] greenValues = new int[256];

        int[] data = ImageUtils.getPixelsAsArray(input);
        for (int rgb : data) {
            //                int a = (rgb >>> 24) & 0xFF;
            int r = (rgb >>> 16) & 0xFF;
            int g = (rgb >>> 8) & 0xFF;
            int b = (rgb) & 0xFF;

            redValues[r]++;
            greenValues[g]++;
            blueValues[b]++;
        }

        red.updateData(redValues);
        green.updateData(greenValues);
        blue.updateData(blueValues);
        repaint();
    }

    @Override
    public void noOpenImageAnymore() {
        red.updateWithNothing();
        green.updateWithNothing();
        blue.updateWithNothing();
        repaint();
    }

    @Override
    public void newImageOpened() {
    }

    @Override
    public void imageContentChanged(ImageChangedEvent e) {
        if (e.getChangeReason().updateHistogram()) {
            updateWithImage(e.getImageComponent().getCompositeImage());
        }
    }

    @Override
    public void activeImageHasChanged(ImageComponent imageComponent) {
        BufferedImage image = imageComponent.getCompositeImage();
        if (image != null) {
            updateWithImage(image);
        }
    }
}