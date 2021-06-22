// ---------------------------------------------------------------------------
// - csys.hxx                                                                -
// - standard platform library - c system function platform definitions      -
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
#ifndef  _GNU_SOURCE
#define  _GNU_SOURCE
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define  AFNIX_PLATFORM_TYPE "unix"
#define  AFNIX_PLATFORM_OSEP '-'
#define  AFNIX_HAVE_HOSTNAME
#define  AFNIX_HAVE_UID
#define  AFNIX_HAVE_BACKTRACE
#endif

// solaris platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_SOLARIS)
#include <sys/systeminfo.h>
#include <sys/stat.h>
#include <pwd.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define  AFNIX_PLATFORM_TYPE "unix"
#define  AFNIX_PLATFORM_OSEP '-'
#define  AFNIX_HAVE_SYSINFO
#define  AFNIX_HAVE_UID
#endif

// freebsd platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_FREEBSD)
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define  AFNIX_PLATFORM_TYPE "unix"
#define  AFNIX_PLATFORM_OSEP '-'
#define  AFNIX_HAVE_HOSTNAME
#define  AFNIX_HAVE_UID
#endif

// darwin platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_DARWIN)
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define  AFNIX_PLATFORM_TYPE "darwin"
#define  AFNIX_PLATFORM_OSEP '-'
#define  AFNIX_HAVE_HOSTNAME
#define  AFNIX_HAVE_UID
#endif

// gnu/freebsd platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_GNUKBSD)
#ifndef  _GNU_SOURCE
#define  _GNU_SOURCE
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define  AFNIX_PLATFORM_TYPE "unix"
#define  AFNIX_PLATFORM_OSEP '-'
#define  AFNIX_HAVE_HOSTNAME
#define  AFNIX_HAVE_UID
#define  AFNIX_HAVE_BACKTRACE
#endif

// gnu platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_GNU)
#ifndef  _GNU_SOURCE
#define  _GNU_SOURCE
#endif
#include <sys/types.h>
#include <pwd.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define  AFNIX_PLATFORM_TYPE "unix"
#define  AFNIX_PLATFORM_OSEP '-'
#define  AFNIX_HAVE_HOSTNAME
#define  AFNIX_HAVE_UID
#define  AFNIX_HAVE_BACKTRACE
#endif

// android platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_ANDROID)
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define  AFNIX_PLATFORM_TYPE "android"
#define  AFNIX_PLATFORM_OSEP '-'
#define  AFNIX_HAVE_HOSTNAME
#define  AFNIX_HAVE_UID
#endif
