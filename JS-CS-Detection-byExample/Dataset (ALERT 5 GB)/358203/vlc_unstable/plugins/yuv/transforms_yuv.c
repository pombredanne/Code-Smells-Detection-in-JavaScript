/*****************************************************************************
 * transforms_yuv.c: C YUV transformation functions
 * Provides functions to perform the YUV conversion. The functions provided here
 * are a complete and portable C implementation, and may be replaced in certain
 * case by optimized functions.
 *****************************************************************************
 * Copyright (C) 1999, 2000 VideoLAN
 *
 * Authors:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#include "defs.h"

#include <math.h>                                            /* exp(), pow() */
#include <errno.h>                                                 /* ENOMEM */
#include <stdlib.h>                                                /* free() */
#include <string.h>                                            /* strerror() */

#include "config.h"
#include "common.h"
#include "threads.h"
#include "mtime.h"

#include "video.h"
#include "video_output.h"

#include "video_common.h"
#include "transforms_common.h"
#include "transforms_yuv.h"

#include "intf_msg.h"

/*****************************************************************************
 * ConvertY4Gray8: grayscale YUV 4:x:x to RGB 8 bpp
 *****************************************************************************/
void ConvertY4Gray8( YUV_ARGS_8BPP )
{
    boolean_t   b_horizontal_scaling;             /* horizontal scaling type */
    int         i_vertical_scaling;                 /* vertical scaling type */
    int         i_x, i_y;                 /* horizontal and vertical indexes */
    int         i_scale_count;                       /* scale modulo counter */
    int         i_chroma_width;                    /* chroma width, not used */
    u8 *        p_gray;                             /* base conversion table */
    u8 *        p_pic_start;       /* beginning of the current line for copy */
    u8 *        p_buffer_start;                   /* conversion buffer start */
    u8 *        p_buffer;                       /* conversion buffer pointer */
    int *       p_offset_start;                        /* offset array start */
    int *       p_offset;                            /* offset array pointer */

    /*
     * Initialize some values  - i_pic_line_width will store the line skip
     */
    i_pic_line_width -= i_pic_width;
    p_gray =            p_vout->yuv.yuv.p_gray8;
    p_buffer_start =    p_vout->yuv.p_buffer;
    p_offset_start =    p_vout->yuv.p_offset;
    SetOffset( i_width, i_height, i_pic_width, i_pic_height,
               &b_horizontal_scaling, &i_vertical_scaling, p_offset_start, 0 );

    /*
     * Perform conversion
     */
    i_scale_count = ( i_vertical_scaling == 1 ) ? i_pic_height : i_height;
    for( i_y = 0; i_y < i_height; i_y++ )
    {
        /* Mark beginnning of line for possible later line copy, and initialize
         * buffer */
        p_pic_start =   p_pic;
        p_buffer =      b_horizontal_scaling ? p_buffer_start : p_pic;

        /* Do YUV conversion to buffer - YUV picture is always formed of 16
         * pixels wide blocks */
        for( i_x = i_width / 16; i_x--;  )
        {
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
        }

        /* Do horizontal and vertical scaling */
        SCALE_WIDTH;
        SCALE_HEIGHT(400, 1);
    }
}

/*****************************************************************************
 * ConvertYUV420RGB8: color YUV 4:2:0 to RGB 8 bpp
 *****************************************************************************/
void ConvertYUV420RGB8( YUV_ARGS_8BPP )
{
    boolean_t   b_horizontal_scaling;             /* horizontal scaling type */
    int         i_vertical_scaling;                 /* vertical scaling type */
    int         i_x, i_y;                 /* horizontal and vertical indexes */
    int         i_scale_count;                       /* scale modulo counter */
    int         i_real_y;                                           /* y % 4 */
    u8 *        p_lookup;                                    /* lookup table */
    int         i_chroma_width;                              /* chroma width */
    int *       p_offset_start;                        /* offset array start */
    int *       p_offset;                            /* offset array pointer */

    /* 
     * The dithering matrices
     */
    static int dither10[4] = {  0x0,  0x8,  0x2,  0xa };
    static int dither11[4] = {  0xc,  0x4,  0xe,  0x6 };
    static int dither12[4] = {  0x3,  0xb,  0x1,  0x9 };
    static int dither13[4] = {  0xf,  0x7,  0xd,  0x5 };

    static int dither20[4] = {  0x0, 0x10,  0x4, 0x14 };
    static int dither21[4] = { 0x18,  0x8, 0x1c,  0xc };
    static int dither22[4] = {  0x6, 0x16,  0x2, 0x12 };
    static int dither23[4] = { 0x1e,  0xe, 0x1a,  0xa };

    /*
     * Initialize some values  - i_pic_line_width will store the line skip
     */
    i_pic_line_width -= i_pic_width;
    i_chroma_width =    i_width / 2;
    p_offset_start =    p_vout->yuv.p_offset;
    p_lookup =          p_vout->yuv.p_base;
    SetOffset( i_width, i_height, i_pic_width, i_pic_height,
               &b_horizontal_scaling, &i_vertical_scaling, p_offset_start, 1 );

    /*
     * Perform conversion
     */
    i_scale_count = ( i_vertical_scaling == 1 ) ? i_pic_height : i_height;
    i_real_y = 0;
    for( i_y = 0; i_y < i_height; i_y++ )
    {
        /* Do horizontal and vertical scaling */
        SCALE_WIDTH_DITHER( 420 );
        SCALE_HEIGHT_DITHER( 420 );
    }
}

/*****************************************************************************
 * ConvertYUV422RGB8: color YUV 4:2:2 to RGB 8 bpp
 *****************************************************************************/
void ConvertYUV422RGB8( YUV_ARGS_8BPP )
{
    intf_ErrMsg( "yuv error: unhandled function, chroma = 422, bpp = 8" );
}

/*****************************************************************************
 * ConvertYUV444RGB8: color YUV 4:4:4 to RGB 8 bpp
 *****************************************************************************/
void ConvertYUV444RGB8( YUV_ARGS_8BPP )
{
    intf_ErrMsg( "yuv error: unhandled function, chroma = 444, bpp = 8" );
}

/*****************************************************************************
 * ConvertY4Gray16: grayscale YUV 4:x:x to RGB 2 Bpp
 *****************************************************************************/
void ConvertY4Gray16( YUV_ARGS_16BPP )
{
    boolean_t   b_horizontal_scaling;             /* horizontal scaling type */
    int         i_vertical_scaling;                 /* vertical scaling type */
    int         i_x, i_y;                 /* horizontal and vertical indexes */
    int         i_scale_count;                       /* scale modulo counter */
    int         i_chroma_width;                    /* chroma width, not used */
    u16 *       p_gray;                             /* base conversion table */
    u16 *       p_pic_start;       /* beginning of the current line for copy */
    u16 *       p_buffer_start;                   /* conversion buffer start */
    u16 *       p_buffer;                       /* conversion buffer pointer */
    int *       p_offset_start;                        /* offset array start */
    int *       p_offset;                            /* offset array pointer */

    /*
     * Initialize some values  - i_pic_line_width will store the line skip
     */
    i_pic_line_width -= i_pic_width;
    p_gray =            p_vout->yuv.yuv.p_gray16;
    p_buffer_start =    p_vout->yuv.p_buffer;
    p_offset_start =    p_vout->yuv.p_offset;
    SetOffset( i_width, i_height, i_pic_width, i_pic_height,
               &b_horizontal_scaling, &i_vertical_scaling, p_offset_start, 0 );

    /*
     * Perform conversion
     */
    i_scale_count = ( i_vertical_scaling == 1 ) ? i_pic_height : i_height;
    for( i_y = 0; i_y < i_height; i_y++ )
    {
        /* Mark beginnning of line for possible later line copy, and initialize
         * buffer */
        p_pic_start =   p_pic;
        p_buffer =      b_horizontal_scaling ? p_buffer_start : p_pic;

        /* Do YUV conversion to buffer - YUV picture is always formed of 16
         * pixels wide blocks */
        for( i_x = i_width / 16; i_x--;  )
        {
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
        }

        /* Do horizontal and vertical scaling */
        SCALE_WIDTH;
        SCALE_HEIGHT(400, 2);
    }
}

/*****************************************************************************
 * ConvertYUV420RGB16: color YUV 4:2:0 to RGB 2 Bpp
 *****************************************************************************/
void ConvertYUV420RGB16( YUV_ARGS_16BPP )
{
    boolean_t   b_horizontal_scaling;             /* horizontal scaling type */
    int         i_vertical_scaling;                 /* vertical scaling type */
    int         i_x, i_y;                 /* horizontal and vertical indexes */
    int         i_scale_count;                       /* scale modulo counter */
    int         i_uval, i_vval;                           /* U and V samples */
    int         i_red, i_green, i_blue;          /* U and V modified samples */
    int         i_chroma_width;                              /* chroma width */
    u16 *       p_yuv;                              /* base conversion table */
    u16 *       p_ybase;                     /* Y dependant conversion table */
    u16 *       p_pic_start;       /* beginning of the current line for copy */
    u16 *       p_buffer_start;                   /* conversion buffer start */
    u16 *       p_buffer;                       /* conversion buffer pointer */
    int *       p_offset_start;                        /* offset array start */
    int *       p_offset;                            /* offset array pointer */

    /*
     * Initialize some values  - i_pic_line_width will store the line skip
     */
    i_pic_line_width -= i_pic_width;
    i_chroma_width =    i_width / 2;
    p_yuv =             p_vout->yuv.yuv.p_rgb16;
    p_buffer_start =    p_vout->yuv.p_buffer;
    p_offset_start =    p_vout->yuv.p_offset;
    SetOffset( i_width, i_height, i_pic_width, i_pic_height,
               &b_horizontal_scaling, &i_vertical_scaling, p_offset_start, 0 );

    /*
     * Perform conversion
     */
    i_scale_count = ( i_vertical_scaling == 1 ) ? i_pic_height : i_height;
    for( i_y = 0; i_y < i_height; i_y++ )
    {
        /* Mark beginnning of line for possible later line copy, and initialize
         * buffer */
        p_pic_start =   p_pic;
        p_buffer =      b_horizontal_scaling ? p_buffer_start : p_pic;

        /* Do YUV conversion to buffer - YUV picture is always formed of 16
         * pixels wide blocks */
        for( i_x = i_width / 16; i_x--;  )
        {
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
        }

        /* Do horizontal and vertical scaling */
        SCALE_WIDTH;
        SCALE_HEIGHT(420, 2);
    }
}

/*****************************************************************************
 * ConvertYUV422RGB16: color YUV 4:2:2 to RGB 2 Bpp
 *****************************************************************************/
void ConvertYUV422RGB16( YUV_ARGS_16BPP )
{
    boolean_t   b_horizontal_scaling;             /* horizontal scaling type */
    int         i_vertical_scaling;                 /* vertical scaling type */
    int         i_x, i_y;                 /* horizontal and vertical indexes */
    int         i_scale_count;                       /* scale modulo counter */
    int         i_uval, i_vval;                           /* U and V samples */
    int         i_red, i_green, i_blue;          /* U and V modified samples */
    int         i_chroma_width;                              /* chroma width */
    u16 *       p_yuv;                              /* base conversion table */
    u16 *       p_ybase;                     /* Y dependant conversion table */
    u16 *       p_pic_start;       /* beginning of the current line for copy */
    u16 *       p_buffer_start;                   /* conversion buffer start */
    u16 *       p_buffer;                       /* conversion buffer pointer */
    int *       p_offset_start;                        /* offset array start */
    int *       p_offset;                            /* offset array pointer */

    /*
     * Initialize some values  - i_pic_line_width will store the line skip
     */
    i_pic_line_width -= i_pic_width;
    i_chroma_width =    i_width / 2;
    p_yuv =             p_vout->yuv.yuv.p_rgb16;
    p_buffer_start =    p_vout->yuv.p_buffer;
    p_offset_start =    p_vout->yuv.p_offset;
    SetOffset( i_width, i_height, i_pic_width, i_pic_height,
               &b_horizontal_scaling, &i_vertical_scaling, p_offset_start, 0 );

    /*
     * Perform conversion
     */
    i_scale_count = ( i_vertical_scaling == 1 ) ? i_pic_height : i_height;
    for( i_y = 0; i_y < i_height; i_y++ )
    {
        /* Mark beginnning of line for possible later line copy, and initialize
         * buffer */
        p_pic_start =   p_pic;
        p_buffer =      b_horizontal_scaling ? p_buffer_start : p_pic;

        /* Do YUV conversion to buffer - YUV picture is always formed of 16
         * pixels wide blocks */
        for( i_x = i_width / 16; i_x--;  )
        {
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_Y_PIXEL(2);
        }

        /* Do horizontal and vertical scaling */
        SCALE_WIDTH;
        SCALE_HEIGHT(422, 2);
    }
}

/*****************************************************************************
 * ConvertYUV444RGB16: color YUV 4:4:4 to RGB 2 Bpp
 *****************************************************************************/
void ConvertYUV444RGB16( YUV_ARGS_16BPP )
{
    boolean_t   b_horizontal_scaling;             /* horizontal scaling type */
    int         i_vertical_scaling;                 /* vertical scaling type */
    int         i_x, i_y;                 /* horizontal and vertical indexes */
    int         i_scale_count;                       /* scale modulo counter */
    int         i_uval, i_vval;                           /* U and V samples */
    int         i_red, i_green, i_blue;          /* U and V modified samples */
    int         i_chroma_width;                    /* chroma width, not used */
    u16 *       p_yuv;                              /* base conversion table */
    u16 *       p_ybase;                     /* Y dependant conversion table */
    u16 *       p_pic_start;       /* beginning of the current line for copy */
    u16 *       p_buffer_start;                   /* conversion buffer start */
    u16 *       p_buffer;                       /* conversion buffer pointer */
    int *       p_offset_start;                        /* offset array start */
    int *       p_offset;                            /* offset array pointer */

    /*
     * Initialize some values  - i_pic_line_width will store the line skip
     */
    i_pic_line_width -= i_pic_width;
    p_yuv =             p_vout->yuv.yuv.p_rgb16;
    p_buffer_start =    p_vout->yuv.p_buffer;
    p_offset_start =    p_vout->yuv.p_offset;
    SetOffset( i_width, i_height, i_pic_width, i_pic_height,
               &b_horizontal_scaling, &i_vertical_scaling, p_offset_start, 0 );

    /*
     * Perform conversion
     */
    i_scale_count = ( i_vertical_scaling == 1 ) ? i_pic_height : i_height;
    for( i_y = 0; i_y < i_height; i_y++ )
    {
        /* Mark beginnning of line for possible later line copy, and initialize
         * buffer */
        p_pic_start =   p_pic;
        p_buffer =      b_horizontal_scaling ? p_buffer_start : p_pic;

        /* Do YUV conversion to buffer - YUV picture is always formed of 16
         * pixels wide blocks */
        for( i_x = i_width / 16; i_x--;  )
        {
            CONVERT_YUV_PIXEL(2);  CONVERT_YUV_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_YUV_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_YUV_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_YUV_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_YUV_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_YUV_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_YUV_PIXEL(2);
            CONVERT_YUV_PIXEL(2);  CONVERT_YUV_PIXEL(2);
        }

        /* Do horizontal and vertical scaling */
        SCALE_WIDTH;
        SCALE_HEIGHT(444, 2);
    }
}

/*****************************************************************************
 * ConvertY4Gray24: grayscale YUV 4:x:x to RGB 2 Bpp
 *****************************************************************************/
void ConvertY4Gray24( YUV_ARGS_24BPP )
{
    intf_ErrMsg( "yuv error: unhandled function, grayscale, bpp = 24" );
}

/*****************************************************************************
 * ConvertYUV420RGB24: color YUV 4:2:0 to RGB 2 Bpp
 *****************************************************************************/
void ConvertYUV420RGB24( YUV_ARGS_24BPP )
{
    intf_ErrMsg( "yuv error: unhandled function, chroma = 420, bpp = 24" );
}

/*****************************************************************************
 * ConvertYUV422RGB24: color YUV 4:2:2 to RGB 2 Bpp
 *****************************************************************************/
void ConvertYUV422RGB24( YUV_ARGS_24BPP )
{
    intf_ErrMsg( "yuv error: unhandled function, chroma = 422, bpp = 24" );
}

/*****************************************************************************
 * ConvertYUV444RGB24: color YUV 4:4:4 to RGB 2 Bpp
 *****************************************************************************/
void ConvertYUV444RGB24( YUV_ARGS_24BPP )
{
    intf_ErrMsg( "yuv error: unhandled function, chroma = 444, bpp = 24" );
}

/*****************************************************************************
 * ConvertY4Gray32: grayscale YUV 4:x:x to RGB 4 Bpp
 *****************************************************************************/
void ConvertY4Gray32( YUV_ARGS_32BPP )
{
    boolean_t   b_horizontal_scaling;             /* horizontal scaling type */
    int         i_vertical_scaling;                 /* vertical scaling type */
    int         i_x, i_y;                 /* horizontal and vertical indexes */
    int         i_scale_count;                       /* scale modulo counter */
    int         i_chroma_width;                    /* chroma width, not used */
    u32 *       p_gray;                             /* base conversion table */
    u32 *       p_pic_start;       /* beginning of the current line for copy */
    u32 *       p_buffer_start;                   /* conversion buffer start */
    u32 *       p_buffer;                       /* conversion buffer pointer */
    int *       p_offset_start;                        /* offset array start */
    int *       p_offset;                            /* offset array pointer */

    /*
     * Initialize some values  - i_pic_line_width will store the line skip
     */
    i_pic_line_width -= i_pic_width;
    p_gray =            p_vout->yuv.yuv.p_gray32;
    p_buffer_start =    p_vout->yuv.p_buffer;
    p_offset_start =    p_vout->yuv.p_offset;
    SetOffset( i_width, i_height, i_pic_width, i_pic_height,
               &b_horizontal_scaling, &i_vertical_scaling, p_offset_start, 0 );

    /*
     * Perform conversion
     */
    i_scale_count = ( i_vertical_scaling == 1 ) ? i_pic_height : i_height;
    for( i_y = 0; i_y < i_height; i_y++ )
    {
        /* Mark beginnning of line for possible later line copy, and initialize
         * buffer */
        p_pic_start =   p_pic;
        p_buffer =      b_horizontal_scaling ? p_buffer_start : p_pic;

        /* Do YUV conversion to buffer - YUV picture is always formed of 16
         * pixels wide blocks */
        for( i_x = i_width / 16; i_x--;  )
        {
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
            *p_buffer++ = p_gray[ *p_y++ ]; *p_buffer++ = p_gray[ *p_y++ ];
        }

        /* Do horizontal and vertical scaling */
        SCALE_WIDTH;
        SCALE_HEIGHT(400, 4);
    }
}

/*****************************************************************************
 * ConvertYUV420RGB32: color YUV 4:2:0 to RGB 4 Bpp
 *****************************************************************************/
void ConvertYUV420RGB32( YUV_ARGS_32BPP )
{
    boolean_t   b_horizontal_scaling;             /* horizontal scaling type */
    int         i_vertical_scaling;                 /* vertical scaling type */
    int         i_x, i_y;                 /* horizontal and vertical indexes */
    int         i_scale_count;                       /* scale modulo counter */
    int         i_uval, i_vval;                           /* U and V samples */
    int         i_red, i_green, i_blue;          /* U and V modified samples */
    int         i_chroma_width;                              /* chroma width */
    u32 *       p_yuv;                              /* base conversion table */
    u32 *       p_ybase;                     /* Y dependant conversion table */
    u32 *       p_pic_start;       /* beginning of the current line for copy */
    u32 *       p_buffer_start;                   /* conversion buffer start */
    u32 *       p_buffer;                       /* conversion buffer pointer */
    int *       p_offset_start;                        /* offset array start */
    int *       p_offset;                            /* offset array pointer */

    /*
     * Initialize some values  - i_pic_line_width will store the line skip
     */
    i_pic_line_width -= i_pic_width;
    i_chroma_width =    i_width / 2;
    p_yuv =             p_vout->yuv.yuv.p_rgb32;
    p_buffer_start =    p_vout->yuv.p_buffer;
    p_offset_start =    p_vout->yuv.p_offset;
    SetOffset( i_width, i_height, i_pic_width, i_pic_height,
               &b_horizontal_scaling, &i_vertical_scaling, p_offset_start, 0 );

    /*
     * Perform conversion
     */
    i_scale_count = ( i_vertical_scaling == 1 ) ? i_pic_height : i_height;
    for( i_y = 0; i_y < i_height; i_y++ )
    {
        /* Mark beginnning of line for possible later line copy, and initialize
         * buffer */
        p_pic_start =   p_pic;
        p_buffer =      b_horizontal_scaling ? p_buffer_start : p_pic;

        /* Do YUV conversion to buffer - YUV picture is always formed of 16
         * pixels wide blocks */
        for( i_x = i_width / 16; i_x--;  )
        {
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
        }

        /* Do horizontal and vertical scaling */
        SCALE_WIDTH;
        SCALE_HEIGHT(420, 4);
    }
}

/*****************************************************************************
 * ConvertYUV422RGB32: color YUV 4:2:2 to RGB 4 Bpp
 *****************************************************************************/
void ConvertYUV422RGB32( YUV_ARGS_32BPP )
{
    boolean_t   b_horizontal_scaling;             /* horizontal scaling type */
    int         i_vertical_scaling;                 /* vertical scaling type */
    int         i_x, i_y;                 /* horizontal and vertical indexes */
    int         i_scale_count;                       /* scale modulo counter */
    int         i_uval, i_vval;                           /* U and V samples */
    int         i_red, i_green, i_blue;          /* U and V modified samples */
    int         i_chroma_width;                              /* chroma width */
    u32 *       p_yuv;                              /* base conversion table */
    u32 *       p_ybase;                     /* Y dependant conversion table */
    u32 *       p_pic_start;       /* beginning of the current line for copy */
    u32 *       p_buffer_start;                   /* conversion buffer start */
    u32 *       p_buffer;                       /* conversion buffer pointer */
    int *       p_offset_start;                        /* offset array start */
    int *       p_offset;                            /* offset array pointer */

    /*
     * Initialize some values  - i_pic_line_width will store the line skip
     */
    i_pic_line_width -= i_pic_width;
    i_chroma_width =    i_width / 2;
    p_yuv =             p_vout->yuv.yuv.p_rgb32;
    p_buffer_start =    p_vout->yuv.p_buffer;
    p_offset_start =    p_vout->yuv.p_offset;
    SetOffset( i_width, i_height, i_pic_width, i_pic_height,
               &b_horizontal_scaling, &i_vertical_scaling, p_offset_start, 0 );

    /*
     * Perform conversion
     */
    i_scale_count = ( i_vertical_scaling == 1 ) ? i_pic_height : i_height;
    for( i_y = 0; i_y < i_height; i_y++ )
    {
        /* Mark beginnning of line for possible later line copy, and initialize
         * buffer */
        p_pic_start =   p_pic;
        p_buffer =      b_horizontal_scaling ? p_buffer_start : p_pic;

        /* Do YUV conversion to buffer - YUV picture is always formed of 16
         * pixels wide blocks */
        for( i_x = i_width / 16; i_x--;  )
        {
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_Y_PIXEL(4);
        }

        /* Do horizontal and vertical scaling */
        SCALE_WIDTH;
        SCALE_HEIGHT(422, 4);
    }
}

/*****************************************************************************
 * ConvertYUV444RGB32: color YUV 4:4:4 to RGB 4 Bpp
 *****************************************************************************/
void ConvertYUV444RGB32( YUV_ARGS_32BPP )
{
    boolean_t   b_horizontal_scaling;             /* horizontal scaling type */
    int         i_vertical_scaling;                 /* vertical scaling type */
    int         i_x, i_y;                 /* horizontal and vertical indexes */
    int         i_scale_count;                       /* scale modulo counter */
    int         i_uval, i_vval;                           /* U and V samples */
    int         i_red, i_green, i_blue;          /* U and V modified samples */
    int         i_chroma_width;                    /* chroma width, not used */
    u32 *       p_yuv;                              /* base conversion table */
    u32 *       p_ybase;                     /* Y dependant conversion table */
    u32 *       p_pic_start;       /* beginning of the current line for copy */
    u32 *       p_buffer_start;                   /* conversion buffer start */
    u32 *       p_buffer;                       /* conversion buffer pointer */
    int *       p_offset_start;                        /* offset array start */
    int *       p_offset;                            /* offset array pointer */

    /*
     * Initialize some values  - i_pic_line_width will store the line skip
     */
    i_pic_line_width -= i_pic_width;
    p_yuv =             p_vout->yuv.yuv.p_rgb32;
    p_buffer_start =    p_vout->yuv.p_buffer;
    p_offset_start =    p_vout->yuv.p_offset;
    SetOffset( i_width, i_height, i_pic_width, i_pic_height,
               &b_horizontal_scaling, &i_vertical_scaling, p_offset_start, 0 );

    /*
     * Perform conversion
     */
    i_scale_count = ( i_vertical_scaling == 1 ) ? i_pic_height : i_height;
    for( i_y = 0; i_y < i_height; i_y++ )
    {
        /* Mark beginnning of line for possible later line copy, and initialize
         * buffer */
        p_pic_start =   p_pic;
        p_buffer =      b_horizontal_scaling ? p_buffer_start : p_pic;

        /* Do YUV conversion to buffer - YUV picture is always formed of 16
         * pixels wide blocks */
        for( i_x = i_width / 16; i_x--;  )
        {
            CONVERT_YUV_PIXEL(4);  CONVERT_YUV_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_YUV_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_YUV_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_YUV_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_YUV_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_YUV_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_YUV_PIXEL(4);
            CONVERT_YUV_PIXEL(4);  CONVERT_YUV_PIXEL(4);
        }

        /* Do horizontal and vertical scaling */
        SCALE_WIDTH;
        SCALE_HEIGHT(444, 4);
    }
}
