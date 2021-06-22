# ---------------------------------------------------------------------------
# - XNET001.als                                                             -
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

# print the ip address information of the arguments
# usage: axi XNET001.als [hosts ...]

# get the modules
interp:library "afnix-net"
interp:library "afnix-sys"

# print the ip address
const ip-address-info (host) {
  try {
    const addr (afnix:net:Address host true)
    println "address name     : " (addr:get-name)
    println "  ip address     : " (addr:get-address)
    println "  canonical name : " (addr:get-canonical-name)
    # get aliases
    const size (addr:get-alias-size)
    loop (trans i 0) (< i size) (i:++) {
      println "  alias address  : " (addr:get-alias-address i)
      println "  alias name     : " (addr:get-alias-name i)
    }      
  } (errorln "error: " what:reason)
}

# get the hosts 
if (== (interp:argv:length) 0) {
  (ip-address-info (afnix:sys:get-host-fqdn))
} {
  for (s) (interp:argv) (ip-address-info s)
}
