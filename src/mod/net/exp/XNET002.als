# ---------------------------------------------------------------------------
# - XNET002.als                                                             -
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

# print the day time with a client socket

# get the module
interp:library "afnix-net"

# get the daytime server port
const port (afnix:net:get-tcp-service "daytime")

# create a client socket and read the data
const print-day-time (host) {
  # create a tcp client socket
  const s (afnix:net:TcpClient host port)

  # read the data - the server close the connection
  while (s:valid-p) (println (s:readln))
}

# check for one argument or use localhost
const host (if (== (interp:argv:length) 0) "localhost" (interp:argv:get 0))
print-day-time host
