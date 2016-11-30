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

import pixelitor.NewImage;
import pixelitor.PixelitorWindow;
import pixelitor.TipsOfTheDay;
import pixelitor.history.History;
import pixelitor.io.OpenSaveManager;
import pixelitor.menus.RecentFileInfo;
import pixelitor.menus.RecentFilesMenu;
import pixelitor.tools.FgBgColorSelector;

import javax.swing.*;
import javax.swing.filechooser.FileSystemView;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GraphicsEnvironment;
import java.awt.GridBagLayout;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Window;
import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.prefs.Preferences;

/**
 * This class manages the saving and loading of application preferences
 */
public final class AppPreferences {
    private static final String FRAME_X_KEY = "window_x";
    private static final String FRAME_Y_KEY = "window_y";
    private static final String FRAME_WIDTH_KEY = "window_width";
    private static final String FRAME_HEIGHT_KEY = "window_height";

    private static final String NEW_IMAGE_WIDTH = "new_image_width";
    private static final String NEW_IMAGE_HEIGHT = "new_image_height";
    private static Dimension newImageSize = null;

    private static final String RECENT_FILE_PREFS_KEY = "recent_file_";

    private static Preferences mainUserNode = Preferences.userNodeForPackage(PixelitorWindow.class);
    private static Preferences recentFilesUserNode = Preferences.userNodeForPackage(RecentFilesMenu.class);

    private static final String FG_COLOR_KEY = "fg_color";
    private static final String BG_COLOR_KEY = "bg_color";

    private static final String LAST_OPEN_DIR_KEY = "last_open_dir";
    private static final String LAST_SAVE_DIR_KEY = "last_save_dir";
    private static final String HISTOGRAMS_SHOWN_KEY = "histograms_shown";
    private static final String LAYERS_SHOWN_KEY = "layers_shown";
    private static final String TOOLS_SHOWN_KEY = "tools_shown";
    private static final String STATUS_BAR_SHOWN_KEY = "status_bar_shown";

    private static final String UNDO_LEVELS_KEY = "undo_levels";

    /**
     * Utility class with static methods
     */
    private AppPreferences() {
    }

    private static void saveFramePosition(Window window) {
        int x = window.getX();
        int y = window.getY();
        int width = window.getWidth();
        int height = window.getHeight();

        mainUserNode.putInt(FRAME_X_KEY, x);
        mainUserNode.putInt(FRAME_Y_KEY, y);
        mainUserNode.putInt(FRAME_WIDTH_KEY, width);
        mainUserNode.putInt(FRAME_HEIGHT_KEY, height);
    }

    public static Dimension loadNewImageSize() {
        if (newImageSize == null) {
            int width = mainUserNode.getInt(NEW_IMAGE_WIDTH, 600);
            int height = mainUserNode.getInt(NEW_IMAGE_HEIGHT, 400);
            //noinspection NonThreadSafeLazyInitialization
            newImageSize = new Dimension(width, height);
        }
        return newImageSize;
    }

    private static void saveNewImageSize() {
        Dimension lastNew = NewImage.getLastNew();
        mainUserNode.putInt(NEW_IMAGE_WIDTH, lastNew.width);
        mainUserNode.putInt(NEW_IMAGE_HEIGHT, lastNew.height);
    }

    public static void loadFramePosition(Window window) {
        int x = mainUserNode.getInt(FRAME_X_KEY, 0);
        int y = mainUserNode.getInt(FRAME_Y_KEY, 0);
        int width = mainUserNode.getInt(FRAME_WIDTH_KEY, 0);
        int height = mainUserNode.getInt(FRAME_HEIGHT_KEY, 0);

        Rectangle screenBounds = GraphicsEnvironment.getLocalGraphicsEnvironment().getMaximumWindowBounds();

        if ((width <= 0) || (height <= 0)) {
            width = screenBounds.width;
            height = screenBounds.height;
        }
        if (width > screenBounds.width) {
            width = screenBounds.width;
        }
        if (height > screenBounds.height) {
            height = screenBounds.height;
        }

        if ((x < 0) || (y < 0)) {
            x = 0;
            y = 0;
        }

        window.setLocation(new Point(x, y));
        window.setSize(width, height);
    }

    public static List<RecentFileInfo> loadRecentFiles(int maxNum) {
        List<RecentFileInfo> recentFileNames = new ArrayList<RecentFileInfo>();
        for (int i = 0; i < maxNum; i++) {
            String key = RECENT_FILE_PREFS_KEY + i;
            String value = recentFilesUserNode.get(key, null);
            if (value == null) {
                break;
            }
            File file = new File(value);
            if (file.exists()) {
                RecentFileInfo fileInfo = new RecentFileInfo(file);
                if (!recentFileNames.contains(fileInfo)) {
                    recentFileNames.add(fileInfo);
                }
            }
        }
        return recentFileNames;
    }

    private static void saveRecentFiles(List<RecentFileInfo> fileNames) {
        for (int i = 0; i < fileNames.size(); i++) {
            String key = RECENT_FILE_PREFS_KEY + i;
            String value = fileNames.get(i).getSavedName();
            recentFilesUserNode.put(key, value);
        }
    }

    public static void removeRecentFiles(int maxNum) {
        for (int i = 0; i < maxNum; i++) {
            recentFilesUserNode.remove(RECENT_FILE_PREFS_KEY + i);
        }
    }


    public static File loadLastOpenDir() {
        return loadDir(LAST_OPEN_DIR_KEY);
    }

    public static File loadLastSaveDir() {
        return loadDir(LAST_SAVE_DIR_KEY);
    }

    private static File loadDir(String key) {
        String s = mainUserNode.get(key, null);

        if (s == null) {
            return getDocumentsDir();
        }
        File dir = new File(s);
        if (!dir.exists()) {
            return getDocumentsDir();
        }
        if (!dir.isDirectory()) {
            return getDocumentsDir();
        }
        return dir;
    }

    private static File getDocumentsDir() {
        return FileSystemView.getFileSystemView().getDefaultDirectory();
    }


    private static void saveLastOpenDir() {
        saveDir(OpenSaveManager.getLastOpenDir(), LAST_OPEN_DIR_KEY);
    }

    private static void saveLastSaveDir() {
        saveDir(OpenSaveManager.getLastSaveDir(), LAST_SAVE_DIR_KEY);
    }

    private static void saveDir(File f, String key) {
        if (f != null) {
            mainUserNode.put(key, f.getAbsolutePath());
        } else {
            mainUserNode.put(key, null);
        }
    }

    public static int loadUndoLevels() {
        int retVal = mainUserNode.getInt(UNDO_LEVELS_KEY, -1);
        if (retVal == -1) {
            return getDefaultUndoLevels();
        }
        return retVal;
    }

//    public static boolean loadShowDevelopMenu() {
//        boolean b = windowUserNode.getBoolean(SHOW_DEVEl_MENU_KEY, false);
//        return b;
//    }

    private static void saveUndoLevels() {
        mainUserNode.putInt(UNDO_LEVELS_KEY, History.getUndoLevels());
    }

//    private static void saveShowDevelopMenu() {
//        MenuBar menuBar = (MenuBar) PixelitorWindow.getInstance().getJMenuBar();
//        boolean b = menuBar.isDevelopMenuShown();
//        windowUserNode.putBoolean(SHOW_DEVEl_MENU_KEY, b);
//    }


    public static void savePreferencesBeforeExit() {
        saveRecentFiles(RecentFilesMenu.getInstance().getRecentFileNamesForSaving());
        saveFramePosition(PixelitorWindow.getInstance());
        saveLastOpenDir();
        saveLastSaveDir();
//        saveLFClassName();
        saveFgBgColors();
        WorkSpace.saveVisibility();
        saveUndoLevels();
//        saveShowDevelopMenu();
        TipsOfTheDay.saveNextTipNr();
        saveNewImageSize();
    }

    public static Color loadFgColor() {
        int fgInt = mainUserNode.getInt(FG_COLOR_KEY, 0xFF000000);
        return new Color(fgInt);
    }

    public static Color loadBgColor() {
        int bgInt = mainUserNode.getInt(BG_COLOR_KEY, 0xFFFFFFFF);
        return new Color(bgInt);
    }

    private static void saveFgBgColors() {
        Color fgColor = FgBgColorSelector.getFG();
        if (fgColor != null) {
            mainUserNode.putInt(FG_COLOR_KEY, fgColor.getRGB());
        }

        Color bgColor = FgBgColorSelector.getBG();
        if (bgColor != null) {
            mainUserNode.putInt(BG_COLOR_KEY, bgColor.getRGB());
        }
    }

//    private static void saveLFClassName() {
//        String className = UIManager.getLookAndFeel().getClass().getName();
//        windowUserNode.put(LOOK_AND_FEEL_KEY, className);
//    }
//
//    public static String loadLFClassName() {
//        String retVal = windowUserNode.get(LOOK_AND_FEEL_KEY, "");
//        if (retVal.isEmpty()) {
//            retVal = getDefaultLookAndFeelClass();
//        }
//        return retVal;
//    }

    public static String getDefaultLookAndFeelClass() {
        UIManager.LookAndFeelInfo[] lookAndFeels = UIManager.getInstalledLookAndFeels();
        for (UIManager.LookAndFeelInfo lookAndFeel : lookAndFeels) {
            if (lookAndFeel.getName().equals("Nimbus")) {
                return lookAndFeel.getClassName();
            }
        }
        return UIManager.getSystemLookAndFeelClassName();
    }

    public static class WorkSpace {
        /**
         * Utility class with static methods
         */
        private WorkSpace() {
        }

        private static boolean DEFAULT_HISTOGRAMS_VISIBILITY = false;
        private static boolean DEFAULT_TOOLS_VISIBILITY = true;
        private static boolean DEFAULT_LAYERS_VISIBILITY = true;
        private static boolean DEFAULT_STATUS_BAR_VISIBILITY = true;

        static boolean loaded = false;
        private static boolean histogramsVisibility;
        private static boolean toolsVisibility;
        private static boolean layersVisibility;
        private static boolean statusBarVisibility;

        private static void load() {
            if (loaded) {
                return;
            }
            histogramsVisibility = mainUserNode.getBoolean(HISTOGRAMS_SHOWN_KEY, DEFAULT_HISTOGRAMS_VISIBILITY);
            toolsVisibility = mainUserNode.getBoolean(TOOLS_SHOWN_KEY, DEFAULT_TOOLS_VISIBILITY);
            layersVisibility = mainUserNode.getBoolean(LAYERS_SHOWN_KEY, DEFAULT_LAYERS_VISIBILITY);
            statusBarVisibility = mainUserNode.getBoolean(STATUS_BAR_SHOWN_KEY, DEFAULT_STATUS_BAR_VISIBILITY);
            loaded = true;
        }

        public static void setDefault() {
            PixelitorWindow pw = PixelitorWindow.getInstance();

            pw.setHistogramsVisibility(DEFAULT_HISTOGRAMS_VISIBILITY, false);
            pw.setToolsVisibility(DEFAULT_TOOLS_VISIBILITY, false);
            pw.setLayersVisibility(DEFAULT_LAYERS_VISIBILITY, false);
            pw.setStatusBarVisibility(DEFAULT_STATUS_BAR_VISIBILITY, false);

            ((JComponent) pw.getContentPane()).revalidate();

            histogramsVisibility = DEFAULT_HISTOGRAMS_VISIBILITY;
            toolsVisibility = DEFAULT_TOOLS_VISIBILITY;
            layersVisibility = DEFAULT_LAYERS_VISIBILITY;
            statusBarVisibility = DEFAULT_STATUS_BAR_VISIBILITY;
        }

        public static boolean getHistogramsVisibility() {
            load();
            return histogramsVisibility;
        }

        public static boolean getLayersVisibility() {
            load();
            return layersVisibility;
        }

        public static boolean getStatusBarVisibility() {
            load();
            return statusBarVisibility;
        }

        public static boolean getToolsVisibility() {
            load();
            return toolsVisibility;
        }

        private static void saveVisibility() {
            mainUserNode.putBoolean(HISTOGRAMS_SHOWN_KEY, histogramsVisibility);
            mainUserNode.putBoolean(LAYERS_SHOWN_KEY, layersVisibility);
            mainUserNode.putBoolean(TOOLS_SHOWN_KEY, toolsVisibility);
            mainUserNode.putBoolean(STATUS_BAR_SHOWN_KEY, statusBarVisibility);
        }

        public static void setLayersVisibility(boolean v) {
            layersVisibility = v;
            PixelitorWindow pixelitorWindow = PixelitorWindow.getInstance();
            pixelitorWindow.setLayersVisibility(v, true);
        }

        public static void setHistogramsVisibility(boolean v) {
            histogramsVisibility = v;
            PixelitorWindow pixelitorWindow = PixelitorWindow.getInstance();
            pixelitorWindow.setHistogramsVisibility(v, true);
        }

        public static void setToolsVisibility(boolean v) {
            toolsVisibility = v;
            PixelitorWindow pixelitorWindow = PixelitorWindow.getInstance();
            pixelitorWindow.setToolsVisibility(v, true);
        }

        public static void setStatusBarVisibility(boolean v) {
            statusBarVisibility = v;
            PixelitorWindow pixelitorWindow = PixelitorWindow.getInstance();
            pixelitorWindow.setStatusBarVisibility(v, true);
        }
    }

    public static class Panel extends JPanel {
        private JTextField undoLevelsTF;
//        private JCheckBox showDevelopMenuCB;

        Panel() {
            setLayout(new GridBagLayout());
            GridBagHelper.addLabel(this, "Undo/Redo Levels: ", 0, 0);
            undoLevelsTF = new IntTextField(3);
            undoLevelsTF.setText(String.valueOf(History.getUndoLevels()));
            GridBagHelper.addControl(this, undoLevelsTF);

//            GridBagHelper.addLabel(this, "Show Develop Menu: ", 0, 1);
//            showDevelopMenuCB = new JCheckBox();
//            MenuBar menuBar = (MenuBar) PixelitorWindow.getInstance().getJMenuBar();
//            showDevelopMenuCB.setSelected(menuBar.isDevelopMenuShown());
//            GridBagHelper.addControl(this, showDevelopMenuCB);
        }

        private int getUndoLevels() {
            String s = undoLevelsTF.getText();
            int retVal = Integer.parseInt(s);
            return retVal;
        }

//        private boolean getShowDevelopMenuSetting() {
//            return showDevelopMenuCB.isSelected();
//        }

        public static void showInDialog() {
            final Panel p = new Panel();
            OKCancelDialog d = new OKCancelDialog(p, "Preferences") {
                @Override
                protected void dialogAccepted() {
                    super.dialogAccepted();

                    int undoLevels = p.getUndoLevels();
                    History.setUndoLevels(undoLevels);

//                    boolean b = p.getShowDevelopMenuSetting();
//                    MenuBar menuBar = (MenuBar) PixelitorWindow.getInstance().getJMenuBar();
//                    menuBar.setShowDevelopMenu(b);

                    dispose();
                }

                @Override
                protected void dialogCancelled() {
                    super.dialogCancelled();
                    dispose();
                }
            };
            d.setVisible(true);
        }
    }

    public static Preferences getMainUserNode() {
        return mainUserNode;
    }

    private static int getDefaultUndoLevels() {
        int sizeInMegaBytes = Utils.getMaxHeapInMegabytes();
        int retVal = 1 + (sizeInMegaBytes / 50);

        return retVal;
    }

}