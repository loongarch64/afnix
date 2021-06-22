# ----------------------------------------------------------------------------
# - afnix-rule                                                               -
# - afnix rule makefile definitions                                          -
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

# get the unix rule definitions
include  $(TOPDIR)/cnf/mak/afnix-runx.mak

# get the android rule definitions
include  $(TOPDIR)/cnf/mak/afnix-raid.mak

# get the debian rule definitions
include  $(TOPDIR)/cnf/mak/afnix-rdeb.mak

# get the rpm rule definitions
include  $(TOPDIR)/cnf/mak/afnix-rrpm.mak
