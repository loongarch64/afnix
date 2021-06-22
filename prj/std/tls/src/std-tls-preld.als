# ----------------------------------------------------------------------------
# - std-tls-preld                                                            -
# - afnix:std:tls preload unit                                               -
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
# - library section                                                          -
# ----------------------------------------------------------------------------

interp:library "afnix-mth"
interp:library "afnix-net"
interp:library "afnix-nwg"
interp:library "afnix-sec"
interp:library "afnix-sio"
interp:library "afnix-sys"
interp:library "afnix-tls"

# ----------------------------------------------------------------------------
# - nameset section                                                          -
# ----------------------------------------------------------------------------

try (const AFNIX     (nameset))
try (const AFNIX:TLS (nameset AFNIX))
