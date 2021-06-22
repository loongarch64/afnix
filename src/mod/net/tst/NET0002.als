# ---------------------------------------------------------------------------
# - NET0002.als                                                             -
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

# @info   tcp server test unit
# @author amaury darsch

# get the modules
interp:library "afnix-net"
interp:library "afnix-sys"

# create a tcp server and loop to accept connection
const run-tcp-server nil {
  # create a tcp server on ephemeral port 
  try {
    const srv (afnix:net:TcpServer)
    port:= (srv:get-socket-port)
    # mark the server as created
    cv:mark
    # wait for a connection
    const s (srv:accept)
    # write a greeting string and close the connection
    s:writeln "hello world"
    # close this socket
    s:close
  } (afnix:sys:exit 1)
}

# create a condition variable
const cv (Condvar)
# ephemeral port assigned to server
trans port 0
# run the tcp server in a thread
launch (run-tcp-server)
cv:wait
# create a tcp client object
const clt (afnix:net:TcpClient "localhost" port)
# read line and check greeting
assert "hello world" (clt:readln)
# close this client
clt:close
