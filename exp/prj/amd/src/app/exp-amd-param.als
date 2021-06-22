# ----------------------------------------------------------------------------
# - exp-amd-param                                                            -
# - afnix:amd parameters definition module                                   -
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
# - project section                                                          -
# ----------------------------------------------------------------------------

# system revision
const AFNIX:AMD:MAJOR        0
const AFNIX:AMD:MINOR        1
const AFNIX:AMD:PATCH        0

# default application title
const AFNIX:AMD:TITLE        "afnix media dumper (+http://www.afnix.org)"
# the copyright holder
const AFNIX:AMD:HOLDER       "© 1999-2021 Amaury Darsch [amaury@afnix.org]"

# default output file flag
const AFNIX:AMD:SYSTEM-OFLG  false
# default output file
const AFNIX:AMD:SYSTEM-ONAM  "index.htm"

# ----------------------------------------------------------------------------
# - session section                                                          -
# ----------------------------------------------------------------------------

# the default output flag
trans afnix:amd:system-oflg  AFNIX:AMD:SYSTEM-OFLG
# the default output name
trans afnix:amd:system-onam  AFNIX:AMD:SYSTEM-ONAM
