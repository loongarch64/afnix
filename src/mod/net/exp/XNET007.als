# ---------------------------------------------------------------------------
# - XNET007.als                                                             -
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

# print some socket information
# @author amaury darsch

# get the module
interp:library "afnix-net"

# get the daytime server port
const port (afnix:net:get-tcp-service "daytime")

# create a client socket and read the data
const print-socket-info (host) {
  # create a tcp client socket
  const s (afnix:net:TcpClient host port)
  # print socket address and port
  const saddr (s:get-socket-address)
  const sport (s:get-socket-port)
  println "socket ip address     : " (saddr:get-address)
  println "socket canonical name : " (saddr:get-canonical-name)
  println "socket port           : " sport
  # print peer address and port
  const paddr (s:get-peer-address)
  const pport (s:get-peer-port)
  println "peer ip address       : " (paddr:get-address)
  println "peer canonical name   : " (paddr:get-canonical-name)
  println "peer port             : " pport  
}

# check for one argument or use localhost
const host (if (== (interp:argv:length) 0) "localhost" (interp:argv:get 0))
print-socket-info host
