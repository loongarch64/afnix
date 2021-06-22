# ---------------------------------------------------------------------------
# - NET0006.als                                                             -
# - afnix:net module test unit                                              -
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

# @info   tcp client (the low level way) test unit
# @author amaury darsch

# get the module
interp:library "afnix-net"
interp:library "afnix-sys"

# create an address and a tcp socket
const addr (afnix:net:Address "localhost")
const sid  (afnix:net:TcpSocket)

# connect the socket
try {
  sid:connect 13 addr
  if (not (sid:valid-p)) (afnix:sys:exit 0)
  sid:shutdown false
  sid:shutdown true
} (afnix:sys:exit 1)

