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
package pixelitor.layers;

import pixelitor.Composition;
import pixelitor.filters.comp.Flip;
import pixelitor.history.History;
import pixelitor.history.TranslateEdit;

/**
 * A layer with a content (text or image layer)
 */
public abstract class ContentLayer extends Layer {
    private static final long serialVersionUID = 2L;

    private transient int temporaryTranslationX = 0;
    private transient int temporaryTranslationY = 0;
    int translationX = 0;
    int translationY = 0;

    protected ContentLayer(Composition comp, String name) {
        super(comp, name);
    }

    /**
     * startTranslation(), endTranslation(), and moveLayerRelative(int, int) are used by the Move tool
     */
    public void moveLayerRelative(int x, int y) {
        temporaryTranslationX = x;
        temporaryTranslationY = y;
    }

    public int getTranslationX() {
        return translationX + temporaryTranslationX;
    }

    public int getTranslationY() {
        return translationY + temporaryTranslationY;
    }

    /**
     * startTranslation(), endTranslation(), and moveLayerRelative(int, int) are used by the Move tool
     */
    public void startTranslation() {
        temporaryTranslationX = 0;
        temporaryTranslationY = 0;
    }

    /**
     * startTranslation(), endTranslation(), and moveLayerRelative(int, int) are used by the Move tool
     */
    public void endTranslation() {
        int oldTranslationX = translationX;
        int oldTranslationY = translationY;

        translationX += temporaryTranslationX;
        translationY += temporaryTranslationY;
        temporaryTranslationX = 0;
        temporaryTranslationY = 0;

        TranslateEdit edit = createTranslateEdit(oldTranslationX, oldTranslationY);
        History.addEdit(edit);
    }

    abstract TranslateEdit createTranslateEdit(int oldTranslationX, int oldTranslationY);

    /**
     * setTranslationX and setTranslationY programmatically set the translation
     * There is no check for layer enlargement
     */
    public void setTranslationX(int translationX) {
        this.translationX = translationX;
    }

    /**
     * setTranslationX and setTranslationY programmatically set the translation
     * There is no check for layer enlargement
     */
    public void setTranslationY(int translationY) {
        this.translationY = translationY;
    }

    public abstract void flip(Flip.Direction direction);

    public abstract void rotate(int angleDegree);

    public abstract void enlargeCanvas(int north, int east, int south, int west);
}