# ---------------------------------------------------------------------------
# - XNET009.als                                                             -
# - afnix example : network module example                                  -
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

# simple echo server (line based mode) - multi-threaded version
# @author amaury darsch

# get the module
interp:library "afnix-net"

# this function echo a line from the client
const echo-server (s) {
  while (s:valid-p) (s:writeln (s:readln))
}

# create a tcp server on port 8000
const srv (afnix:net:TcpServer 8000)

# wait for a connection
while true {
  trans s (srv:accept)
  launch  (echo-server s)
}
