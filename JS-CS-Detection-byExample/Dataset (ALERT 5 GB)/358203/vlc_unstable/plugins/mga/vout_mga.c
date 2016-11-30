/*****************************************************************************
 * vout_mga.c: MGA video output display method
 *****************************************************************************
 * Copyright (C) 1998, 1999, 2000 VideoLAN
 *
 * Authors:
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#include "defs.h"

#include <errno.h>                                                 /* ENOMEM */
#include <stdlib.h>                                                /* free() */
#include <string.h>                                            /* strerror() */
#include <fcntl.h>                                                 /* open() */
#include <sys/ioctl.h>                                            /* ioctl() */
#include <sys/mman.h>                                          /* PROT_WRITE */

#ifdef SYS_BSD
#include <sys/types.h>                                     /* typedef ushort */
#endif

#include <sys/shm.h>                                   /* shmget(), shmctl() */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>

#include "config.h"
#include "common.h"
#include "threads.h"
#include "mtime.h"
#include "plugins.h"

#include "video.h"
#include "video_output.h"

#include "intf_msg.h"

#include "vout_mga.h"

/*****************************************************************************
 * vout_MGACreate: allocate X11 video thread output method
 *****************************************************************************
 * This function allocate and initialize a X11 vout method. It uses some of the
 * vout properties to choose the window size, and change them according to the
 * actual properties of the display.
 *****************************************************************************/
int vout_MGACreate( vout_thread_t *p_vout, char *psz_display,
                    int i_root_window, void *p_data )
{
    /* Allocate structure */
    p_vout->p_sys = malloc( sizeof( vout_sys_t ) );
    if( p_vout->p_sys == NULL )
    {
        intf_ErrMsg("error: %s", strerror(ENOMEM) );
        return( 1 );
    }

    /* Allocate MGA configuration structure */
    p_vout->p_sys->p_mga = malloc( sizeof( mga_vid_config_t ) );
    if( p_vout->p_sys->p_mga == NULL )
    {
        intf_ErrMsg("error: %s", strerror(ENOMEM) );
        free( p_vout->p_sys );
        return( 1 );
    }

    if( (p_vout->p_sys->i_fd = open("/dev/mga_vid",O_RDWR)) == -1 )
    {
        intf_ErrMsg("error: can't open MGA driver /dev/mga_vid" );
        free( p_vout->p_sys->p_mga );
        free( p_vout->p_sys );
        return( 1 );
    }

    /* Open and initialize device. This function issues its own error messages.
     * Since XLib is usually not thread-safe, we can't use the same display
     * pointer than the interface or another thread. However, the root window
     * id is still valid. */
    if( X11OpenDisplay( p_vout, psz_display, i_root_window ) )
    {
        intf_ErrMsg("error: can't initialize X11 display" );
        free( p_vout->p_sys->p_mga );
        free( p_vout->p_sys );
        return( 1 );
    }

    return( 0 );
}

/*****************************************************************************
 * vout_MGAInit: initialize X11 video thread output method
 *****************************************************************************
 * This function create the XImages needed by the output thread. It is called
 * at the beginning of the thread, but also each time the window is resized.
 *****************************************************************************/
int vout_MGAInit( vout_thread_t *p_vout )
{
    int i_err;

    /* create the MGA output */
    p_vout->p_sys->p_mga->src_width = p_vout->i_width;
    p_vout->p_sys->p_mga->src_height = p_vout->i_height;
    /* FIXME: we should initialize these ones according to the streams */
    p_vout->p_sys->p_mga->dest_width = p_vout->i_width;
    p_vout->p_sys->p_mga->dest_height = p_vout->i_height;
    //p_vout->p_sys->p_mga->dest_width = 400;
    //p_vout->p_sys->p_mga->dest_height = 300;
    p_vout->p_sys->p_mga->x_org = 100;
    p_vout->p_sys->p_mga->y_org = 100;
    p_vout->p_sys->p_mga->colkey_on = 0;

    if( ioctl(p_vout->p_sys->i_fd, MGA_VID_CONFIG, p_vout->p_sys->p_mga) )
    {
        intf_ErrMsg("error in config ioctl");
    }

    if (p_vout->p_sys->p_mga->card_type == MGA_G200)
    {
        intf_Msg( "vout: detected MGA G200 (%d MB Ram)",
                  p_vout->p_sys->p_mga->ram_size );
        p_vout->p_sys->b_g400 = 0;
    }
    else
    {
        intf_Msg( "vout: detected MGA G400 (%d MB Ram)",
                  p_vout->p_sys->p_mga->ram_size );
        p_vout->p_sys->b_g400 = 1;
    }

    ioctl( p_vout->p_sys->i_fd, MGA_VID_ON, 0 );

    p_vout->p_sys->i_size = ( (p_vout->p_sys->p_mga->src_width + 31) & ~31 )
                             * p_vout->p_sys->p_mga->src_height;

    p_vout->p_sys->p_mga_vid_base = mmap( 0, p_vout->p_sys->i_size
                                             + p_vout->p_sys->i_size / 2,
                                          PROT_WRITE, MAP_SHARED,
                                          p_vout->p_sys->i_fd, 0 );

    memset( p_vout->p_sys->p_mga_vid_base,
            0x00, p_vout->p_sys->i_size );

    memset( p_vout->p_sys->p_mga_vid_base + p_vout->p_sys->i_size,
            0x80, p_vout->p_sys->i_size / 2 );

    /* Create XImages using XShm extension - on failure, fall back to regular
     * way (and destroy the first image if it was created successfully) */
    if( p_vout->p_sys->b_shm )
    {
        /* Create first image */
        i_err = X11CreateShmImage( p_vout, &p_vout->p_sys->p_ximage[0],
                                   &p_vout->p_sys->shm_info[0] );
        if( !i_err )                         /* first image has been created */
        {
            /* Create second image */
            if( X11CreateShmImage( p_vout, &p_vout->p_sys->p_ximage[1],
                                   &p_vout->p_sys->shm_info[1] ) )
            {                             /* error creating the second image */
                X11DestroyShmImage( p_vout, p_vout->p_sys->p_ximage[0],
                                    &p_vout->p_sys->shm_info[0] );
                i_err = 1;
            }
        }
        if( i_err )                                      /* an error occured */
        {
            intf_Msg("vout: XShm video sextension deactivated" );
            p_vout->p_sys->b_shm = 0;
        }
    }

    /* Create XImages without XShm extension */
    if( !p_vout->p_sys->b_shm )
    {
        if( X11CreateImage( p_vout, &p_vout->p_sys->p_ximage[0] ) )
        {
            intf_ErrMsg("error: can't create images");
            p_vout->p_sys->p_ximage[0] = NULL;
            p_vout->p_sys->p_ximage[1] = NULL;
            return( 1 );
        }
        if( X11CreateImage( p_vout, &p_vout->p_sys->p_ximage[1] ) )
        {
            intf_ErrMsg("error: can't create images");
            X11DestroyImage( p_vout->p_sys->p_ximage[0] );
            p_vout->p_sys->p_ximage[0] = NULL;
            p_vout->p_sys->p_ximage[1] = NULL;
            return( 1 );
        }
    }

    /* Set bytes per line and initialize buffers */
    p_vout->i_bytes_per_line = p_vout->p_sys->p_ximage[0]->bytes_per_line;
    vout_SetBuffers( p_vout, p_vout->p_sys->p_ximage[ 0 ]->data,
                     p_vout->p_sys->p_ximage[ 1 ]->data );
    return( 0 );
}

/*****************************************************************************
 * vout_MGAEnd: terminate X11 video thread output method
 *****************************************************************************
 * Destroy the X11 XImages created by vout_MGAInit. It is called at the end of
 * the thread, but also each time the window is resized.
 *****************************************************************************/
void vout_MGAEnd( vout_thread_t *p_vout )
{
    if( p_vout->p_sys->b_shm )                             /* Shm XImages... */
    {
        X11DestroyShmImage( p_vout, p_vout->p_sys->p_ximage[0],
                            &p_vout->p_sys->shm_info[0] );
        X11DestroyShmImage( p_vout, p_vout->p_sys->p_ximage[1],
                            &p_vout->p_sys->shm_info[1] );
    }
    else                                          /* ...or regular XImages */
    {
        X11DestroyImage( p_vout->p_sys->p_ximage[0] );
        X11DestroyImage( p_vout->p_sys->p_ximage[1] );
    }
}

/*****************************************************************************
 * vout_MGADestroy: destroy X11 video thread output method
 *****************************************************************************
 * Terminate an output method created by vout_CreateOutputMethod
 *****************************************************************************/
void vout_MGADestroy( vout_thread_t *p_vout )
{
    X11CloseDisplay( p_vout );

    ioctl( p_vout->p_sys->i_fd, MGA_VID_OFF, 0 );
    close( p_vout->p_sys->i_fd );

    free( p_vout->p_sys->p_mga );
    free( p_vout->p_sys );
}

/*****************************************************************************
 * vout_MGAManage: handle X11 events
 *****************************************************************************
 * This function should be called regularly by video output thread. It manages
 * X11 events and allows window resizing. It returns a non null value on
 * error.
 *****************************************************************************/
int vout_MGAManage( vout_thread_t *p_vout )
{
    /*
     * Color/Grayscale or gamma change: in 8bpp, just change the colormap
     */
    if( (p_vout->i_changes & VOUT_GRAYSCALE_CHANGE) && (p_vout->i_screen_depth == 8) )
    {
        /* FIXME: clear flags ?? */
    }

    /*
     * Size change
     */
    if( p_vout->i_changes & VOUT_SIZE_CHANGE )
    {
        intf_DbgMsg("resizing window");
        p_vout->i_changes &= ~VOUT_SIZE_CHANGE;

        /* Resize window */
        XResizeWindow( p_vout->p_sys->p_display, p_vout->p_sys->window,
                       p_vout->i_width, p_vout->i_height );

        /* Destroy XImages to change their size */
        vout_MGAEnd( p_vout );

        /* Recreate XImages. If SysInit failed, the thread can't go on. */
        if( vout_MGAInit( p_vout ) )
        {
            intf_ErrMsg("error: can't resize display");
            return( 1 );
        }

        /* Tell the video output thread that it will need to rebuild YUV
         * tables. This is needed since convertion buffer size may have changed */
        p_vout->i_changes |= VOUT_YUV_CHANGE;
        intf_Msg("vout: video display resized (%dx%d)", p_vout->i_width, p_vout->i_height);
    }

    return 0;
}

/*****************************************************************************
 * vout_MGADisplay: displays previously rendered output
 *****************************************************************************
 * This function send the currently rendered image to X11 server, wait until
 * it is displayed and switch the two rendering buffer, preparing next frame.
 *****************************************************************************/
void vout_MGADisplay( vout_thread_t *p_vout )
{
    if( p_vout->p_sys->b_shm)                                /* XShm is used */
    {
        /* Display rendered image using shared memory extension */
        XShmPutImage(p_vout->p_sys->p_display, p_vout->p_sys->window, p_vout->p_sys->gc,
                     p_vout->p_sys->p_ximage[ p_vout->i_buffer_index ],
                     0, 0, 0, 0,
                     p_vout->p_sys->p_ximage[ p_vout->i_buffer_index ]->width,
                     p_vout->p_sys->p_ximage[ p_vout->i_buffer_index ]->height, True);

        /* Send the order to the X server */
        XFlush(p_vout->p_sys->p_display);
    }
    else                                /* regular X11 capabilities are used */
    {
        XPutImage(p_vout->p_sys->p_display, p_vout->p_sys->window, p_vout->p_sys->gc,
                  p_vout->p_sys->p_ximage[ p_vout->i_buffer_index ],
                  0, 0, 0, 0,
                  p_vout->p_sys->p_ximage[ p_vout->i_buffer_index ]->width,
                  p_vout->p_sys->p_ximage[ p_vout->i_buffer_index ]->height);

        /* Send the order to the X server */
        XFlush(p_vout->p_sys->p_display);
    }
}

/* following functions are local */

/*****************************************************************************
 * X11OpenDisplay: open and initialize X11 device
 *****************************************************************************
 * Create a window according to video output given size, and set other
 * properties according to the display properties.
 *****************************************************************************/
static int X11OpenDisplay( vout_thread_t *p_vout, char *psz_display, Window root_window )
{
    XPixmapFormatValues *       p_xpixmap_format;          /* pixmap formats */
    XVisualInfo *               p_xvisual;           /* visuals informations */
    XVisualInfo                 xvisual_template;         /* visual template */
    int                         i_count;                       /* array size */

    /* Open display */
    p_vout->p_sys->p_display = XOpenDisplay( psz_display );
    if( p_vout->p_sys->p_display == NULL )
    {
        intf_ErrMsg("error: can't open display %s", psz_display );
        return( 1 );
    }

    /* Initialize structure */
    p_vout->p_sys->root_window  = root_window;
    p_vout->p_sys->b_shm        = (XShmQueryExtension(p_vout->p_sys->p_display) == True);
    p_vout->p_sys->i_screen     = DefaultScreen( p_vout->p_sys->p_display );
    if( !p_vout->p_sys->b_shm )
    {
        intf_Msg("vout: XShm video extension is not available");
    }

    /* Get screen depth */
    p_vout->i_screen_depth = XDefaultDepth( p_vout->p_sys->p_display, p_vout->p_sys->i_screen );
    switch( p_vout->i_screen_depth )
    {
    case 8:
        /*
         * Screen depth is 8bpp. Use PseudoColor visual with private colormap.
         */
        xvisual_template.screen =   p_vout->p_sys->i_screen;
        xvisual_template.class =    DirectColor;
        p_xvisual = XGetVisualInfo( p_vout->p_sys->p_display, VisualScreenMask | VisualClassMask,
                                    &xvisual_template, &i_count );
        if( p_xvisual == NULL )
        {
            intf_ErrMsg("error: no PseudoColor visual available");
            XCloseDisplay( p_vout->p_sys->p_display );
            return( 1 );
        }
        /* FIXME: SetColormap; ?? */
        p_vout->i_bytes_per_pixel = 1;
        break;
    case 15:
    case 16:
    case 24:
    default:
        /*
         * Screen depth is higher than 8bpp. TrueColor visual is used.
         */
        xvisual_template.screen =   p_vout->p_sys->i_screen;
        xvisual_template.class =    TrueColor;
        p_xvisual = XGetVisualInfo( p_vout->p_sys->p_display, VisualScreenMask | VisualClassMask,
                                    &xvisual_template, &i_count );
        if( p_xvisual == NULL )
        {
            intf_ErrMsg("error: no TrueColor visual available");
            XCloseDisplay( p_vout->p_sys->p_display );
            return( 1 );
        }
        p_vout->i_red_mask =        p_xvisual->red_mask;
        p_vout->i_green_mask =      p_xvisual->green_mask;
        p_vout->i_blue_mask =       p_xvisual->blue_mask;

        /* There is no difference yet between 3 and 4 Bpp. The only way to find
         * the actual number of bytes per pixel is to list supported pixmap
         * formats. */
        p_xpixmap_format = XListPixmapFormats( p_vout->p_sys->p_display, &i_count );
        p_vout->i_bytes_per_pixel = 0;
        for( ; i_count--; p_xpixmap_format++ )
        {
            if( p_xpixmap_format->bits_per_pixel / 8 > p_vout->i_bytes_per_pixel )
            {
                p_vout->i_bytes_per_pixel = p_xpixmap_format->bits_per_pixel / 8;
            }
        }
        break;
    }
    p_vout->p_sys->p_visual = p_xvisual->visual;
    XFree( p_xvisual );

    /* Create a window */
    if( X11CreateWindow( p_vout ) )
    {
        intf_ErrMsg("error: can't open a window");
        XCloseDisplay( p_vout->p_sys->p_display );
        return( 1 );
    }
    return( 0 );
}

/*****************************************************************************
 * X11CloseDisplay: close X11 device
 *****************************************************************************
 * Returns all resources allocated by X11OpenDisplay and restore the original
 * state of the display.
 *****************************************************************************/
static void X11CloseDisplay( vout_thread_t *p_vout )
{
    /* Destroy colormap */
    if( p_vout->i_screen_depth == 8 )
    {
        XFreeColormap( p_vout->p_sys->p_display, p_vout->p_sys->colormap );
    }

    /* Destroy window and close display */
    X11DestroyWindow( p_vout );
    XCloseDisplay( p_vout->p_sys->p_display );
}

/*****************************************************************************
 * X11CreateWindow: create X11 vout window
 *****************************************************************************
 * The video output window will be created. Normally, this window is wether
 * full screen or part of a parent window. Therefore, it does not need a
 * title or other hints. Thery are still supplied in case the window would be
 * spawned as a standalone one by the interface.
 *****************************************************************************/
static int X11CreateWindow( vout_thread_t *p_vout )
{
    XSetWindowAttributes    xwindow_attributes;         /* window attributes */
    XGCValues               xgcvalues;      /* graphic context configuration */
    XEvent                  xevent;                          /* first events */
    boolean_t               b_expose;             /* 'expose' event received */
    boolean_t               b_map_notify;     /* 'map_notify' event received */

    /* Prepare window attributes */
    xwindow_attributes.backing_store = Always;       /* save the hidden part */

    /* Create the window and set hints */
    p_vout->p_sys->window = XCreateSimpleWindow( p_vout->p_sys->p_display,
                                         p_vout->p_sys->root_window,
                                         0, 0,
                                         p_vout->i_width, p_vout->i_height,
                                         0, 0, 0);
    XSelectInput( p_vout->p_sys->p_display, p_vout->p_sys->window,
                  ExposureMask | StructureNotifyMask );
    XChangeWindowAttributes( p_vout->p_sys->p_display, p_vout->p_sys->window,
                             CWBackingStore, &xwindow_attributes);

    /* Creation of a graphic context that doesn't generate a GraphicsExpose event
       when using functions like XCopyArea */
    xgcvalues.graphics_exposures = False;
    p_vout->p_sys->gc =  XCreateGC( p_vout->p_sys->p_display, p_vout->p_sys->window,
                                    GCGraphicsExposures, &xgcvalues);

    /* Send orders to server, and wait until window is displayed - two events
     * must be received: a MapNotify event, an Expose event allowing drawing in the
     * window */
    b_expose = 0;
    b_map_notify = 0;
    XMapWindow( p_vout->p_sys->p_display, p_vout->p_sys->window);
    do
    {
        XNextEvent( p_vout->p_sys->p_display, &xevent);
        if( (xevent.type == Expose)
            && (xevent.xexpose.window == p_vout->p_sys->window) )
        {
            b_expose = 1;
        }
        else if( (xevent.type == MapNotify)
                 && (xevent.xmap.window == p_vout->p_sys->window) )
        {
            b_map_notify = 1;
        }
    }
    while( !( b_expose && b_map_notify ) );
    XSelectInput( p_vout->p_sys->p_display, p_vout->p_sys->window, 0 );

    /* At this stage, the window is openned, displayed, and ready to receive
     * data */
    return( 0 );
}

/*****************************************************************************
 * X11DestroyWindow: destroy X11 window
 *****************************************************************************
 * Destroy an X11 window created by vout_CreateWindow
 *****************************************************************************/
static void X11DestroyWindow( vout_thread_t *p_vout )
{
    XUnmapWindow( p_vout->p_sys->p_display, p_vout->p_sys->window );
    XFreeGC( p_vout->p_sys->p_display, p_vout->p_sys->gc );
    XDestroyWindow( p_vout->p_sys->p_display, p_vout->p_sys->window );
}

/*****************************************************************************
 * X11CreateImage: create an XImage
 *****************************************************************************
 * Create a simple XImage used as a buffer.
 *****************************************************************************/
static int X11CreateImage( vout_thread_t *p_vout, XImage **pp_ximage )
{
    byte_t *    pb_data;                          /* image data storage zone */
    int         i_quantum;                     /* XImage quantum (see below) */

    /* Allocate memory for image */
    p_vout->i_bytes_per_line = p_vout->i_width * p_vout->i_bytes_per_pixel;
    pb_data = (byte_t *) malloc( p_vout->i_bytes_per_line * p_vout->i_height );
    if( !pb_data )                                                  /* error */
    {
        intf_ErrMsg("error: %s", strerror(ENOMEM));
        return( 1 );
    }

    /* Optimize the quantum of a scanline regarding its size - the quantum is
       a diviser of the number of bits between the start of two scanlines. */
    if( !(( p_vout->i_bytes_per_line ) % 32) )
    {
        i_quantum = 32;
    }
    else
    {
        if( !(( p_vout->i_bytes_per_line ) % 16) )
        {
            i_quantum = 16;
        }
        else
        {
            i_quantum = 8;
        }
    }

    /* Create XImage */
    *pp_ximage = XCreateImage( p_vout->p_sys->p_display, p_vout->p_sys->p_visual,
                               p_vout->i_screen_depth, ZPixmap, 0, pb_data,
                               p_vout->i_width, p_vout->i_height, i_quantum, 0);
    if(! *pp_ximage )                                               /* error */
    {
        intf_ErrMsg( "error: XCreateImage() failed" );
        free( pb_data );
        return( 1 );
    }

    return 0;
}

/*****************************************************************************
 * X11CreateShmImage: create an XImage using shared memory extension
 *****************************************************************************
 * Prepare an XImage for DisplayX11ShmImage function.
 * The order of the operations respects the recommandations of the mit-shm
 * document by J.Corbet and K.Packard. Most of the parameters were copied from
 * there.
 *****************************************************************************/
static int X11CreateShmImage( vout_thread_t *p_vout, XImage **pp_ximage,
                              XShmSegmentInfo *p_shm_info)
{
    /* Create XImage */
    *pp_ximage = XShmCreateImage( p_vout->p_sys->p_display, p_vout->p_sys->p_visual,
                                  p_vout->i_screen_depth, ZPixmap, 0,
                                  p_shm_info, p_vout->i_width, p_vout->i_height );
    if(! *pp_ximage )                                               /* error */
    {
        intf_ErrMsg("error: XShmCreateImage() failed");
        return( 1 );
    }

    /* Allocate shared memory segment - 0777 set the access permission
     * rights (like umask), they are not yet supported by X servers */
    p_shm_info->shmid = shmget( IPC_PRIVATE,
                                (*pp_ximage)->bytes_per_line * (*pp_ximage)->height,
                                IPC_CREAT | 0777);
    if( p_shm_info->shmid < 0)                                      /* error */
    {
        intf_ErrMsg("error: can't allocate shared image data (%s)",
                    strerror(errno));
        XDestroyImage( *pp_ximage );
        return( 1 );
    }

    /* Attach shared memory segment to process (read/write) */
    p_shm_info->shmaddr = (*pp_ximage)->data = shmat(p_shm_info->shmid, 0, 0);
    if(! p_shm_info->shmaddr )
    {                                                               /* error */
        intf_ErrMsg("error: can't attach shared memory (%s)",
                    strerror(errno));
        shmctl( p_shm_info->shmid, IPC_RMID, 0 );      /* free shared memory */
        XDestroyImage( *pp_ximage );
        return( 1 );
    }

    /* Mark the shm segment to be removed when there will be no more
     * attachements, so it is automatic on process exit or after shmdt */
    shmctl( p_shm_info->shmid, IPC_RMID, 0 );

    /* Attach shared memory segment to X server (read only) */
    p_shm_info->readOnly = True;
    if( XShmAttach( p_vout->p_sys->p_display, p_shm_info ) == False )    /* error */
    {
        intf_ErrMsg("error: can't attach shared memory to X11 server");
        shmdt( p_shm_info->shmaddr );     /* detach shared memory from process
                                           * and automatic free                */
        XDestroyImage( *pp_ximage );
        return( 1 );
    }

    /* Send image to X server. This instruction is required, since having
     * built a Shm XImage and not using it causes an error on XCloseDisplay */
    XFlush( p_vout->p_sys->p_display );
    return( 0 );
}

/*****************************************************************************
 * X11DestroyImage: destroy an XImage
 *****************************************************************************
 * Destroy XImage AND associated data. If pointer is NULL, the image won't be
 * destroyed (see vout_ManageOutputMethod())
 *****************************************************************************/
static void X11DestroyImage( XImage *p_ximage )
{
    if( p_ximage != NULL )
    {
        XDestroyImage( p_ximage );                     /* no free() required */
    }
}

/*****************************************************************************
 * X11DestroyShmImage
 *****************************************************************************
 * Destroy XImage AND associated data. Detach shared memory segment from
 * server and process, then free it. If pointer is NULL, the image won't be
 * destroyed (see vout_ManageOutputMethod())
 *****************************************************************************/
static void X11DestroyShmImage( vout_thread_t *p_vout, XImage *p_ximage,
                                XShmSegmentInfo *p_shm_info )
{
    /* If pointer is NULL, do nothing */
    if( p_ximage == NULL )
    {
        return;
    }

    XShmDetach( p_vout->p_sys->p_display, p_shm_info );     /* detach from server */
    XDestroyImage( p_ximage );
    if( shmdt( p_shm_info->shmaddr ) )  /* detach shared memory from process */
    {                                   /* also automatic freeing...         */
        intf_ErrMsg("error: can't detach shared memory (%s)",
                    strerror(errno));
    }
}
