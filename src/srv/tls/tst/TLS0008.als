# ---------------------------------------------------------------------------
# - TLS0008.als                                                             -
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

# @info   tls certificate list test unit
# @author amaury darsch

# get the module
interp:library "afnix-tls"

# create a default certificate list
const cl (afnix:tls:TlsCerts)

# check representation
assert true (afnix:tls:certificate-list-p cl)
assert "TlsCerts" (cl:repr)

# add a certificate and check
cl:add "TLS0509.der"
assert 1 (cl:length)
