# ----------------------------------------------------------------------------
# - afnix-accs                                                               -
# - afnix compiler configuration - android configuration                     -
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
# - android platform                                                         -
# ----------------------------------------------------------------------------

ifeq ($(PLATNAME),android)
# reset platform processor
DROID_NTV=$(PROCNATV)
ifeq ($(PROCNATV),x64)
DROID_NTV=x86_64
endif
ifeq ($(PROCNAME),x64)
DROID_PCU=x86_64-linux-android
DROID_PCN=x86_64-linux-android
endif
ifeq ($(PROCNAME),ia32)
DROID_PCU=i686-linux-android
DROID_PCN=i686-linux-android
endif
ifeq ($(PROCNAME),arm)
DROID_PCU=armv7a-linux-androideabi
DROID_PCN=arm-linux-androideabi
endif
ifeq ($(PROCNAME),aarch64)
DROID_PCU=aarch64-linux-android
DROID_PCN=aarch64-linux-android
endif
# set tool bin directory
ifeq ($(DROID_BIN),)
DROID_BIN	= $(SDKDIR)/toolchains/llvm/prebuilt/linux-$(DROID_NTV)/bin
endif
# set tool version target
ifeq ($(DROID_VRS),)
DROID_VRS	= 26
endif
endif

# ----------------------------------------------------------------------------
# - compiler and linker section                                              -
# ----------------------------------------------------------------------------

C               = $(DROID_BIN)/$(DROID_PCU)$(DROID_VRS)-clang
CC              = $(DROID_BIN)/$(DROID_PCU)$(DROID_VRS)-clang++
CN		= g++
LD		= $(DROID_BIN)/$(DROID_PCU)$(DROID_VRS)-clang++
LK		= $(DROID_BIN)/$(DROID_PCU)$(DROID_VRS)-clang++
AR              = $(DROID_BIN)/$(DROID_PCN)-ar
RANLIB		= $(DROID_BIN)/$(DROID_PCN)-ranlib
STDEVFLAGS      =
STDCCFLAGS      = -Wall -Werror -Wno-overloaded-virtual -fno-builtin
STACCFLAGS	=
DYNCCFLAGS	= -fPIC
PLTCCFLAGS      = -MMD -pthread
DEBUGFLAGS      = -g
OPTCCFLAGS      = -O2
PFLCCFLAGS      =
COVCCFLAGS      =
CPPCCFLAGS      =
CXXCCFLAGS      = 
STDDEFINES      =
DBGDEFINES      = -DAFNIX_DEBUG
OPTDEFINES      =
PFLDEFINES	=
STDINCLUDE      =
AFXCPPTYPE	=
AFXCPPVERS	= 8

# ----------------------------------------------------------------------------
# - compiler dependant libraries                                             -
# ----------------------------------------------------------------------------

# adjust for android platform
ifeq ($(PLATNAME),android)
PLTCCFLAGS      = -MMD -pthread
ifeq ($(OPENMP),yes)
PLTCCFLAGS     += -fopenmp
endif
endif

# ----------------------------------------------------------------------------
# - platform dependant linking flags                                         -
# ----------------------------------------------------------------------------

# adjust for android platform
ifeq ($(PLATNAME),android)
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

# ----------------------------------------------------------------------------
# - platform dependant final executable flags                                -
# ----------------------------------------------------------------------------

LKFLAGS		=

# adjust linker flags for android
ifeq ($(PLATNAME),android)
LKFLAGS      	= -pthread
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
