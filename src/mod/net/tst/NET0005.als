# ---------------------------------------------------------------------------
# - NET0005.als                                                             -
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

# @info   multicast test unit
# @author amaury darsch

# get the modules
interp:library "afnix-sys"
interp:library "afnix-net"

# try to create a multicast socket
try {
  # create a multicast class
  const ..:s (afnix:net:Multicast "224.0.0.10")
} (afnix:sys:exit 0)

try {
  # write and read from this socket
  s:write "hello world"
  trans   dg (s:accept)
  assert  11 (dg:get-buffer-length)
  assert "hello world" (dg:readln)
  # close and exit
  s:close
} (afnix:sys:exit 1)
