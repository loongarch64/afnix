// ---------------------------------------------------------------------------
// - cmem.hxx                                                                -
// - standard platform library - c memory function platform definitions      -
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
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define  AFNIX_HAVE_PAGESIZE
#define  AFNIX_HAVE_MPROTECT
#define  AFNIX_HAVE_MAPANONYMOUS
#define  AFNIX_HAVE_MREMAP
#endif

// solaris platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_SOLARIS)
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define  AFNIX_HAVE_SYSCONF
#endif

// freebsd platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_FREEBSD)
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define  AFNIX_HAVE_PAGESIZE
#define  AFNIX_HAVE_MPROTECT
#define  AFNIX_HAVE_MAPANON
#endif

// darwin platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_DARWIN)
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define  AFNIX_HAVE_PAGESIZE
#define  AFNIX_HAVE_MPROTECT
#define  AFNIX_HAVE_MAPANON
#endif

// gnu/freebsd platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_GNUKBSD)
#ifndef  _GNU_SOURCE
#define  _GNU_SOURCE
#endif
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define  AFNIX_HAVE_PAGESIZE
#define  AFNIX_HAVE_MPROTECT
#define  AFNIX_HAVE_MAPANON
#endif

// gnu platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_GNU)
#ifndef  _GNU_SOURCE
#define  _GNU_SOURCE
#endif
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define  AFNIX_HAVE_PAGESIZE
#define  AFNIX_HAVE_MAPANON
#endif

// android platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_ANDROID)
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define  AFNIX_HAVE_PAGESIZE
#define  AFNIX_HAVE_MPROTECT
#define  AFNIX_HAVE_MAPANON
#define  AFNIX_HAVE_MREMAP
#endif

