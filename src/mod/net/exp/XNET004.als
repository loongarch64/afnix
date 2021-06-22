# ---------------------------------------------------------------------------
# - XNET004.als                                                             -
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

# fetch an html page by host and page
# usage: axi XNET004.als [host] [page]

# get the modules
interp:library "afnix-net"
interp:library "afnix-sys"

# connect to the http server and issue a request
const send-http-request (host page) {
  println host
  # create a client sock on port 80
  const s     (afnix:net:TcpClient host 80)
  const saddr (s:get-socket-address)

  # format the request
  s:writeln "GET " page " HTTP/1.1"
  s:writeln "Host: " (saddr:get-canonical-name)
  s:writeln "Connection: close"
  s:writeln "User-Agent: afnix tcp client example"
  s:newline

  # write the result
  while (s:valid-p) (println (s:readln))
}

# get the argument
if (!= (interp:argv:length) 2) (afnix:sys:exit 1)
const host (interp:argv:get 0)
const page (interp:argv:get 1)

# send request
send-http-request host page

