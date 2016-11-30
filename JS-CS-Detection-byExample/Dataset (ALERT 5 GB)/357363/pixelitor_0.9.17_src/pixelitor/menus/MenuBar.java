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

package pixelitor.menus;

import com.jhlabs.composite.MultiplyComposite;
import pixelitor.AppLogic;
import pixelitor.Build;
import pixelitor.Composition;
import pixelitor.EnlargeCanvas;
import pixelitor.ExceptionHandler;
import pixelitor.LayerChangeReason;
import pixelitor.NewImage;
import pixelitor.PixelitorWindow;
import pixelitor.TipsOfTheDay;
import pixelitor.automate.Automate;
import pixelitor.filters.AddNoise;
import pixelitor.filters.Brick;
import pixelitor.filters.Brightness;
import pixelitor.filters.Canny;
import pixelitor.filters.ChannelInvert;
import pixelitor.filters.ChannelMixer;
import pixelitor.filters.CircleToSquare;
import pixelitor.filters.Clouds;
import pixelitor.filters.DrawGrid;
import pixelitor.filters.Fade;
import pixelitor.filters.FastBlur;
import pixelitor.filters.Fill;
import pixelitor.filters.FillWithColorWheel;
import pixelitor.filters.HueSat;
import pixelitor.filters.Invert;
import pixelitor.filters.LastOp;
import pixelitor.filters.Magnify;
import pixelitor.filters.Mirror;
import pixelitor.filters.NoDialogPixelOpFactory;
import pixelitor.filters.Orton;
import pixelitor.filters.ParamTest;
import pixelitor.filters.Posterize;
import pixelitor.filters.RandomSpheres;
import pixelitor.filters.Solarize;
import pixelitor.filters.StackBlur;
import pixelitor.filters.Threshold;
import pixelitor.filters.TileProxy;
import pixelitor.filters.ValueNoise;
import pixelitor.filters.comp.Flip;
import pixelitor.filters.comp.Rotate;
import pixelitor.filters.convolve.Convolve;
import pixelitor.filters.jhlabsproxies.JHBoxBlur;
import pixelitor.filters.jhlabsproxies.JHCaustics;
import pixelitor.filters.jhlabsproxies.JHConvolutionEdge;
import pixelitor.filters.jhlabsproxies.JHCrystallize;
import pixelitor.filters.jhlabsproxies.JHDifferenceOfGaussians;
import pixelitor.filters.jhlabsproxies.JHDryBrush;
import pixelitor.filters.jhlabsproxies.JHEmboss;
import pixelitor.filters.jhlabsproxies.JHFishEye;
import pixelitor.filters.jhlabsproxies.JHFrostedGlass;
import pixelitor.filters.jhlabsproxies.JHGaussianBlur;
import pixelitor.filters.jhlabsproxies.JHGlint;
import pixelitor.filters.jhlabsproxies.JHGlow;
import pixelitor.filters.jhlabsproxies.JHKaleidoscope;
import pixelitor.filters.jhlabsproxies.JHLaplacian;
import pixelitor.filters.jhlabsproxies.JHLensBlur;
import pixelitor.filters.jhlabsproxies.JHMedian;
import pixelitor.filters.jhlabsproxies.JHMotionBlur;
import pixelitor.filters.jhlabsproxies.JHOffset;
import pixelitor.filters.jhlabsproxies.JHPinch;
import pixelitor.filters.jhlabsproxies.JHPixelate;
import pixelitor.filters.jhlabsproxies.JHPlasma;
import pixelitor.filters.jhlabsproxies.JHPointillize;
import pixelitor.filters.jhlabsproxies.JHPolarCoordinates;
import pixelitor.filters.jhlabsproxies.JHQuantize;
import pixelitor.filters.jhlabsproxies.JHRays;
import pixelitor.filters.jhlabsproxies.JHReduceNoise;
import pixelitor.filters.jhlabsproxies.JHSmartBlur;
import pixelitor.filters.jhlabsproxies.JHSmear;
import pixelitor.filters.jhlabsproxies.JHSparkle;
import pixelitor.filters.jhlabsproxies.JHStamp;
import pixelitor.filters.jhlabsproxies.JHTurbulentDistortion;
import pixelitor.filters.jhlabsproxies.JHUnsharpMask;
import pixelitor.filters.jhlabsproxies.JHVideoFeedback;
import pixelitor.filters.jhlabsproxies.JHWaterRipple;
import pixelitor.filters.jhlabsproxies.JHWaves;
import pixelitor.filters.jhlabsproxies.JHWrapAroundArc;
import pixelitor.filters.lookup.ColorBalance;
import pixelitor.filters.lookup.Levels;
import pixelitor.filters.lookup.Luminosity;
import pixelitor.filters.lookup.StaticLookupOp;
import pixelitor.filters.lookup.StaticLookupType;
import pixelitor.filters.painters.CenteredText;
import pixelitor.history.History;
import pixelitor.io.OpenSaveManager;
import pixelitor.layers.AddNewLayerAction;
import pixelitor.layers.AdjustmentLayer;
import pixelitor.layers.ContentLayer;
import pixelitor.layers.DeleteActiveLayerAction;
import pixelitor.layers.Layer;
import pixelitor.layers.LayerDownAction;
import pixelitor.layers.LayerUpAction;
import pixelitor.layers.TextLayer;
import pixelitor.utils.AppPreferences;
import pixelitor.utils.FilterCreator;
import pixelitor.utils.HistogramsPanel;
import pixelitor.utils.PerformanceTestingDialog;
import pixelitor.utils.test.ImageTests;
import pixelitor.utils.test.OpTests;
import pixelitor.utils.test.RobotTest;
import pixelitor.utils.test.ToolTests;

import javax.swing.*;
import java.awt.Composite;
import java.awt.Graphics2D;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.image.BufferedImage;

/**
 * The menu bar of the app
 */
public class MenuBar extends JMenuBar {

    public MenuBar(JFrame parent) {
        super();

//        showDevelopMenu = AppPreferences.loadShowDevelopMenu();

        initFileMenu();
        initEditMenu();
        initColorsMenu();
        initLayerMenu();
        initSelectMenu();
        initFilterMenu();
        initViewMenu();

        if (Build.CURRENT != Build.FINAL) {
            initDevelopMenu();
        }

        initHelpMenu();
    }


    private void initFileMenu() {
        JMenu fileMenu = new JMenu("File");

        // new image
        MenuFactory.createMenuItem(NewImage.getAction(), KeyStroke.getKeyStroke('N', InputEvent.CTRL_MASK), fileMenu, MenuEnableCondition.SIMPLE);

        // open
        Action openAction = new AbstractAction("Open...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    OpenSaveManager.open();
                } catch (Exception ex) {
                    ExceptionHandler.showExceptionDialog(ex);
                }
            }
        };
        MenuFactory.createMenuItem(openAction, KeyStroke.getKeyStroke('O', InputEvent.CTRL_MASK), fileMenu, MenuEnableCondition.SIMPLE);

        // recent files
        JMenu recentFiles = RecentFilesMenu.getInstance();
        fileMenu.add(recentFiles);

        // save
        Action saveAction = new AbstractAction("Save") {
            @Override
            public void actionPerformed(ActionEvent e) {
                OpenSaveManager.save(false);
            }
        };
        MenuFactory.createMenuItem(saveAction, KeyStroke.getKeyStroke('S', InputEvent.CTRL_MASK), fileMenu, MenuEnableCondition.IF_THERE_IS_OPEN_IMAGE);

        // save as
        Action saveAsAction = new AbstractAction("Save As...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                OpenSaveManager.save(true);
            }
        };
        MenuFactory.createMenuItem(saveAsAction, KeyStroke.getKeyStroke('S', InputEvent.CTRL_MASK + InputEvent.SHIFT_MASK), fileMenu, MenuEnableCondition.IF_THERE_IS_OPEN_IMAGE);

        // close
        Action closeAction = new AbstractAction("Close") {
            @Override
            public void actionPerformed(ActionEvent e) {
                OpenSaveManager.warnAndCloseImage(AppLogic.getActiveImageComponent());
            }
        };
        MenuFactory.createMenuItem(closeAction, KeyStroke.getKeyStroke('W', InputEvent.CTRL_MASK), fileMenu, MenuEnableCondition.IF_THERE_IS_OPEN_IMAGE);

        // close all
        Action closeAllAction = new AbstractAction("Close All") {
            @Override
            public void actionPerformed(ActionEvent e) {
                OpenSaveManager.warnAndCloseAllImages();
            }
        };
        MenuFactory.createMenuItem(closeAllAction, KeyStroke.getKeyStroke('W', InputEvent.CTRL_MASK | InputEvent.ALT_MASK), fileMenu, MenuEnableCondition.IF_THERE_IS_OPEN_IMAGE);


        initAutomateSubmenu(fileMenu);

        fileMenu.addSeparator();


        // exit
        Action exitAction = new AbstractAction("Exit") {
            @Override
            public void actionPerformed(ActionEvent e) {
                AppLogic.exitApp();
            }
        };
        MenuFactory.createMenuItem(exitAction, null, fileMenu, MenuEnableCondition.SIMPLE);

        this.add(fileMenu);
    }

    private static void initAutomateSubmenu(JMenu fileMenu) {
        JMenu batchSubmenu = new JMenu("Automate");
        fileMenu.add(batchSubmenu);

        Action batchResizeAction = new AbstractAction("Batch Resize...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    Automate.batchResize();
                } catch (Exception ex) {
                    ExceptionHandler.showExceptionDialog(ex);
                }
            }
        };
        MenuFactory.createMenuItem(batchResizeAction, null, batchSubmenu, MenuEnableCondition.SIMPLE);

        Action exportLayersAction = new AbstractAction("Export Layers to PNG...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    OpenSaveManager.exportLayersToPNG();
                } catch (Exception ex) {
                    ExceptionHandler.showExceptionDialog(ex);
                }
            }
        };
        MenuFactory.createMenuItem(exportLayersAction, null, batchSubmenu, MenuEnableCondition.IF_THERE_IS_OPEN_IMAGE);
    }

    private void initEditMenu() {
        JMenu editMenu = new JMenu("Edit");

        // last op
        MenuFactory.createMenuItem(LastOp.INSTANCE, KeyStroke.getKeyStroke('F', InputEvent.CTRL_MASK), editMenu, MenuEnableCondition.IF_CAN_REPEAT_OPERATION);
        editMenu.addSeparator();

        // undo
        Action undoAction = new AbstractAction("Undo") {
            @Override
            public void actionPerformed(ActionEvent e) {
                History.undo();
            }
        };
        MenuFactory.createMenuItem(undoAction, KeyStroke.getKeyStroke('Z', InputEvent.CTRL_MASK), editMenu, MenuEnableCondition.IF_UNDO_POSSIBLE);

        // undo
        Action redoAction = new AbstractAction("Redo") {
            @Override
            public void actionPerformed(ActionEvent e) {
                History.redo();
            }
        };
        MenuFactory.createMenuItem(redoAction, KeyStroke.getKeyStroke('Z', InputEvent.SHIFT_MASK + InputEvent.CTRL_MASK), editMenu, MenuEnableCondition.IF_REDO_POSSIBLE);

        // fade
        MenuFactory.createMenuItem(new Fade(), KeyStroke.getKeyStroke('F', InputEvent.CTRL_MASK + InputEvent.SHIFT_MASK), editMenu, MenuEnableCondition.IF_FADING_POSSIBLE);

        // crop
        MenuFactory.createMenuItem(SelectionActions.getCropAction(), null, editMenu, MenuEnableCondition.SIMPLE);

        editMenu.addSeparator();

        // copy
        MenuFactory.createMenuItem(new CopyAction(CopyType.COPY_LAYER), KeyStroke.getKeyStroke('C', InputEvent.CTRL_MASK), editMenu, MenuEnableCondition.IF_THERE_IS_OPEN_IMAGE);
        MenuFactory.createMenuItem(new CopyAction(CopyType.COPY_COMPOSITE), KeyStroke.getKeyStroke('C', InputEvent.CTRL_MASK + InputEvent.SHIFT_MASK), editMenu, MenuEnableCondition.IF_THERE_IS_OPEN_IMAGE);
        // paste
        MenuFactory.createMenuItem(new PasteAction(false), KeyStroke.getKeyStroke('V', InputEvent.CTRL_MASK), editMenu, MenuEnableCondition.SIMPLE);
        MenuFactory.createMenuItem(new PasteAction(true), KeyStroke.getKeyStroke('V', InputEvent.CTRL_MASK + InputEvent.SHIFT_MASK), editMenu);


        editMenu.addSeparator();

        // resize
        Action resizeAction = new AbstractAction("Resize...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                AppLogic.resizeActiveImage();
            }
        };
        MenuFactory.createMenuItem(resizeAction, KeyStroke.getKeyStroke('I', InputEvent.CTRL_MASK | InputEvent.ALT_MASK), editMenu, MenuEnableCondition.IF_THERE_IS_OPEN_IMAGE);

        JMenu rotateSubmenu = new JMenu("Rotate/Flip");
        editMenu.add(rotateSubmenu);
        // rotate
        MenuFactory.createMenuItem(new Rotate(90, "Rotate 90\u00B0 CW"), null, rotateSubmenu, MenuEnableCondition.IF_THERE_IS_OPEN_IMAGE);
        MenuFactory.createMenuItem(new Rotate(180, "Rotate 180\u00B0"), null, rotateSubmenu, MenuEnableCondition.IF_THERE_IS_OPEN_IMAGE);
        MenuFactory.createMenuItem(new Rotate(270, "Rotate 90\u00B0 CCW"), null, rotateSubmenu, MenuEnableCondition.IF_THERE_IS_OPEN_IMAGE);
        rotateSubmenu.addSeparator();
        // flip
        MenuFactory.createMenuItem(Flip.createFlipOp(Flip.Direction.HORIZONTAL), null, rotateSubmenu, MenuEnableCondition.IF_THERE_IS_OPEN_IMAGE);
        MenuFactory.createMenuItem(Flip.createFlipOp(Flip.Direction.VERTICAL), null, rotateSubmenu, MenuEnableCondition.IF_THERE_IS_OPEN_IMAGE);

        editMenu.addSeparator();
        // preferences
        Action preferencesAction = new AbstractAction("Preferences...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                AppPreferences.Panel.showInDialog();
            }
        };
        editMenu.add(preferencesAction);

        this.add(editMenu);
    }

    private void initColorsMenu() {
        JMenu colorsMenu = new JMenu("Colors");

        MenuFactory.createMenuItem(new ColorBalance(), KeyStroke.getKeyStroke('B', InputEvent.CTRL_MASK), colorsMenu);
        MenuFactory.createMenuItem(new HueSat(), KeyStroke.getKeyStroke('U', InputEvent.CTRL_MASK), colorsMenu);
        MenuFactory.createMenuItem(new Levels(), KeyStroke.getKeyStroke('L', InputEvent.CTRL_MASK), colorsMenu);
        MenuFactory.createMenuItem(new Brightness(), null, colorsMenu);
        MenuFactory.createMenuItem(new Solarize(), null, colorsMenu);
        MenuFactory.createMenuItem(new Invert(), KeyStroke.getKeyStroke('I', InputEvent.CTRL_MASK), colorsMenu);
        MenuFactory.createMenuItem(new ChannelInvert(), null, colorsMenu);
        MenuFactory.createMenuItem(new ChannelMixer(), null, colorsMenu);

        initExtractChannelsSubmenu(colorsMenu);

        initReduceColorsSubmenu(colorsMenu);

        initFillSubmenu(colorsMenu);

        this.add(colorsMenu);
    }

    private static void initFillSubmenu(JMenu colorsMenu) {
        JMenu fillSubmenu = new JMenu("Fill");
        MenuFactory.createMenuItem(new Fill(Fill.Method.FG), KeyStroke.getKeyStroke(KeyEvent.VK_BACK_SPACE, InputEvent.ALT_MASK), fillSubmenu);
        MenuFactory.createMenuItem(new Fill(Fill.Method.BG), KeyStroke.getKeyStroke(KeyEvent.VK_BACK_SPACE, InputEvent.CTRL_MASK), fillSubmenu);
        MenuFactory.createMenuItem(new Fill(Fill.Method.BLACK), null, fillSubmenu);
        MenuFactory.createMenuItem(new Fill(Fill.Method.WHITE), null, fillSubmenu);
        MenuFactory.createMenuItem(new Fill(Fill.Method.GRAY), null, fillSubmenu);
        MenuFactory.createMenuItem(new FillWithColorWheel(), null, fillSubmenu);

        colorsMenu.add(fillSubmenu);
    }

    private static void initExtractChannelsSubmenu(JMenu colorsMenu) {
        JMenu channelsSubmenu = new JMenu("Extract Channels");
        colorsMenu.add(channelsSubmenu);

        // red and remove red
        MenuFactory.createMenuItem(new StaticLookupOp(StaticLookupType.RED), null, channelsSubmenu);
        MenuFactory.createMenuItem(NoDialogPixelOpFactory.getRedChannelOp(), null, channelsSubmenu);
        MenuFactory.createMenuItem(new StaticLookupOp(StaticLookupType.REMOVE_RED), null, channelsSubmenu);
        channelsSubmenu.addSeparator();

        // green and remove green
        MenuFactory.createMenuItem(new StaticLookupOp(StaticLookupType.GREEN), null, channelsSubmenu);
        MenuFactory.createMenuItem(NoDialogPixelOpFactory.getGreenChannelOp(), null, channelsSubmenu);
        MenuFactory.createMenuItem(new StaticLookupOp(StaticLookupType.REMOVE_GREEN), null, channelsSubmenu);
        channelsSubmenu.addSeparator();

        // blue and remove blue
        MenuFactory.createMenuItem(new StaticLookupOp(StaticLookupType.BLUE), null, channelsSubmenu);
        MenuFactory.createMenuItem(NoDialogPixelOpFactory.getBlueChannelOp(), null, channelsSubmenu);
        MenuFactory.createMenuItem(new StaticLookupOp(StaticLookupType.REMOVE_BLUE), null, channelsSubmenu);
        channelsSubmenu.addSeparator();
        MenuFactory.createMenuItem(new Luminosity(), null, channelsSubmenu);
        MenuFactory.createMenuItem(NoDialogPixelOpFactory.getValueChannelOp(), null, channelsSubmenu);
        MenuFactory.createMenuItem(NoDialogPixelOpFactory.getDesaturateChannelOp(), null, channelsSubmenu);
        channelsSubmenu.addSeparator();
        MenuFactory.createMenuItem(NoDialogPixelOpFactory.getHueChannelOp(), null, channelsSubmenu);
        MenuFactory.createMenuItem(NoDialogPixelOpFactory.getHueInColorsChannelOp(), null, channelsSubmenu);
        MenuFactory.createMenuItem(NoDialogPixelOpFactory.getSaturationChannelOp(), null, channelsSubmenu);
    }

    private static void initReduceColorsSubmenu(JMenu colorsMenu) {
        JMenu reduceColorsSubmenu = new JMenu("Reduce Colors");
        colorsMenu.add(reduceColorsSubmenu);

        MenuFactory.createMenuItem(new JHQuantize(), null, reduceColorsSubmenu);
        MenuFactory.createMenuItem(new Posterize(), null, reduceColorsSubmenu);
        MenuFactory.createMenuItem(new Threshold(), null, reduceColorsSubmenu);
    }

    private void initSelectMenu() {
        JMenu selectMenu = new JMenu("Selection");

        MenuFactory.createMenuItem(SelectionActions.getDeselectAction(), KeyStroke.getKeyStroke('D', InputEvent.CTRL_MASK), selectMenu, MenuEnableCondition.SIMPLE);
        MenuFactory.createMenuItem(SelectionActions.getInvertSelectionAction(), KeyStroke.getKeyStroke('I', InputEvent.CTRL_MASK + InputEvent.SHIFT_MASK), selectMenu, MenuEnableCondition.SIMPLE);

        selectMenu.addSeparator();
        MenuFactory.createMenuItem(SelectionActions.getTraceWithBrush(), null, selectMenu, MenuEnableCondition.SIMPLE);
        MenuFactory.createMenuItem(SelectionActions.getTraceWithEraser(), null, selectMenu, MenuEnableCondition.SIMPLE);

        this.add(selectMenu);
    }

    private void initFilterMenu() {
        JMenu filterMenu = new JMenu("Filter");

        initBlurSubmenu(filterMenu);
        initSharpenSubmenu(filterMenu);
        initDistortSubmenu(filterMenu);
        initLightSubmenu(filterMenu);
        initFunSubmenu(filterMenu);
        initNoiseSubmenu(filterMenu);
        initRenderSubmenu(filterMenu);
        initArtisticSubmenu(filterMenu);
        initFindEdgesSubmenu(filterMenu);
        initOtherSubmenu(filterMenu);

        MenuFactory.createMenuItem(CenteredText.INSTANCE, KeyStroke.getKeyStroke('T'), filterMenu);

        this.add(filterMenu);
    }

    private static void initRenderSubmenu(JMenu filterMenu) {
        JMenu renderSubmenu = new JMenu("Render");
        MenuFactory.createMenuItem(new Clouds(), null, renderSubmenu);
        MenuFactory.createMenuItem(new ValueNoise(), null, renderSubmenu);
        MenuFactory.createMenuItem(new JHCaustics(), null, renderSubmenu);
        MenuFactory.createMenuItem(new JHPlasma(), null, renderSubmenu);
//        MenuFactory.createMenuItem(new JHCells(), null, renderSubmenu);
        filterMenu.add(renderSubmenu);
    }

    private static void initFindEdgesSubmenu(JMenu filterMenu) {
        JMenu findEdgesSubmenu = new JMenu("Find Edges");
        MenuFactory.createMenuItem(new JHConvolutionEdge(), null, findEdgesSubmenu);
        MenuFactory.createMenuItem(new JHLaplacian(), null, findEdgesSubmenu);
        MenuFactory.createMenuItem(new JHDifferenceOfGaussians(), null, findEdgesSubmenu);
        MenuFactory.createMenuItem(new Canny(), null, findEdgesSubmenu);
        filterMenu.add(findEdgesSubmenu);
    }

    private static void initOtherSubmenu(JMenu filterMenu) {
        JMenu otherFiltersSubmenu = new JMenu("Other");
        MenuFactory.createMenuItem(new Convolve(3), null, otherFiltersSubmenu);
        MenuFactory.createMenuItem(new Convolve(5), null, otherFiltersSubmenu);
        MenuFactory.createMenuItem(new JHOffset(), null, otherFiltersSubmenu);


        filterMenu.add(otherFiltersSubmenu);
    }

    private static void initArtisticSubmenu(JMenu filterMenu) {
        JMenu artisticFiltersSubmenu = new JMenu("Artistic");
        MenuFactory.createMenuItem(new JHStamp(), null, artisticFiltersSubmenu);
        MenuFactory.createMenuItem(new JHDryBrush(), null, artisticFiltersSubmenu);
        MenuFactory.createMenuItem(new Orton(), null, artisticFiltersSubmenu);

        filterMenu.add(artisticFiltersSubmenu);
    }

    private static void initBlurSubmenu(JMenu filterMenu) {
        JMenu blurSubmenu = new JMenu("Blur");
        MenuFactory.createMenuItem(new JHGaussianBlur(), null, blurSubmenu);
        MenuFactory.createMenuItem(new JHSmartBlur(), null, blurSubmenu);
        MenuFactory.createMenuItem(new JHBoxBlur(), null, blurSubmenu);
        MenuFactory.createMenuItem(new FastBlur(), null, blurSubmenu);
        MenuFactory.createMenuItem(new StackBlur(), null, blurSubmenu);
        MenuFactory.createMenuItem(new JHLensBlur(), null, blurSubmenu);
        MenuFactory.createMenuItem(new JHMotionBlur(JHMotionBlur.Mode.MOTION_BLUR), null, blurSubmenu);
        MenuFactory.createMenuItem(new JHMotionBlur(JHMotionBlur.Mode.SPIN_ZOOM_BLUR), null, blurSubmenu);
        filterMenu.add(blurSubmenu);
    }

    private static void initSharpenSubmenu(JMenu filterMenu) {
        JMenu sharpenSubmenu = new JMenu("Sharpen");
        MenuFactory.createMenuItem(new JHUnsharpMask(), null, sharpenSubmenu);
        filterMenu.add(sharpenSubmenu);
    }

    private static void initNoiseSubmenu(JMenu filterMenu) {
        JMenu noiseSubmenu = new JMenu("Noise");
        MenuFactory.createMenuItem(new JHReduceNoise(), null, noiseSubmenu);
        MenuFactory.createMenuItem(new JHMedian(), null, noiseSubmenu);

        noiseSubmenu.addSeparator();
        MenuFactory.createMenuItem(new AddNoise(), null, noiseSubmenu);
        MenuFactory.createMenuItem(new JHPixelate(), null, noiseSubmenu);

        filterMenu.add(noiseSubmenu);
    }

    private static void initLightSubmenu(JMenu filterMenu) {
        JMenu lightSubmenu = new JMenu("Light");
        filterMenu.add(lightSubmenu);
        MenuFactory.createMenuItem(new JHGlow(), null, lightSubmenu);
        MenuFactory.createMenuItem(new JHSparkle(), null, lightSubmenu);
        MenuFactory.createMenuItem(new JHRays(), null, lightSubmenu);
        MenuFactory.createMenuItem(new JHGlint(), null, lightSubmenu);
    }

    private static void initDistortSubmenu(JMenu filterMenu) {
        JMenu distortMenu = new JMenu("Distort");
        filterMenu.add(distortMenu);
        MenuFactory.createMenuItem(new JHTurbulentDistortion(), null, distortMenu);
        MenuFactory.createMenuItem(new JHPinch(), null, distortMenu);
        MenuFactory.createMenuItem(new JHFishEye(), null, distortMenu);
        MenuFactory.createMenuItem(new JHWaterRipple(), null, distortMenu);
        MenuFactory.createMenuItem(new JHWaves(), null, distortMenu);
        MenuFactory.createMenuItem(new JHPolarCoordinates(), null, distortMenu);
        MenuFactory.createMenuItem(new TileProxy(), null, distortMenu);
        MenuFactory.createMenuItem(new JHFrostedGlass(), null, distortMenu);
        MenuFactory.createMenuItem(new CircleToSquare(), null, distortMenu);
        MenuFactory.createMenuItem(new JHWrapAroundArc(), null, distortMenu);
    }

    private static void initFunSubmenu(JMenu filterMenu) {
        JMenu funSubmenu = new JMenu("Fun");
        filterMenu.add(funSubmenu);
        MenuFactory.createMenuItem(new JHKaleidoscope(), null, funSubmenu);
        MenuFactory.createMenuItem(new JHCrystallize(), null, funSubmenu);
        MenuFactory.createMenuItem(new JHPointillize(), null, funSubmenu);
        MenuFactory.createMenuItem(new JHVideoFeedback(), null, funSubmenu);
        MenuFactory.createMenuItem(new RandomSpheres(), null, funSubmenu);
        MenuFactory.createMenuItem(new JHSmear(), null, funSubmenu);
        MenuFactory.createMenuItem(new JHEmboss(), null, funSubmenu);
    }


    private void initLayerMenu() {
        JMenu layersMenu = new JMenu("Layer");

        layersMenu.add(AddNewLayerAction.INSTANCE);
        layersMenu.add(DeleteActiveLayerAction.INSTANCE);

        AbstractAction flattenImageAction = new AbstractAction("Flatten Image") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Composition comp = AppLogic.getActiveComp();
                comp.flattenImage();
            }
        };
        MenuFactory.createMenuItem(flattenImageAction, null, layersMenu);

        AbstractAction mergeDownAction = new AbstractAction("Merge Down") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Composition comp = AppLogic.getActiveComp();
                comp.mergeDown();
            }
        };
        MenuFactory.createMenuItem(mergeDownAction, KeyStroke.getKeyStroke('E', InputEvent.CTRL_MASK), layersMenu);


        AbstractAction duplicateLayerAction = new AbstractAction("Duplicate Layer") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Composition comp = AppLogic.getActiveComp();
                comp.duplicateLayer();
            }
        };
        MenuFactory.createMenuItem(duplicateLayerAction, KeyStroke.getKeyStroke('J', InputEvent.CTRL_MASK), layersMenu);

        AbstractAction newLayerFromCompositeAction = new AbstractAction("New Layer from Composite") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Composition comp = AppLogic.getActiveComp();
                comp.addNewLayerFromComposite("Composite");
            }
        };
        MenuFactory.createMenuItem(newLayerFromCompositeAction, KeyStroke.getKeyStroke('E', InputEvent.CTRL_MASK + InputEvent.ALT_MASK + InputEvent.SHIFT_MASK), layersMenu);

        AbstractAction layerToCanvasSizeAction = new AbstractAction("Layer to Canvas Size") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Composition comp = AppLogic.getActiveComp();
                comp.layerToCanvasSize();
            }
        };
        MenuFactory.createMenuItem(layerToCanvasSizeAction, null, layersMenu);


        initLayerStackSubmenu(layersMenu);
        this.add(layersMenu);
    }

    private static void initLayerStackSubmenu(JMenu layersMenu) {
        JMenu layerStackSubmenu = new JMenu("Layer Stack");

        MenuFactory.createMenuItem(LayerUpAction.INSTANCE, KeyStroke.getKeyStroke(']', InputEvent.CTRL_MASK), layerStackSubmenu);

        MenuFactory.createMenuItem(LayerDownAction.INSTANCE, KeyStroke.getKeyStroke('[', InputEvent.CTRL_MASK), layerStackSubmenu);

        AbstractAction moveToLast = new AbstractAction("Layer to Top") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Composition comp = AppLogic.getActiveComp();
                comp.moveActiveLayerToTop();
            }
        };
        MenuFactory.createMenuItem(moveToLast, KeyStroke.getKeyStroke(']', InputEvent.CTRL_MASK + InputEvent.SHIFT_MASK), layerStackSubmenu);

        AbstractAction moveToFirstAction = new AbstractAction("Layer to Bottom") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Composition comp = AppLogic.getActiveComp();
                comp.moveActiveLayerToBottom();
            }
        };
        MenuFactory.createMenuItem(moveToFirstAction, KeyStroke.getKeyStroke('[', InputEvent.CTRL_MASK + InputEvent.SHIFT_MASK), layerStackSubmenu);

        layerStackSubmenu.addSeparator();

        AbstractAction moveSelectionUpAction = new AbstractAction("Raise Layer Selection") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Composition comp = AppLogic.getActiveComp();
                comp.moveLayerSelectionUp();
            }
        };
        MenuFactory.createMenuItem(moveSelectionUpAction, KeyStroke.getKeyStroke(']', InputEvent.ALT_MASK), layerStackSubmenu);

        AbstractAction moveDownSelectionAction = new AbstractAction("Lower Layer Selection") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Composition comp = AppLogic.getActiveComp();
                comp.moveLayerSelectionDown();
            }
        };
        MenuFactory.createMenuItem(moveDownSelectionAction, KeyStroke.getKeyStroke('[', InputEvent.ALT_MASK), layerStackSubmenu);


        layersMenu.add(layerStackSubmenu);
    }


    private void initViewMenu() {
        JMenu viewMenu = new JMenu("View");
//        JMenu lfSubmenu = new LookAndFeelMenu("Skin", parent);
//        viewMenu.add(lfSubmenu);

        viewMenu.add(ZoomMenu.INSTANCE);

        viewMenu.addSeparator();

        viewMenu.add(new ShowHideStatusBarAction());
        MenuFactory.createMenuItem(new ShowHideHistogramsAction(), KeyStroke.getKeyStroke(KeyEvent.VK_F6, 0), viewMenu, MenuEnableCondition.SIMPLE);
        MenuFactory.createMenuItem(new ShowHideLayersAction(), KeyStroke.getKeyStroke(KeyEvent.VK_F7, 0), viewMenu, MenuEnableCondition.SIMPLE);
        viewMenu.add(new ShowHideToolsAction());
        MenuFactory.createMenuItem(ShowHideAllAction.INSTANCE, KeyStroke.getKeyStroke(KeyEvent.VK_TAB, 0), viewMenu, MenuEnableCondition.SIMPLE);
        viewMenu.addSeparator();

        initArrangeWindowsSubmenu(viewMenu);

        this.add(viewMenu);
    }

    private static void initArrangeWindowsSubmenu(JMenu viewMenu) {
        JMenu arrangeWindowsSubmenu = new JMenu("Arrange Windows");

        AbstractAction cascadeWindowsAction = new AbstractAction("Cascade") {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    PixelitorWindow.getInstance().cascadeWindows();
                } catch (Exception ex) {
                    ExceptionHandler.showExceptionDialog(ex);
                }
            }
        };
        MenuFactory.createMenuItem(cascadeWindowsAction, null, arrangeWindowsSubmenu);

        AbstractAction tileWindowsAction = new AbstractAction("Tile") {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    PixelitorWindow.getInstance().tileWindows();
                } catch (Exception ex) {
                    ExceptionHandler.showExceptionDialog(ex);
                }
            }
        };
        MenuFactory.createMenuItem(tileWindowsAction, null, arrangeWindowsSubmenu);

        viewMenu.add(arrangeWindowsSubmenu);
    }

    private void initDevelopMenu() {
        JMenu developMenu = new JMenu("Develop");

        initDebugSubmenu(developMenu);

        initTestSubmenu(developMenu);

        initExperimentalSubmenu(developMenu);

        AbstractAction newTextLayer = new AbstractAction("New Text Layer...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                String s = JOptionPane.showInputDialog(PixelitorWindow.getInstance(), "Text:", "Text Layer Text", JOptionPane.QUESTION_MESSAGE);
                Composition comp = AppLogic.getActiveComp();
                TextLayer textLayer = new TextLayer(comp, "text layer", s);
                comp.addLayer(textLayer, LayerChangeReason.NEW_LAYER_WITH_CONTENT, false);
            }
        };
        MenuFactory.createMenuItem(newTextLayer, null, developMenu);

        AbstractAction newAdjustmentLayer = new AbstractAction("New Global Adjustment Layer...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Composition comp = AppLogic.getActiveComp();
                AdjustmentLayer adjustmentLayer = new AdjustmentLayer(comp, "invert adjustment", new Invert());
                comp.addLayer(adjustmentLayer, LayerChangeReason.NEW_LAYER_WITH_CONTENT, false);
            }
        };
        MenuFactory.createMenuItem(newAdjustmentLayer, null, developMenu);


        AbstractAction filterCreatorAction = new AbstractAction("Filter Creator...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                FilterCreator.showInDialog(PixelitorWindow.getInstance());
            }
        };
        MenuFactory.createMenuItem(filterCreatorAction, null, developMenu, MenuEnableCondition.SIMPLE);

        AbstractAction debugSpecialAction = new AbstractAction("Debug Special") {
            @Override
            public void actionPerformed(ActionEvent e) {

                BufferedImage img = AppLogic.getActiveCompositeImage();

                Composite composite =  new MultiplyComposite(1.0f);
//                Composite composite =  BlendComposite.Multiply;
//                Composite composite =  new TestComposite();

                long startTime = System.nanoTime();

                int testsRun = 100;
                for (int i = 0; i < testsRun; i++) {
                    Graphics2D g = img.createGraphics();
                    g.setComposite(composite);
                    g.drawImage(img, 0, 0, null);
                    g.dispose();
                }

                long totalTime = (System.nanoTime() - startTime) / 1000000;
                System.out.println("MenuBar.actionPerformed: it took " + totalTime + " ms, average time = " + totalTime/testsRun);

            }
        };
        MenuFactory.createMenuItem(debugSpecialAction, null, developMenu, MenuEnableCondition.SIMPLE);


        this.add(developMenu);
    }

    private static void initExperimentalSubmenu(JMenu developMenu) {
        JMenu experimentalSubmenu = new JMenu("Experimental");
        developMenu.add(experimentalSubmenu);

        MenuFactory.createMenuItem(new Magnify(), null, experimentalSubmenu);
        MenuFactory.createMenuItem(new Mirror(), null, experimentalSubmenu);

        MenuFactory.createMenuItem(EnlargeCanvas.getAction(), null, experimentalSubmenu);


        MenuFactory.createMenuItem(new DrawGrid(), null, experimentalSubmenu);
        MenuFactory.createMenuItem(new Brick(), null, experimentalSubmenu);

    }

    private static void initTestSubmenu(JMenu developMenu) {
        JMenu testSubmenu = new JMenu("Test");

        MenuFactory.createMenuItem(new ParamTest(),  KeyStroke.getKeyStroke('T', InputEvent.CTRL_MASK), testSubmenu);

        AbstractAction randomResizeAction = new AbstractAction("Random Resize") {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    OpTests.randomResize();
                } catch (Exception ex) {
                    ExceptionHandler.showExceptionDialog(ex);
                }
            }
        };
        MenuFactory.createMenuItem(randomResizeAction, null, testSubmenu);

        AbstractAction randomBrushAction = new AbstractAction("1001 Random Brush Strokes") {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    ToolTests.randomBrushStrokes();
                } catch (Exception ex) {
                    ExceptionHandler.showExceptionDialog(ex);
                }
            }
        };
        MenuFactory.createMenuItem(randomBrushAction, null, testSubmenu);

        AbstractAction robotTestAction = new AbstractAction("Robot Test...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    RobotTest.runRobot();
                } catch (Exception ex) {
                    ExceptionHandler.showExceptionDialog(ex);
                }
            }
        };
//        MenuFactory.createMenuItem(robotTestAction, KeyStroke.getKeyStroke('R', InputEvent.CTRL_MASK), testSubmenu, MenuEnableCondition.ALWAYS);
        MenuFactory.createMenuItem(robotTestAction, null, testSubmenu, MenuEnableCondition.SIMPLE);


        AbstractAction opPerformanceTestAction = new AbstractAction("Operation Performance Test...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                new PerformanceTestingDialog(PixelitorWindow.getInstance());
            }
        };
        MenuFactory.createMenuItem(opPerformanceTestAction, null, testSubmenu);

        AbstractAction ciPerformanceTestAction = new AbstractAction("getCompositeImage() Performance Test...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                OpTests.getCompositeImagePerformanceTest();
            }
        };
        MenuFactory.createMenuItem(ciPerformanceTestAction, null, testSubmenu);

        testSubmenu.addSeparator();

        AbstractAction runAllOps = new AbstractAction("Run All Operations on Current Layer") {
            @Override
            public void actionPerformed(ActionEvent e) {
                OpTests.runAllOpsOnCurrentLayer();
            }
        };
        MenuFactory.createMenuItem(runAllOps, null, testSubmenu);

        AbstractAction saveAllOps = new AbstractAction("Save the Result of Each Operation...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                OpTests.saveTheResultOfEachOp();
            }
        };
        MenuFactory.createMenuItem(saveAllOps, null, testSubmenu);

        AbstractAction saveInAllFormats = new AbstractAction("Save Current Image in All Formats...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                OpenSaveManager.saveCurrentImageInAllFormats();
            }
        };
        MenuFactory.createMenuItem(saveInAllFormats, null, testSubmenu);


        testSubmenu.addSeparator();

        AbstractAction splashScreenAction = new AbstractAction("Create Pixelitor Splash Image") {
            @Override
            public void actionPerformed(ActionEvent e) {
                ImageTests.createSplashImage();
            }
        };
        MenuFactory.createMenuItem(splashScreenAction, null, testSubmenu, MenuEnableCondition.SIMPLE);

        AbstractAction manySplashScreensAction = new AbstractAction("Save Many Splash Images...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                ImageTests.saveManySplashImages();
            }
        };
        MenuFactory.createMenuItem(manySplashScreensAction, null, testSubmenu, MenuEnableCondition.SIMPLE);

        testSubmenu.addSeparator();

        AbstractAction testAllOnNewImg = new AbstractAction("Test Layer Operations") {
            @Override
            public void actionPerformed(ActionEvent e) {
                ImageTests.testLayers();
            }
        };
        MenuFactory.createMenuItem(testAllOnNewImg, null, testSubmenu, MenuEnableCondition.SIMPLE);

        AbstractAction testTools = new AbstractAction("Test Tools") {
            @Override
            public void actionPerformed(ActionEvent e) {
                ToolTests.testTools();
            }
        };
        MenuFactory.createMenuItem(testTools, null, testSubmenu, MenuEnableCondition.SIMPLE);

        AbstractAction testIOOverlayBlur = new AbstractAction("IO Overlay Blur...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                ImageTests.ioOverlayBlur();
            }
        };
        MenuFactory.createMenuItem(testIOOverlayBlur, null, testSubmenu, MenuEnableCondition.SIMPLE);


        developMenu.add(testSubmenu);
    }

    private static void initDebugSubmenu(JMenu develMenu) {
        JMenu debugSubmenu = new JMenu("Debug");

        AbstractAction debugAppAction = new AbstractAction("Debug App...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                AppLogic.showDebugAppDialog();
            }
        };
        MenuFactory.createMenuItem(debugAppAction, null, debugSubmenu, MenuEnableCondition.SIMPLE);

        AbstractAction debugHistoryAction = new AbstractAction("Debug History...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                History.showHistory();
            }
        };
        MenuFactory.createMenuItem(debugHistoryAction, KeyStroke.getKeyStroke(KeyEvent.VK_F5, 0), debugSubmenu);

        AbstractAction imageInfo = new AbstractAction("Image Info...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Composition comp = AppLogic.getActiveComp();
                int canvasWidth = comp.getCanvasWidth();
                int canvasHeight = comp.getCanvasHeight();
                long pixels = canvasWidth * canvasHeight * 4;

                float sizeMBytes = pixels / 1048576.0f;
                String msg = String.format("Canvas Width = %d pixels\nCanvas Height = %d pixels\nSize in Memory = %.2f Mbytes/layer", canvasWidth, canvasHeight, sizeMBytes);
                ExceptionHandler.showInfoDialog("Image Info - " + comp.getName(), msg);
            }
        };
        MenuFactory.createMenuItem(imageInfo, null, debugSubmenu);

        AbstractAction repaintActive = new AbstractAction("repaint() on the active image") {
            @Override
            public void actionPerformed(ActionEvent e) {
                AppLogic.getActiveImageComponent().repaint();
            }
        };
        MenuFactory.createMenuItem(repaintActive, null, debugSubmenu);

        AbstractAction imageChangedActive = new AbstractAction("imageChanged(true, true) on the active image") {
            @Override
            public void actionPerformed(ActionEvent e) {
                AppLogic.getActiveComp().imageChanged(true, true);
            }
        };
        MenuFactory.createMenuItem(imageChangedActive, null, debugSubmenu);


        AbstractAction revalidateActive = new AbstractAction("revalidate() the main window") {
            @Override
            public void actionPerformed(ActionEvent e) {
                ((JComponent) PixelitorWindow.getInstance().getContentPane()).revalidate();
            }
        };
        MenuFactory.createMenuItem(revalidateActive, null, debugSubmenu, MenuEnableCondition.SIMPLE);

        AbstractAction resetLayerTranslation = new AbstractAction("reset the translation of current layer") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Composition comp = AppLogic.getActiveComp();
                Layer layer = comp.getActiveLayer();
                if (layer instanceof ContentLayer) {
                    ContentLayer contentLayer = (ContentLayer) layer;
                    contentLayer.setTranslationX(0);
                    contentLayer.setTranslationY(0);
                    comp.imageChanged(true, true);
                }
            }
        };
        MenuFactory.createMenuItem(resetLayerTranslation, null, debugSubmenu);


        AbstractAction updateHistogram = new AbstractAction("Update Histograms") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Composition comp = AppLogic.getActiveComp();
                HistogramsPanel.INSTANCE.updateFromCompIfShown(comp);
            }
        };
        MenuFactory.createMenuItem(updateHistogram, null, debugSubmenu);

        AbstractAction saveAllImagesToDir = new AbstractAction("Save All Images to Directory...") {
            @Override
            public void actionPerformed(ActionEvent e) {
                OpenSaveManager.saveAllImagesToDir();
            }
        };
        MenuFactory.createMenuItem(saveAllImagesToDir, null, debugSubmenu);


        develMenu.add(debugSubmenu);
    }

    private void initHelpMenu() {
        JMenu helpMenu = new JMenu("Help");

        AbstractAction tipOfTheDayAction = new AbstractAction("Tip of the Day") {
            @Override
            public void actionPerformed(ActionEvent e) {
                TipsOfTheDay.showTips(PixelitorWindow.getInstance(), true);
            }
        };
        MenuFactory.createMenuItem(tipOfTheDayAction, null, helpMenu, MenuEnableCondition.SIMPLE);

        AbstractAction aboutAction = new AbstractAction("About") {
            @Override
            public void actionPerformed(ActionEvent e) {
                new AboutDialog(PixelitorWindow.getInstance());
            }
        };
        MenuFactory.createMenuItem(aboutAction, null, helpMenu, MenuEnableCondition.SIMPLE);

        this.add(helpMenu);
    }

//    public boolean isDevelopMenuShown() {
//        return showDevelopMenu;
//    }

//    public void setShowDevelopMenu(boolean newShowDevelopMenu) {
//        if (newShowDevelopMenu && !showDevelopMenu) { // it was not shown, but it should be
//            remove(helpMenu);
//            add(developMenu);
//            add(helpMenu);
//            revalidate();
//        } else if (!newShowDevelopMenu && showDevelopMenu) { // it was shown but it shouldn't be
//            remove(developMenu);
//            revalidate();
//        }
//
//        this.showDevelopMenu = newShowDevelopMenu;
//
//    }
}