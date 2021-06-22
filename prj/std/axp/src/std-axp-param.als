# ----------------------------------------------------------------------------
# - std-axp-param                                                            -
# - afnix:axp parameters definition module                                   -
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
const AFNIX:AXP:MAJOR        3
const AFNIX:AXP:MINOR        1
const AFNIX:AXP:PATCH        0

# default application title
const AFNIX:AXP:TITLE        "afnix xml processor"
# the copyright holder
const AFNIX:AXP:HOLDER       "© 1999-2021 Amaury Darsch [amaury@afnix.org]"

# ----------------------------------------------------------------------------
# - default section                                                          -
# ----------------------------------------------------------------------------

# default output file flag
const AFNIX:AXP:SYSTEM-OFLG  false
# default output file
const AFNIX:AXP:SYSTEM-ONAM  "axp.txt"

# ----------------------------------------------------------------------------
# - session section                                                          -
# ----------------------------------------------------------------------------

# the default output flag
trans afnix:axp:system-oflg  AFNIX:AXP:SYSTEM-OFLG
# the default output name
trans afnix:axp:system-onam  AFNIX:AXP:SYSTEM-ONAM
