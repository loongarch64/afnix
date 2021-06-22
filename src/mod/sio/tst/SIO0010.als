# ---------------------------------------------------------------------------
# - SIO0010.als                                                             -
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

# @info   afnix:sio output buffer test unit
# @author amaury darsch

# get the module
interp:library "afnix-sio"

# the reference string
const rs "hello world" 

# create an output buffer
const  os   (afnix:sio:OutputBuffer)
assert true (afnix:sio:output-buffer-p os)

# write the reference string
os:write rs

# verify the content
assert (rs:length) (os:length)
assert rs (os:to-string)

# flush the normal output stream for write
os:flush
assert 0 (os:length)

