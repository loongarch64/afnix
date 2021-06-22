// ---------------------------------------------------------------------------
// - cclk.hxx                                                                -
// - standard platform library - c time native platform function definition  -
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
#include <sys/time.h>
#include <time.h>
#define  AFNIX_ATC_EPOCH 62167219200LL
#endif

// solaris platform - the initializers are a mess
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_SOLARIS)
#include <sys/time.h>
#include <time.h>
#define  AFNIX_ATC_EPOCH 62167219200LL
#endif

// freebsd platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_FREEBSD)
#include <sys/time.h>
#include <time.h>
#define  AFNIX_ATC_EPOCH 62167219200LL
#endif

// darwin platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_DARWIN)
#include <sys/time.h>
#include <time.h>
#define  AFNIX_ATC_EPOCH 62167219200LL
#endif

// linux platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_GNUKBSD)
#include <sys/time.h>
#include <time.h>
#define  AFNIX_ATC_EPOCH 62167219200LL
#endif

// gnu platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_GNU)
#include <sys/time.h>
#include <time.h>
#define  AFNIX_HAVE_RT
#define  AFNIX_ATC_EPOCH 62167219200LL
#endif

// android platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_ANDROID)
#include <sys/time.h>
#include <time.h>
#define  AFNIX_HAVE_RT
#define  AFNIX_ATC_EPOCH 62167219200LL
#endif
