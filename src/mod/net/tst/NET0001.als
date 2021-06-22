# ---------------------------------------------------------------------------
# - NET0001.als                                                             -
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

# @info   afnix:net address test unit
# @author amaury darsch

# get the modules
interp:library "afnix-sys"
interp:library "afnix-net"

# localhost address class
const  lh   (afnix:net:Address (afnix:net:get-loopback))
assert true (afnix:net:address-p lh)

# check for daytime service
assert 13 (afnix:net:get-tcp-service "daytime")
assert 13 (afnix:net:get-udp-service "daytime")

# check for numerical address
assert true  (afnix:net:isipn-p "127.0.0.1" "46")
assert true  (afnix:net:isipn-p "127.0.0.1" "4")
assert false (afnix:net:isipn-p "127.0.0.1" "6")
assert true  (afnix:net:isipn-p "::1" "46")
assert false (afnix:net:isipn-p "::1" "4")
assert true  (afnix:net:isipn-p "::1" "6")
assert false (afnix:net:isipn-p "localhost" "46")
assert false (afnix:net:isipn-p "localhost" "4")
assert false (afnix:net:isipn-p "localhost" "6")

# create a loopback address
const addr (lh:get-address)
if (afnix:net:isipn-p addr "4") {
  assert addr "127.0.0.1"
}
if (afnix:net:isipn-p addr "6") {
  assert addr "::1"
}
