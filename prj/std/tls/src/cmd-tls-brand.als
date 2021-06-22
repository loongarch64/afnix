# ----------------------------------------------------------------------------
# - cmd-tls-brand                                                            -
# - afnix:tls random byte command module                                     -
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
# - load section                                                             -
# ----------------------------------------------------------------------------

interp:load "std-tls-prjld"
interp:load "std-tls-brand"

# ----------------------------------------------------------------------------
# - main section                                                             -
# ----------------------------------------------------------------------------

# create an inets command
const cmd (afnix:tls:brand interp:argv)
# execute the command
cmd:run

# ----------------------------------------------------------------------------
# - unload section                                                             -
# ----------------------------------------------------------------------------

interp:load "std-tls-punld"
