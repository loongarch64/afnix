# ---------------------------------------------------------------------------
# - NET0007.als                                                             -
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

# @info   tcp option test unit
# @author amaury darsch

# get the modules
interp:library "afnix-net"
interp:library "afnix-sys"

# check socket enumerated item
assert "SO-REUSE-ADDRESS"    (afnix:net:Sockopt:SO-REUSE-ADDRESS:to-string)
assert "SO-REUSE-PORT"       (afnix:net:Sockopt:SO-REUSE-PORT:to-string)
assert "SO-BROADCAST"        (afnix:net:Sockopt:SO-BROADCAST:to-string)
assert "SO-DONT-ROUTE"       (afnix:net:Sockopt:SO-DONT-ROUTE:to-string)
assert "SO-KEEP-ALIVE"       (afnix:net:Sockopt:SO-KEEP-ALIVE:to-string)
assert "SO-LINGER"           (afnix:net:Sockopt:SO-LINGER:to-string)
assert "SO-RECV-SIZE"        (afnix:net:Sockopt:SO-RECV-SIZE:to-string)
assert "SO-SEND-SIZE"        (afnix:net:Sockopt:SO-SEND-SIZE:to-string)
assert "SO-HOP-LIMIT"        (afnix:net:Sockopt:SO-HOP-LIMIT:to-string)
assert "SO-MCAST-LOOPBACK"   (afnix:net:Sockopt:SO-MCAST-LOOPBACK:to-string)
assert "SO-MCAST-HOP-LIMIT"  (afnix:net:Sockopt:SO-MCAST-HOP-LIMIT:to-string)
assert "SO-MAX-SEGMENT-SIZE" (afnix:net:Sockopt:SO-MAX-SEGMENT-SIZE:to-string)
assert "SO-NO-DELAY"         (afnix:net:Sockopt:SO-NO-DELAY:to-string)


# create a tcp socket object
const tcp (afnix:net:TcpSocket)

# set some options
assert true (tcp:set-option afnix:net:Sockopt:SO-REUSE-ADDRESS true)
assert true (tcp:set-option afnix:net:Sockopt:SO-DONT-ROUTE    true)
assert true (tcp:set-option afnix:net:Sockopt:SO-KEEP-ALIVE    true)
assert true (tcp:set-option afnix:net:Sockopt:SO-LINGER        10)
assert true (tcp:set-option afnix:net:Sockopt:SO-RECV-SIZE     1024)
assert true (tcp:set-option afnix:net:Sockopt:SO-SEND-SIZE     2084)
assert true (tcp:set-option afnix:net:Sockopt:SO-HOP-LIMIT     10)
assert true (tcp:set-option afnix:net:Sockopt:SO-NO-DELAY      true)

# create a udp socket
const udp (afnix:net:UdpSocket)

# set some options
assert true (udp:set-option afnix:net:Sockopt:SO-REUSE-ADDRESS true)
assert true (udp:set-option afnix:net:Sockopt:SO-DONT-ROUTE    true)
assert true (udp:set-option afnix:net:Sockopt:SO-RECV-SIZE     1024)
assert true (udp:set-option afnix:net:Sockopt:SO-SEND-SIZE     2084)
assert true (tcp:set-option afnix:net:Sockopt:SO-HOP-LIMIT     10)

# check special options
if (udp:broadcast-p) {
  assert true (udp:set-option afnix:net:Sockopt:SO-BROADCAST   true)
}
