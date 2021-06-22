# ---------------------------------------------------------------------------
# - SIO0004.als                                                             -
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

# @info   afnix:sio input/output string test unit
# @author amaury darsch

# get the module
interp:library "afnix-sio"

# the reference string
const rs "hello world" 

# create an input string
const  is   (afnix:sio:InputString rs)
assert true (afnix:sio:input-string-p is)

# create an output string
const  os   (afnix:sio:OutputString)
assert true (afnix:sio:output-string-p os)

# read from the input string and write to the output
while (is:valid-p) (os:write (is:read))
assert rs (os:to-string)

# create an input/output stream
const io (afnix:sio:InputOutput)
# write the reference string
io:write rs

# flush the normal output stream for write
os:flush
# read from the i/o stream and write to the output
while (io:valid-p) (os:write (io:read))
assert rs (os:to-string)
