# ---------------------------------------------------------------------------
# - SIO0009.als                                                             -
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

# @info   afnix:sio mapped input stream test unit
# @author amaury darsch

# get the module
interp:library "afnix-sio"

# create a null stream
const  nm   (afnix:sio:InputMapped)
assert true (afnix:sio:input-mapped-p nm)
assert "InputMapped" (nm:repr)

# check the stream status
assert true  (nm:valid-p)
assert false (nm:eos-p)

# check stream content and length
const  c    (nm:read)
assert true (c:nil-p)
assert 0    (nm:length)

# the source file name
const name "SIO0009.als"

# create an input and mapped files
const fs (afnix:sio:InputFile   name)
const ms (afnix:sio:InputMapped name)

# check the length
assert (fs:length) (ms:length)

# preset character counter
trans count 0
# loop into the file
while (and (fs:valid-p) (ms:valid-p)) {
  assert (fs:read) (ms:read)
  count:++
}
# verify count
assert count (fs:length)
assert count (ms:length)

# create a buffer by name
const buf (Buffer name)
# map the buffer
const mb  (afnix:sio:InputMapped buf)

# check the size
assert (mb:length) (buf:length)

# preset character counter
trans count 0
# loop into the file
while (and (mb:valid-p) (not (buf:empty-p))) {
  assert (mb:read) (buf:read)
  count:++
}
# verify count
assert count (mb:length)
assert 0     (buf:length)
