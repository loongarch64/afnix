# ---------------------------------------------------------------------------
# - XNET011.als                                                             -
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
# usage: axi XNET011.als [host1 host2]

# get the modules
interp:library "afnix-net"
interp:library "afnix-sys"

# print the address information

const print-addr-info (addr) {
  try {
    println "host name        : " (addr:get-name)
    println "  ip address     : " (addr:get-address)
    println "  canonical name : " (addr:get-canonical-name)
  } (errorln "error: " what:reason)
}

# loop betwwen two hosts

const run-hosts-loop (host1 host2) {
  const addr1 (afnix:net:Address host1 true)
  const addr2 (afnix:net:Address host2 true)
  loop (trans addr addr1) (<= addr addr2) (addr:++) (print-addr-info addr)
}

# get the hosts 
if (!= (interp:argv:length) 2) {
  errorln "usage: axi XNET011 host1 host2"
  afnix:sys:exit 1
} {
  const host1 (interp:argv:get 0)
  const host2 (interp:argv:get 1)
  run-hosts-loop host1 host2
}
