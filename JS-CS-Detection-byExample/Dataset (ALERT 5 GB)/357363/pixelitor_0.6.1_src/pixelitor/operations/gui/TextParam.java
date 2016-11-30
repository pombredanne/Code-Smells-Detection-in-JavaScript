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
package pixelitor.operations.gui;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.FlowLayout;
import java.util.Date;

/**
 *
 */
public class TextParam implements GUIParam {
    private String name;
    private String defaultValue;
    //    private String value;
    private ParamAdjustingListener adjustingListener;
    private JTextField tf;
    private boolean resetting = false;

    public TextParam(String name, String defaultValue) {
        this.name = name;
        this.defaultValue = defaultValue;

        tf = new JTextField(defaultValue);
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public boolean isSetToDefault() {
        return defaultValue.equals(getValue());
    }

    @Override
    public JComponent createControl() {
        JPanel p = new JPanel();
        p.setLayout(new FlowLayout(FlowLayout.LEFT));
        p.add(new JLabel(name + ": "));
        if (adjustingListener != null) {
            tf.getDocument().addDocumentListener(new DocumentListener() {
                @Override
                public void insertUpdate(DocumentEvent e) {
//                    adjustingListener.paramAdjusted();
                }

                @Override
                public void removeUpdate(DocumentEvent e) {
//                    adjustingListener.paramAdjusted();
                }

                @Override
                public void changedUpdate(DocumentEvent e) {
                    if(!resetting) {
                        adjustingListener.paramAdjusted();
                    }
                }
            });
        }
        p.add(tf);
        return p;
    }

    public void reset(boolean triggerAction) {
        if(!triggerAction) {
            resetting = true;
        }
        setValue(defaultValue);
        resetting = false;
    }

    @Override
    public void setAdjustingListener(ParamAdjustingListener listener) {
        this.adjustingListener = listener;
    }

    @Override
    public int getNrOfGUIWidgets() {
        return 2;
    }

    @Override
    public void randomize() {
        resetting = true;
        setValue(new Date().toString()); // TODO
        resetting = false;
    }

    public String getValue() {
        return tf.getText();
    }

    public void setValue(String s) {
        tf.setText(s);
    }
}