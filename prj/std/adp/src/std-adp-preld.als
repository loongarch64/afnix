# ----------------------------------------------------------------------------
# - std-adp-preld                                                            -
# - afnix:adp preload module                                                 -
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
# - libary section                                                           -
# ----------------------------------------------------------------------------

# load the libraries
interp:library "afnix-sio"
interp:library "afnix-sys"
interp:library "afnix-txt"
interp:library "afnix-nwg"
interp:library "afnix-xml"
interp:library "afnix-wax"
interp:library "afnix-xpe"

# ----------------------------------------------------------------------------
# - nameset section                                                          -
# ----------------------------------------------------------------------------

try (const afnix     (nameset))
try (const afnix:adp (nameset afnix))

try (const AFNIX     (nameset))
try (const AFNIX:ADP (nameset AFNIX))
