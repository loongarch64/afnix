# ---------------------------------------------------------------------------
# - NET0003.als                                                             -
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

# @info   udp server test unit
# @author amaury darsch

# get the modules
interp:library "afnix-net"
interp:library "afnix-sys"

# create a udp server and loop to accept connection
const create-udp-server nil {
  try {
    # create a udp server on ephemeral port
    const s (afnix:net:UdpServer)
    port:= (s:get-socket-port)
    # notify we are ready
    cv:mark
    # wait for a datagram
    const dg (s:accept)
    # reply with a greeting string 
    dg:write "hello world"
    # close this udp server
    s:close
  } (afnix:sys:exit 1)
}

# create a condition variable
const cv (Condvar)
# ephemeral port assigned to server
trans port 0
# create the udp server in a thread
launch (create-udp-server)
# wait for the server to settle
cv:wait

# create a udp client object
const clt (afnix:net:UdpClient "localhost" port)
# send an empty datagram
clt:write

# read line and check greeting
const buf (clt:read 256)
assert "hello world" (buf:to-string)
clt:close
