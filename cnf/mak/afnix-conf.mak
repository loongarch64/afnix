# ----------------------------------------------------------------------------
# - afnix-conf                                                               -
# - afnix generic configuration makefile                                     -
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
# - copyright (c) 1999-2021 amaury darsch                                    -
# ----------------------------------------------------------------------------

# get the info make file
include  $(TOPDIR)/cnf/mak/afnix-info.mak

# get the unix make file
include  $(TOPDIR)/cnf/mak/afnix-unix.mak

# get the platform file
-include $(TOPDIR)/bld/cnf/afnix-plat.mak

# get the compiler file
-include $(TOPDIR)/bld/cnf/afnix-comp.mak

# get the library file
-include $(TOPDIR)/cnf/mak/afnix-libs.mak

# get the definitions file
include  $(TOPDIR)/cnf/mak/afnix-defs.mak

# get the packages file
include  $(TOPDIR)/cnf/mak/afnix-pkgs.mak

# eventually get the extension file
ifneq ($(EXTSFILE),)
include $(EXTSFILE)
endif

# mark the file as loaded
ifeq ($(AFNIX-CONF-LOAD),)
AFNIX-CONF-LOAD	= 1
endif
