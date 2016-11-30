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
package pixelitor.operations.gui;

import com.jhlabs.image.TransformFilter;

import javax.swing.*;
import javax.swing.event.ListDataEvent;
import javax.swing.event.ListDataListener;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

/**
 *
 */
public class IntChoiceParam extends AbstractListModel implements ComboBoxModel, GUIParam {
    private String name;
//    private Value[] choicesList;
    private List<Value> choicesList = new ArrayList<Value>();

    private Value defaultChoice;
    private Value currentChoice;
//    private ChoiceSelector choiceSelector;
    private ParamAdjustingListener adjustingListener;
    private boolean resetting = false;

    public IntChoiceParam(String name, Value[] choices) {
        this.name = name + ":";
//        this.choicesList = choices;

        for (Value choice : choices) {
            choicesList.add(choice);
        }

        this.defaultChoice = choices[0];
        currentChoice = defaultChoice;
//        choiceSelector = new ChoiceSelector(this);
    }

    public IntChoiceParam(String name) {
        this.name = name;
//        choiceSelector = new ChoiceSelector(this);
    }

    public void addValue(String description, int value) {
        Value v = new Value(description, value);
        if (choicesList.isEmpty()) { // the first one will be the default
            defaultChoice = v;
            currentChoice = v;
        }
        choicesList.add(v);
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public boolean isSetToDefault() {
        return defaultChoice.equals(currentChoice);
    }

    @Override
    public JComponent createControl() {
        ChoiceSelector choiceSelector = new ChoiceSelector(this);
        return choiceSelector;
    }

    @Override
    public void reset(boolean triggerAction) {
        if (!triggerAction) {
            resetting = true;
        }
        setSelectedItem(defaultChoice);
        resetting = false;
    }

    @Override
    public void setAdjustingListener(ParamAdjustingListener listener) {
        adjustingListener = listener;
    }

    @Override
    public int getNrOfGUIWidgets() {
        return 2;
    }

    @Override
    public void randomize() {
        Random rnd = new Random();
        int randomIndex = rnd.nextInt(choicesList.size());
        resetting = true;
        setCurrentChoice(choicesList.get(randomIndex));
        resetting = false;
    }

    public int getCurrentInt() {
        return currentChoice.getIntValue();
    }

//    public Value getCurrentChoice() {
//        return (Value) getSelectedItem();
//    }

    public void setCurrentChoice(Value currentChoice) {
        setSelectedItem(currentChoice);
    }

    public void setDefaultChoice(Value defaultChoice) {
        this.defaultChoice = defaultChoice;
    }

    @Override
    public void setSelectedItem(Object anItem) {
        if (!currentChoice.equals(anItem)) {
            currentChoice = (Value) anItem;
            fireContentsChanged(this, -1, -1);
            if (!resetting) {
                if (adjustingListener != null) {  // when called from randomize, this is null
                    adjustingListener.paramAdjusted();
                }
            }
        }
    }

    @Override
    public Object getSelectedItem() {
        return currentChoice;
    }

    @Override
    public int getSize() {
        return choicesList.size();
    }

    @Override
    public Object getElementAt(int index) {
        return choicesList.get(index);
    }

    /**
     * Represents an integer value with a description
     */
    public static class Value {
        private int intValue;
        private String description;

        public Value(String description, int intValue) {
            this.description = description;
            this.intValue = intValue;
        }

        public int getIntValue() {
            return intValue;
        }

        public String getDescription() {
            return description;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;

            Value value = (Value) o;

            if (intValue != value.intValue) {
                return false;
            }
            if (description != null ? !description.equals(value.description) : value.description != null) {
                return false;
            }

            return true;
        }

        @Override
        public int hashCode() {
            int result = intValue;
            result = 31 * result + (description != null ? description.hashCode() : 0);
            return result;
        }

        @Override
        public String toString() {
            return description;
        }
    }


    public static final Value EDGE_CLAMP = new Value("Repeat Edge Pixels", TransformFilter.CLAMP);
    private static IntChoiceParam.Value[] edgeActions = new IntChoiceParam.Value[]{
            new IntChoiceParam.Value("Wrap Around", TransformFilter.WRAP),
            EDGE_CLAMP,
            new IntChoiceParam.Value("Transparent", TransformFilter.ZERO),
    };

    public static IntChoiceParam getEdgeActionChoices() {
        return new IntChoiceParam("Edge Action", edgeActions);
    }

    private static IntChoiceParam.Value[] interpolationChoices = new IntChoiceParam.Value[]{
            new IntChoiceParam.Value("Bilinear (Better)", TransformFilter.BILINEAR),
            new IntChoiceParam.Value("Nearest Neighbour (Faster)", TransformFilter.NEAREST_NEIGHBOUR),
    };

    public static IntChoiceParam getInterpolationChoices() {
        return new IntChoiceParam("Interpolation", interpolationChoices);
    }

    protected void fireContentsChanged(Object source, int index0, int index1) {
        Object[] listeners = listenerList.getListenerList();
        ListDataEvent e = null;

        for (int i = listeners.length - 2; i >= 0; i -= 2) {
            if (listeners[i] == ListDataListener.class) {
                if (e == null) {
                    e = new ListDataEvent(source, ListDataEvent.CONTENTS_CHANGED, index0, index1);
                }
                ((ListDataListener) listeners[i + 1]).contentsChanged(e);
            }
        }
    }


    public static void main(String[] args) {
        IntChoiceParam choiceParam = IntChoiceParam.getEdgeActionChoices();
        ParamAdjustingListener pal = new ParamAdjustingListener() {
            @Override
            public void paramAdjusted() {
                System.out.println("IntChoiceParam.main.paramAdjusted CALLED");
            }
        };
        choiceParam.setAdjustingListener(pal);
        choiceParam.setCurrentChoice(EDGE_CLAMP);
        choiceParam.reset(true);
    }


}