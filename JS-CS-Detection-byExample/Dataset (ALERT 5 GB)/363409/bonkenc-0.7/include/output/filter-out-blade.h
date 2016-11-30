 /* BonkEnc version 0.7
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_OUT_BLADE_
#define _H_FILTER_OUT_BLADE_

#include "outputfilter.h"
#include <bladedll/bladedll.h>

class FilterOutBLADE : public OutputFilter
{
	private:
		BE_CONFIG	 beConfig;

		unsigned long	 samples_size;
		unsigned long	 buffersize;
		unsigned long	 handle;
	public:
				 FilterOutBLADE(bonkEncConfig *, bonkFormatInfo *);
				~FilterOutBLADE();

		bool		 EncodeData(unsigned char **, int, int *);
		bool		 DecodeData(unsigned char **, int, int *);
};

#endif