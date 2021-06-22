# ---------------------------------------------------------------------------
# - SIO0011.als                                                             -
# - afnix:sio module test unit                                              -
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

# @info   afnix:sio intercom test unit
# @author amaury darsch

# get the module
interp:library "afnix-sio"

# create an input/output buffer
const io (afnix:sio:InputOutput)

# create an intercom
const icom  (afnix:sio:Intercom)
assert true (afnix:sio:intercom-p icom)

# check the name
assert "Intercom" (icom:repr)

# bind the i/o streams
icom:set-input-stream  io
icom:set-output-stream io

# the reference string
const rs "afnix"
icom:send rs
assert rs (icom:recv)

# the assertion function
const check-icom-type nil (trans ...:tval (afnix:sio:intercom-p icom))

# send by protecting
icom:send (protect check-icom-type)
# receive and eval
trans  form (eval (icom:recv))
assert true (closure-p form)
# evaluate and check
form
assert true tval

# reset tval
trans ...:tval false
assert false tval

# send by delay
icom:send (delay check-icom-type)
# receive and eval
trans  prom (icom:recv)
assert true (promise-p prom)
# evaluate and check
(force prom)
assert true tval
