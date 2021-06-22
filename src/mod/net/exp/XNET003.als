# ---------------------------------------------------------------------------
# - XNET003.als                                                             -
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

# a port scanner example
# @author amaury darsch

# get the module
interp:library "afnix-net"

# a simple port scanner on a host
const port-scanner (host) {
  loop (trans i 0) (< i 1024) (i:++) {
    try {
      # create a tcp client socket
      afnix:net:TcpClient host i
      println "server found at port : " i
    }
  }
}

# check for one argument or use localhost
const host (if (== (interp:argv:length) 0) "localhost" (interp:argv:get 0))
port-scanner host
