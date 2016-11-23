 /* BonkEnc version 0.6
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_GENCONFIG_
#define _H_GENCONFIG_

#include <smoothx.h>
#include <main.h>

class configureGeneralSettings : public SMOOTHApplication
{
	private:
		SMOOTHTabRegister	*reg_register;

		SMOOTHLayer		*register_layer_encoders;
		SMOOTHLayer		*register_layer_dirs;
		SMOOTHLayer		*register_layer_cdrip;

		SMOOTHGroupBox		*encoders_group_encoder;
		SMOOTHComboBox		*encoders_combo_encoder;
		SMOOTHButton		*encoders_button_config;

		SMOOTHGroupBox		*dirs_group_outdir;
		SMOOTHEditBox		*dirs_edit_outdir;
		SMOOTHButton		*dirs_button_outdir_browse;

		SMOOTHGroupBox		*cdrip_group_drive;
		SMOOTHComboBox		*cdrip_combo_drive;
		SMOOTHGroupBox		*cdrip_group_ripping;
		SMOOTHCheckBox		*cdrip_check_paranoia;
		SMOOTHComboBox		*cdrip_combo_paranoia_mode;
		SMOOTHCheckBox		*cdrip_check_jitter;
		SMOOTHCheckBox		*cdrip_check_swapchannels;
		SMOOTHCheckBox		*cdrip_check_locktray;
		SMOOTHCheckBox		*cdrip_check_ntscsi;

		SMOOTHDivisionbar	*divbar;

		SMOOTHWindow		*mainWnd;
		SMOOTHTitlebar		*mainWnd_titlebar;
		SMOOTHLayer		*mainWnd_layer;

		SMOOTHButton		*btn_cancel;
		SMOOTHButton		*btn_ok;

		SMOOTHBool		 cdparanoia;
		SMOOTHBool		 jitter;
		SMOOTHBool		 swapchannels;
		SMOOTHBool		 locktray;
		SMOOTHBool		 ntscsi;
 
		bonkEncConfig		*currentConfig;

		SMOOTHVoid		 OK();
		SMOOTHVoid		 Cancel();
		SMOOTHVoid		 SelectDir();
		SMOOTHVoid		 ConfigureEncoder();
		SMOOTHVoid		 SetParanoia();
	public:
					 configureGeneralSettings(bonkEncConfig *);
					~configureGeneralSettings();
		SMOOTHInt		 ShowDialog();
};

#endif
