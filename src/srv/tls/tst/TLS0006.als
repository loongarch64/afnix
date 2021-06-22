# ---------------------------------------------------------------------------
# - TLS0006.als                                                             -
# - afnix:tls module test unit                                              -
# ---------------------------------------------------------------------------
# - This program is free software;  you can redistribute it  and/or  modify -
# - it provided that this copyright notice is kept intact.                  -
# -                                                                         -
# - This program  is  distributed in  the hope  that it will be useful, but -
# - without  any  warranty;  without  even   the   implied    warranty   of -
# - merchantability or fitness for a particular purpose.  In no event shall -
# - the copyright holder be liable for any  direct, indirect, incidental or -
# - special damages arising in any way out of the use of this software.     -
# ---------------------------------------------------------------------------
# - copyright (c) 1999-2021 amaury darsch                                   -
# ---------------------------------------------------------------------------

# @info   tls server hello test unit
# @author amaury darsch

# get the module
interp:library "afnix-tls"

# create a default handshake
const hlo (afnix:tls:TlsShello)

# check representation
assert true (afnix:tls:server-hello-p hlo)
assert "TlsShello" (hlo:repr)
