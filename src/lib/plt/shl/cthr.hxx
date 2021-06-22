// ---------------------------------------------------------------------------
// - cthr.hxx                                                                -
// - standard platform library - c thread function platform definitions      -
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
#include <pthread.h>
#include <signal.h>
#define  AFNIX_PTHREAD_ONCE_INIT         PTHREAD_ONCE_INIT
#define  AFNIX_PTHREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#define  AFNIX_PTHREAD_COND_INITIALIZER  PTHREAD_COND_INITIALIZER
#define  AFNIX_HAVE_CANCEL
#endif

// solaris platform - the initializers are a mess
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_SOLARIS)
#include <pthread.h>
#include <signal.h>
#ifdef  __GNUC__
#define  AFNIX_PTHREAD_ONCE_INIT         {PTHREAD_ONCE_INIT}
#if (AFNIX_PLATFORM_MAJOR == 5) && (AFNIX_PLATFORM_MINOR < 7)
#define  AFNIX_PTHREAD_MUTEX_INITIALIZER {{PTHREAD_MUTEX_INITIALIZER}}
#define  AFNIX_PTHREAD_COND_INITIALIZER  {{PTHREAD_COND_INITIALIZER}}
#else
#define  AFNIX_PTHREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#define  AFNIX_PTHREAD_COND_INITIALIZER  PTHREAD_COND_INITIALIZER
#endif
#else
#define  AFNIX_PTHREAD_ONCE_INIT         PTHREAD_ONCE_INIT
#define  AFNIX_PTHREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#define  AFNIX_PTHREAD_COND_INITIALIZER  PTHREAD_COND_INITIALIZER
#endif
#define  AFNIX_HAVE_CANCEL
#endif

// freebsd platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_FREEBSD)
#include <pthread.h>
#include <signal.h>
#define  AFNIX_PTHREAD_ONCE_INIT         PTHREAD_ONCE_INIT
#define  AFNIX_PTHREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#define  AFNIX_PTHREAD_COND_INITIALIZER  PTHREAD_COND_INITIALIZER
#define  AFNIX_HAVE_CANCEL
#endif

// darwin platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_DARWIN)
#include <pthread.h>
#include <signal.h>
#define  AFNIX_PTHREAD_ONCE_INIT         PTHREAD_ONCE_INIT
#define  AFNIX_PTHREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#define  AFNIX_PTHREAD_COND_INITIALIZER  PTHREAD_COND_INITIALIZER
#define  AFNIX_HAVE_CANCEL
#endif

// gnu/freebsd platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_GNUKBSD)
#include <pthread.h>
#include <signal.h>
#define  AFNIX_PTHREAD_ONCE_INIT         PTHREAD_ONCE_INIT
#define  AFNIX_PTHREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#define  AFNIX_PTHREAD_COND_INITIALIZER  PTHREAD_COND_INITIALIZER
#define  AFNIX_HAVE_CANCEL
#endif

// gnu platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_GNU)
#include <pthread.h>
#include <signal.h>
#define  AFNIX_PTHREAD_ONCE_INIT         PTHREAD_ONCE_INIT
#define  AFNIX_PTHREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#define  AFNIX_PTHREAD_COND_INITIALIZER  PTHREAD_COND_INITIALIZER
#define  AFNIX_HAVE_CANCEL
#endif

// android platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_ANDROID)
#include <pthread.h>
#define  AFNIX_PTHREAD_ONCE_INIT         PTHREAD_ONCE_INIT
#define  AFNIX_PTHREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#define  AFNIX_PTHREAD_COND_INITIALIZER  PTHREAD_COND_INITIALIZER
#endif
