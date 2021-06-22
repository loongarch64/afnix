// ---------------------------------------------------------------------------
// - ctrm.hxx                                                                -
// - standard platform library - c terminal platform native functions        -
// ---------------------------------------------------------------------------
// - This program is free software;  you can redistribute it  and/or  modify -
// - it provided that this copyright notice is kept intact.                  -
// -                                                                         -
// - This program  is  distributed in  the hope  that it will be useful, but -
// - without  any  warranty;  without  even   the   implied    warranty   of -
// - merchantability or fitness for a particular purpose.  In no event shall -
// - the copyright holder be liable for any  direct, indirect, incidental or -
// - special damages arising in any way out of the use of this software.     -
// ---------------------------------------------------------------------------
// - author (c) 1999-2021 amaury   darsch                                    -
// - author (c) 1999-2021 pino     toscano                     hurd platform -
// ---------------------------------------------------------------------------

// check for unknown platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_UNKNOWN)
#error "unknown platform type"
#endif

// check for unknown processor
#if (AFNIX_PLATFORM_PROCID == AFNIX_PROCTYPE_UNKNOWN)
#error "unknown processor type"
#endif

// linux platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_LINUX)
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <curses.h>
#include <term.h>
#define  AFNIX_HAVE_TERM
#define  AFNIX_HAVE_WINSIZE
#define  AFNIX_HAVE_DELTERM
#define  AFNIX_HAVE_NONCONST_TIGET
#endif

// solaris platform - the include files are completly messed up
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_SOLARIS)
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#if (AFNIX_PLATFORM_MAJOR == 5) && (AFNIX_PLATFORM_MINOR < 7)
#include <curses.h>
#include <term.h>
#else
#define __lint
#include <curses.h>
#include <term.h>
#undef __lint
#endif
#define  AFNIX_HAVE_TERM
#define  AFNIX_HAVE_WINSIZE
#define  AFNIX_HAVE_DELTERM
#define  AFNIX_HAVE_COMPLEX_TPARM
#define  AFNIX_HAVE_NONCONST_TIGET
#endif

// freebsd platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_FREEBSD)
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <curses.h>
#include <term.h>
#ifdef bool
#undef bool
#endif
#define  AFNIX_HAVE_TERM
#define  AFNIX_HAVE_WINSIZE
#define  AFNIX_HAVE_NONCONST_TIGET
#endif

// darwin platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_DARWIN)
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <curses.h>
#include <term.h>
#ifdef bool
#undef bool
#endif
#define  AFNIX_HAVE_TERM
#define  AFNIX_DELETE_ISBS
#define  AFNIX_HAVE_WINSIZE
#define  AFNIX_HAVE_NONCONST_TIGET
#endif

// gnu/freebsd platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_GNUKBSD)
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <curses.h>
#include <term.h>
#define  AFNIX_HAVE_TERM
#define  AFNIX_HAVE_WINSIZE
#define  AFNIX_HAVE_DELTERM
#define  AFNIX_HAVE_NONCONST_TIGET
#endif

// gnu platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_GNU)
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <curses.h>
#include <term.h>
#define  AFNIX_HAVE_TERM
#define  AFNIX_HAVE_WINSIZE
#define  AFNIX_HAVE_DELTERM
#define  AFNIX_HAVE_NONCONST_TIGET
#endif

// android platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_ANDROID)
#endif
