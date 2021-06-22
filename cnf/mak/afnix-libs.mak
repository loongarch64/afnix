# ----------------------------------------------------------------------------
# - afnix-libs                                                               -
# - afnix library makefile configuration                                     -
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
# ---------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# - library extensions                                                       -
# ----------------------------------------------------------------------------

# default library extensions
AREXT		= a
DYEXT		= so

# darwin extensions
ifeq ($(PLATNAME),darwin)
DYEXT		= dylib
endif

# ----------------------------------------------------------------------------
# - platform library settings                                                -
# ----------------------------------------------------------------------------

# standard libraries for linux
ifeq ($(PLATNAME),linux)
STDLIBS  = $(EXTSTDLIBS) -ltinfo -ldl -lm
STDLIBS += $(AFXCPPLIBS)
EXESLIB  =
endif

# standard libraries for solaris
ifeq ($(PLATNAME),solaris)
STDLIBS  = $(EXTSTDLIBS) -lsocket -lnsl -lpthread -lposix4 -ldl -lm
STDLIBS += $(AFXCPPLIBS)
EXESLIB  = -ltermcap
endif

# standard libraries for freebsd
ifeq ($(PLATNAME),freebsd)
STDLIBS  = $(EXTSTDLIBS) -lm -lncurses
STDLIBS += $(AFXCPPLIBS)
EXESLIB  = 
endif

# standard libraries for darwin
ifeq ($(PLATNAME),darwin)
STDLIBS  = $(EXTSTDLIBS) -lncurses
STDLIBS += $(AFXCPPLIBS)
EXESLIB  = 
endif

# standard libraries for gnu/freebsd
ifeq ($(PLATNAME),gnukbsd)
STDLIBS  = $(EXTSTDLIBS) -lpthread -ldl -lm -lcurses
STDLIBS += $(AFXCPPLIBS)
EXESLIB  =
endif

# standard libraries for gnu
ifeq ($(PLATNAME),gnu)
STDLIBS  = $(EXTSTDLIBS) -lpthread -ldl -lm -lcurses
STDLIBS += $(AFXCPPLIBS)
EXESLIB  =
endif
