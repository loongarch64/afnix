// ---------------------------------------------------------------------------
// - cdir.hxx                                                                -
// - standard platform library - c directory function definition             -
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
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#define  AFNIX_PLATFORM_RDIR "/"
#define  AFNIX_PLATFORM_TDIR "/tmp"
#define  AFNIX_PLATFORM_DSEP '/'
#define  AFNIX_PLATFORM_HSEP '.'
#define  AFNIX_PLATFORM_ESEP '.'
#define  AFNIX_PLATFORM_LDIR "."
#define  AFNIX_PLATFORM_UDIR ".."
#define  AFNIX_PLATFORM_DDOT {AFNIX_PLATFORM_LDIR, AFNIX_PLATFORM_UDIR, nullptr}
#endif

// solaris platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_SOLARIS)
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#define  AFNIX_PLATFORM_RDIR "/"
#define  AFNIX_PLATFORM_TDIR "/tmp"
#define  AFNIX_PLATFORM_DSEP '/'
#define  AFNIX_PLATFORM_HSEP '.'
#define  AFNIX_PLATFORM_ESEP '.'
#define  AFNIX_PLATFORM_LDIR "."
#define  AFNIX_PLATFORM_UDIR ".."
#define  AFNIX_PLATFORM_DDOT {AFNIX_PLATFORM_LDIR, AFNIX_PLATFORM_UDIR, nullptr}
#endif

// freebsd platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_FREEBSD)
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#define  AFNIX_PLATFORM_RDIR "/"
#define  AFNIX_PLATFORM_TDIR "/tmp"
#define  AFNIX_PLATFORM_DSEP '/'
#define  AFNIX_PLATFORM_HSEP '.'
#define  AFNIX_PLATFORM_ESEP '.'
#define  AFNIX_PLATFORM_LDIR "."
#define  AFNIX_PLATFORM_UDIR ".."
#define  AFNIX_PLATFORM_DDOT {AFNIX_PLATFORM_LDIR, AFNIX_PLATFORM_UDIR, nullptr}
#endif

// darwin platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_DARWIN)
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#define  AFNIX_PLATFORM_RDIR "/"
#define  AFNIX_PLATFORM_TDIR "/tmp"
#define  AFNIX_PLATFORM_DSEP '/'
#define  AFNIX_PLATFORM_HSEP '.'
#define  AFNIX_PLATFORM_ESEP '.'
#define  AFNIX_PLATFORM_LDIR "."
#define  AFNIX_PLATFORM_UDIR ".."
#define  AFNIX_PLATFORM_DDOT {AFNIX_PLATFORM_LDIR, AFNIX_PLATFORM_UDIR, nullptr}
#endif

// gnu/freebsd platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_GNUKBSD)
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#define  AFNIX_PLATFORM_RDIR "/"
#define  AFNIX_PLATFORM_TDIR "/tmp"
#define  AFNIX_PLATFORM_DSEP '/'
#define  AFNIX_PLATFORM_HSEP '.'
#define  AFNIX_PLATFORM_ESEP '.'
#define  AFNIX_PLATFORM_LDIR "."
#define  AFNIX_PLATFORM_UDIR ".."
#define  AFNIX_PLATFORM_DDOT {AFNIX_PLATFORM_LDIR, AFNIX_PLATFORM_UDIR, nullptr}
#endif

// gnu platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_GNU)
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#define  AFNIX_PLATFORM_RDIR "/"
#define  AFNIX_PLATFORM_TDIR "/tmp"
#define  AFNIX_PLATFORM_DSEP '/'
#define  AFNIX_PLATFORM_HSEP '.'
#define  AFNIX_PLATFORM_ESEP '.'
#define  AFNIX_PLATFORM_LDIR "."
#define  AFNIX_PLATFORM_UDIR ".."
#define  AFNIX_PLATFORM_DDOT {AFNIX_PLATFORM_LDIR, AFNIX_PLATFORM_UDIR, nullptr}
#endif

// android platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_ANDROID)
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#define  AFNIX_PLATFORM_RDIR "/"
#define  AFNIX_PLATFORM_TDIR "/tmp"
#define  AFNIX_PLATFORM_DSEP '/'
#define  AFNIX_PLATFORM_HSEP '.'
#define  AFNIX_PLATFORM_ESEP '.'
#define  AFNIX_PLATFORM_LDIR "."
#define  AFNIX_PLATFORM_UDIR ".."
#define  AFNIX_PLATFORM_DDOT {AFNIX_PLATFORM_LDIR, AFNIX_PLATFORM_UDIR, nullptr}
#endif
