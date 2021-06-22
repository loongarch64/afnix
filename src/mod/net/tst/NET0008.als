# ---------------------------------------------------------------------------
# - NET0008.als                                                             -
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

# @info   tcp client address test unit
# @author amaury darsch

# get the modules
interp:library "afnix-net"
interp:library "afnix-sys"

# get the hostname
const hn (afnix:net:get-loopback)

# connect to the daytime server (port 13)
try {
  trans  dts  (afnix:net:TcpClient hn 13)
  assert true (dts:valid-p)
  dts:close
}

# create another socket by address
try {
  const addr (afnix:net:Address hn)
  trans  dts  (afnix:net:TcpClient addr 13)
  assert true (dts:valid-p)
  dts:close
}
