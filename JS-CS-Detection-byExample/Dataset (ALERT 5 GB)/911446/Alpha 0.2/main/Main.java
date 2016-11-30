/* Merchant of Venice - technical analysis software for the stock market.
   Copyright (C) 2002 Andrew Leppard (aleppard@picknowl.com.au)

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/

package org.mov.main;

import java.awt.Color;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import java.util.prefs.Preferences;
import javax.swing.JDesktopPane;
import javax.swing.UIManager;
import javax.swing.JFrame;
import org.mov.prefs.PreferencesManager;
import org.mov.ui.DesktopManager;
import org.mov.ui.MainMenu;

/**
 * The top level class which contains the main() function. This class builds 
 * the outer frame and creates the desktop.
 */
public class Main extends JFrame {
    
    private JDesktopPane desktop;
    private DesktopManager desktopManager;

    private static Main venice;

    // Release date and version information

    /** Short version string, e.g. "0.1a" */
    public static String SHORT_VERSION = "0.2a";

    /** Longer version string, e.g. "0.1 alpha" */
    public static String LONG_VERSION  = "0.2 alpha";

    /** Release date, e.g. 13/Jan/2003 */
    public static String RELEASE_DATE  = "24/Aug/2003";

    /**
     * Get the main frame for the current application
     * @return The frame
     */
    public static JFrame getApplicationFrame() {
	return Main.venice;
    }

    // Go!
    private Main() {
        // Display a brief copyright message
        String title = "Merchant of Venice, " + LONG_VERSION + " / " + RELEASE_DATE;
        System.out.println(title);
        for(int i = 0; i < title.length(); i++)
            System.out.print("-");
        System.out.println("");
        System.out.println("Copyright (C) 2003, Andrew Leppard (aleppard@picknowl.com.au)");
        System.out.println("See COPYING.txt for license terms.");

	Preferences p = PreferencesManager.getUserNode("/display");
	setSize(p.getInt("default_width", 800),
		p.getInt("default_height", 600));
	setLocation(p.getInt("default_x", 0),
		    p.getInt("default_y", 0));

	setTitle("Venice " + SHORT_VERSION);

	desktop = new JDesktopPane();
	desktopManager = new org.mov.ui.DesktopManager(desktop);
	desktop.setDesktopManager(desktopManager);

        // I didn't mind the blue colour background on the desktop pane
        // under the default steel l&f, but the Windows XP uses a very
        // strong blue colour that looks horrible. So this light green
        // which is the Venice theme will be the default.
        desktop.setBackground(new Color(238, 241,238));
	CommandManager.getInstance().setDesktop(desktop);

	// Instantiate main menu singleton
	MainMenu.getInstance(this, desktopManager, desktop);

	setContentPane(desktop);
	addWindowListener(new WindowAdapter() {
		public void windowClosing(WindowEvent e) {
		    // User closed window by hitting "X" button
		    saveSettingsAndExit();
		}
		public void windowClosed(WindowEvent e) {
		    // User closed window by selecting exit from the menu
		    saveSettingsAndExit();
		}
	    });
    }

    // Save settings and exit!
    private void saveSettingsAndExit() {
	// Save window dimensions in prefs file
	Preferences p = PreferencesManager.getUserNode("/display");
	p.putInt("default_width", getWidth());
	p.putInt("default_height", getHeight());
	p.putInt("default_x", getX());
	p.putInt("default_y", getY());

	// Call save() on each module so they can save their
	// preferences data
	desktopManager.save();

	dispose();	
	System.exit(0);
    }

    /**
     * Start the application. Currently the application ignores all 
     * command line arguments.
     */
    public static void main(String[] args) {
	// Set the look and feel to be the default for the current platform
	try {
	    UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
	}
	catch(Exception e) {
	    // Shouldn't happen, but if it does just keep going
	}
	venice = new Main();

	venice.setVisible(true);
    }
}


