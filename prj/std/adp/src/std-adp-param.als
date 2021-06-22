# ----------------------------------------------------------------------------
# - std-adp-param                                                            -
# - afnix:adp parameters definition module                                   -
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
const AFNIX:ADP:MAJOR        3
const AFNIX:ADP:MINOR        1
const AFNIX:ADP:PATCH        0

# default application title
const AFNIX:ADP:TITLE        "afnix documentation processor"
# the copyright holder
const AFNIX:ADP:HOLDER       "© 1999-2021 Amaury Darsch [amaury@afnix.org]"

# ----------------------------------------------------------------------------
# - default section                                                          -
# ----------------------------------------------------------------------------

# the adp writing mode
const AFNIX:ADP:WRITER-MODE  (enum XHT MAN TEX)

# default output file flag
const AFNIX:ADP:SYSTEM-OFLG  false
# default output file
const AFNIX:ADP:SYSTEM-ONAM  "adp.txt"

# ----------------------------------------------------------------------------
# - xhtml writer section                                                     -
# ----------------------------------------------------------------------------

# the default style file
const AFNIX:ADP:SYSTEM-XCSS  "std-us-style.css"
# the default image file
const AFNIX:ADP:SYSTEM-XIMG  "std-us-afnix.png"
# the default description
const AFNIX:ADP:SYSTEM-XDSC  "AFNIX Writing System"
# the no header flag
const AFNIX:ADP:SYSTEM-NOHD  false
# the no body div flag
const AFNIX:ADP:SYSTEM-NOBD  false

# ----------------------------------------------------------------------------
# - nroff writer section                                                     -
# ----------------------------------------------------------------------------

# the default writer source
const AFNIX:ADP:SYSTEM-XSRC  "AFNIX"

# ----------------------------------------------------------------------------
# - session section                                                          -
# ----------------------------------------------------------------------------

# the default xhtml style file
trans afnix:adp:system-xcss  AFNIX:ADP:SYSTEM-XCSS
# the default xhtml image file
trans afnix:adp:system-ximg  AFNIX:ADP:SYSTEM-XIMG
# the default description
trans afnix:adp:system-xdsc  AFNIX:ADP:SYSTEM-XDSC
# the default no header flag
trans afnix:adp:system-nohd  AFNIX:ADP:SYSTEM-NOHD
# the default no body div flag
trans afnix:adp:system-nobd  AFNIX:ADP:SYSTEM-NOBD
# the default nroff writer source
trans afnix:adp:system-xsrc  AFNIX:ADP:SYSTEM-XSRC
# the default output flag
trans afnix:adp:system-oflg  AFNIX:ADP:SYSTEM-OFLG
# the default output name
trans afnix:adp:system-onam  AFNIX:ADP:SYSTEM-ONAM
