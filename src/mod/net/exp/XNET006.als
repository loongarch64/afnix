# ---------------------------------------------------------------------------
# - XNET006.als                                                             -
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

# print the time with a udp client socket (with timeout)
# @author amaury darsch

# get the modules
interp:library "afnix-net"
interp:library "afnix-sys"

# get the daytime server port
const port (afnix:net:get-udp-service "time")

# create a client socket and read the data
const print-time (host) {
  # create a udp client socket
  const s (afnix:net:UdpClient host port)

  # send an empty datagram
  s:write
  # retransmit datagram each second
  while (not (s:valid-p 1000)) (s:write)
  # read the 4 bytes data and adjust to epoch
  const buf (s:read 4)
  const val (- (buf:get-host-quad) 2208988800)
  # format the date
  const time (afnix:sys:Time val)
  println (time:format-date) ' ' (time:format-time)
}

# check for one argument or use localhost
const host (if (== (interp:argv:length) 0) "localhost" (interp:argv:get 0))
print-time host
