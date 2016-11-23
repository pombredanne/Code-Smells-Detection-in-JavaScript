package org.jpos.ui.action;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import org.jpos.ui.UI;
import org.jpos.ui.UIAware;
import org.jdom.Element;

public class Dispose implements ActionListener, UIAware {
    public UI ui;
    public Dispose () {
        super();
    }
    public void setUI (UI ui, Element e) {
        this.ui = ui;
    }
    public void actionPerformed (ActionEvent ev) {
        ui.dispose ();
    }
}

