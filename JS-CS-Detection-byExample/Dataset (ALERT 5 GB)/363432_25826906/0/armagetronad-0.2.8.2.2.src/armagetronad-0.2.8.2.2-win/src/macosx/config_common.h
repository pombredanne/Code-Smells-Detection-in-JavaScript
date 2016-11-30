/* config.h.  Generated by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* #define MACOSX_VERSION_10_2 1 */

/* AIX */
/* #undef AIX */

/* BeOS */
/* #undef BEOS */

/* Include pthread support for binary relocation? */
/* #undef BR_PTHREAD */

/* Define if your system deos not like the pointer tricks in eWall.h. */
/* #undef CAUTION_WALL */

/* Define if you wish to compile a dedicated server */
/* #undef DEDICATED */

/* Define if you wish to use the old and dirty OpenGL initialization method */
/* #undef DIRTY */

/* Define if you dont want to use a custom memory manager. */
#define DONTUSEMEMMANAGER 1

/* Use binary relocation? */
/* #undef ENABLE_BINRELOC */

/* Define to 1 if you have the `atan2f' function. */
#ifndef MACOSX_VERSION_10_2
#define HAVE_ATAN2F 1
#endif

/* Define to 1 if you have the `cosf' function. */
#ifndef MACOSX_VERSION_10_2
#define HAVE_COSF 1
#endif

/* Define to 1 if you have the `fabsf' function. */
#define HAVE_FABSF 1

/* Define to 1 if you have the `floorf' function. */
#define HAVE_FLOORF 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `jpeg' library (-ljpeg). */
/* #undef HAVE_LIBJPEG */

/* Define to 1 if you have the `pthread' library (-lpthread). */
/* #undef HAVE_LIBPTHREAD */

/* Define to 1 if you have the `wsock32' library (-lwsock32). */
/* #undef HAVE_LIBWSOCK32 */

/* Define if you have the X11 library (-lX11). */
/* #undef HAVE_LIBX11 */

/* Define to 1 if you have the `xml2' library (-lxml2). */
#define HAVE_LIBXML2 1

/* Define if your xml2 library is obsolete (lacks
   xmlParserInputBufferCreateFilenameDefault). */
/* #ifndef MACOSX_VERSION_10_2
 * #define HAVE_LIBXML2_WO_PIBCREATE 1
 * #endif
 */ 

/* Define to 1 if you have the `z' library (-lz). */
#define HAVE_LIBZ 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the `sinf' function. */
#ifndef MACOSX_VERSION_10_2
#define HAVE_SINF 1
#endif

/* Define if you have the type socklen_t. */
#ifndef MACOSX_VERSION_10_2
#define HAVE_SOCKLEN_T 1
#endif

/* Define to 1 if you have the `sqrtf' function. */
#ifndef MACOSX_VERSION_10_2
#define HAVE_SQRTF 1
#endif

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the `tanf' function. */
#ifndef MACOSX_VERSION_10_2
#define HAVE_TANF 1
#endif

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `wmemset' function. */
#ifndef MACOSX_VERSION_10_2
#define HAVE_WMEMSET 1
#endif

/* Define to 1 if you have the `logf' function. */
#ifndef MACOSX_VERSION_10_2
#define HAVE_LOGF 1
#endif

/* Define to 1 if you have the `expf' function. */
#ifndef MACOSX_VERSION_10_2
#define HAVE_EXPF 1
#endif

/* enables krawall */
/* #undef KRAWALL */

/* enables krawall server */
/* #undef KRAWALL_SERVER */

/* GNU/Linux */
/* #undef LINUX */

/* Mac OS X */
#define MACOSX 1

/* The size of a `Enum', as computed by sizeof. */
#define SIZEOF_ENUM 4

/* The size of a `int ', as computed by sizeof. */
#define SIZEOF_INT_ 4

/* Solaris */
/* #undef SOLARIS */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "0.2.8.2.2"

/* Windows 9x/NT/2k/XP */
/* #undef WIN32 */

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */
