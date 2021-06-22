# ----------------------------------------------------------------------------
# - afnix-unix                                                               -
# - afnix unix make file configuration                                       -
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

# ----------------------------------------------------------------------------
# - generic unix commands                                                    -
# ----------------------------------------------------------------------------

RM              = /bin/rm -f
CP              = /bin/cp
MKDIR           = /bin/mkdir -p
CPDIR           = /bin/cp -r
MV              = /bin/mv -f
LN              = /bin/ln -s
CAT             = /bin/cat
RMDIR           = /bin/rm -r -f
GREP            = /bin/grep
CMP		= /usr/bin/cmp -s
TEST		= test
