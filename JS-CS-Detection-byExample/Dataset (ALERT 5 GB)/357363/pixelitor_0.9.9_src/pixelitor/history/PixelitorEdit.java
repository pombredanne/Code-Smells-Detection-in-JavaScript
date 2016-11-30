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
package pixelitor.history;

import pixelitor.Composition;

import javax.swing.undo.AbstractUndoableEdit;

/**
 * An UndoableEdit with the added possibility of fading and repeating
 */
public abstract class PixelitorEdit extends AbstractUndoableEdit {
    Composition comp;

    PixelitorEdit(Composition comp) {
        this.comp = comp;
    }

    public Composition getComp() {
        return comp;
    }

    @Override
    public void die() {
        super.die();

        comp = null;
    }

    public abstract boolean canFade();

    public abstract boolean canRepeat();
}