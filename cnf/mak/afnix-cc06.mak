# ----------------------------------------------------------------------------
# - afnix-clg5                                                               -
# - afnix compiler configuration - clang configuration                       -
# ----------------------------------------------------------------------------
# - This program is  free software;  you can  redistribute it and/or  modify -
# - it provided that this copyright notice is kept intact.                   -
# -                                                                          -
# - This  program  is  distributed in the hope  that it  will be useful, but -
# - without  any   warranty;  without  even   the   implied    warranty   of -
# - merchantability  or fitness for a particular purpose. In not event shall -
# - the copyright holder be  liable for  any direct, indirect, incidental or -
# - special damages arising in any way out of the use of this software.      -
# ----------------------------------------------------------------------------
# - author (c) 1999-2021 amaury   darsch                                     -
# - author (c) 2011-2013 pino     toscano                      hurd platform -
# - author (c) 2016-2016 martin   michlmayr                            GCC 5 -
# ----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# - compiler and linker section                                              -
# ----------------------------------------------------------------------------

CC              = clang++
LD              = clang++
LK		= clang++
AR              = ar
RANLIB		= ranlib
STDEVFLAGS      =
STDCCFLAGS      = -std=c++14 -Wall -Werror -Wno-overloaded-virtual -fno-builtin
STACCFLAGS	=
DYNCCFLAGS	= -fPIC
PLTCCFLAGS      = 
DEBUGFLAGS      = -g
OPTCCFLAGS      = -O2
PFLCCFLAGS      =
COVCCFLAGS      =
CPPCCFLAGS      = -nostdinc -nostdinc++
CXXCCFLAGS      = 
STDDEFINES      =
DBGDEFINES      = -DAFNIX_DEBUG
OPTDEFINES      =
PFLDEFINES	=
STDINCLUDE      =
AFXCPPTYPE	=
AFXCPPVERS	= 6

# ----------------------------------------------------------------------------
# - compiler dependant libraries                                             -
# ----------------------------------------------------------------------------

# adjust for linux platform
ifeq ($(PLATNAME),linux)
PLTCCFLAGS      = -MMD -pthread
ifeq ($(OPENMP),yes)
PLTCCFLAGS     += -fopenmp
endif
endif

# adjust for solaris platform
ifeq ($(PLATNAME),solaris)
PLTDEFINES	= -MMD -D_REENTRANT
endif

# adjust for freebsd platform
ifeq ($(PLATNAME),freebsd)
PLTDEFINES	= -MMD -D_REENTRANT
endif

# adjust for gnu/freebsd platform
ifeq ($(PLATNAME),gnukbsd)
PLTDEFINES	= -MMD -D_REENTRANT
ifeq ($(OPENMP),yes)
PLTCCFLAGS     += -fopenmp
endif
endif

# adjust for gnu platform
ifeq ($(PLATNAME),gnu)
PLTDEFINES	= -MMD -D_REENTRANT
ifeq ($(OPENMP),yes)
PLTCCFLAGS     += -fopenmp
endif
endif

# adjust for darwin platform
ifeq ($(PLATNAME),darwin)
PLTDEFINES	= -D_REENTRANT
PLTSDKROOT	= $(SDKDIR)
PLTSDKARCH	= -arch ppc -arch ppc64 -arch i686
PLTCCFLAGS      = -isysroot ${PLTSDKROOT} $(PLTSDKARCH)
PLTLDFLAGS	= $(PLTCCFLAGS)
PLTLKFLAGS	= $(PLTCCFLAGS)
PLTEVFLAGS	= MACOSX_DEPLOYMENT_TARGET=10.5
endif

# ----------------------------------------------------------------------------
# - platform dependant linking flags                                         -
# ----------------------------------------------------------------------------

# adjust for linux platform
ifeq ($(PLATNAME),linux)
ARFLAGS		= rc
LDFLAGS		= -shared -pthread
ifeq ($(LKMODE),soname)
LDFLAGS	       += -Wl,-soname,$(SOMAJ)
endif
ifeq ($(OPENMP),yes)
LDFLAGS        += -fopenmp
endif
AFXCPPLIBS	=
endif

# adjust for solaris platform
ifeq ($(PLATNAME),solaris)
ARFLAGS		= rc
LDFLAGS		= -shared
ifeq ($(LKMODE),soname)
LDFLAGS	       += -Wl,-h,$(SOMAJ)
endif
AFXCPPLIBS	=
endif

# adjust for freebsd platform
ifeq ($(PLATNAME),freebsd)
ARFLAGS		= rc
LDFLAGS		= -shared
ifeq ($(LKMODE),soname)
LDFLAGS	       += -Wl,-soname,$(SOMAJ)
endif
AFXCPPLIBS	=
endif

# adjust for gnu/freebsd platform
ifeq ($(PLATNAME),gnukbsd)
ARFLAGS		= rc
LDFLAGS		= -shared
ifeq ($(LKMODE),soname)
LDFLAGS	       += -Wl,-soname,$(SOMAJ)
endif
AFXCPPLIBS	=
endif

# adjust for gnu platform
ifeq ($(PLATNAME),gnu)
ARFLAGS		= rc
LDFLAGS		= -shared
ifeq ($(LKMODE),soname)
LDFLAGS	       += -Wl,-soname,$(SOMAJ)
endif
AFXCPPLIBS	=
endif

# adjust for darwin platform
ifeq ($(PLATNAME),darwin)
ARFLAGS		= -rc
LDFLAGS		= -dynamiclib $(PLTLDFLAGS)
ifeq ($(LKMODE),dylib)
LDFLAGS	       += -compatibility_version $(MAJOR).$(MINOR)
LDFLAGS        += -current_version $(MAJOR).$(MINOR).$(PATCH)
else
$(error, "undefined darwin linking mode")
endif
AFXCPPLIBS	=
endif

# ----------------------------------------------------------------------------
# - platform dependant final executable flags                                -
# ----------------------------------------------------------------------------

LKFLAGS		=

# adjust linker flags for linux
ifeq ($(PLATNAME),linux)
LKFLAGS      	= -pthread
endif

# adjust linker flags for freebsd
ifeq ($(PLATNAME),freebsd)
LKFLAGS      	= -pthread
endif

# adjust linker flags for darwin
ifeq ($(PLATNAME),darwin)
LKFLAGS      	= $(PLTLKFLAGS)
endif

# ----------------------------------------------------------------------------
# - target dependant final executable flags                                  -
# ----------------------------------------------------------------------------

# adjust extra flag for profiled code
ifeq ($(CCMODE),profiled)
LKFLAGS	       += -pg
endif

# adjust platform flag for static or dynamic
ifeq ($(LKTYPE),dynamic)
PLTCCFLAGS     += $(DYNCCFLAGS)
endif
ifeq ($(LKTYPE),static)
PLTCCFLAGS     += $(STACCFLAGS)
endif
