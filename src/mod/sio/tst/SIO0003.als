# ---------------------------------------------------------------------------
# - SIO0003.als                                                             -
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

# @info   afnix:sio select test unit
# @author amaury darsch

# get the module
interp:library "afnix-sio"

# create a marked selector
const sh     (afnix:sio:Selector true)
assert true  (afnix:sio:selector-p sh)

# check the marking
assert false (sh:marked-p)
sh:mark
assert false (sh:marked-p)
# wait and check
sh:wait
assert true  (sh:marked-p)
assert false (sh:marked-p)

# create a temporary file
const tname (afnix:sio:absolute-path "tmp" (afnix:sio:tmp-name))
trans tfile (afnix:sio:OutputFile tname)

# open the input file 
const ifile (afnix:sio:InputFile "SIO0003.als")
const ilen  (ifile:length)

# create a selector between the input and output
trans sobj  (afnix:sio:Selector ifile tfile)
assert true (afnix:sio:selector-p sobj)

# simple check first
assert 1    (sobj:input-length)
assert 1    (sobj:output-length)
assert true (afnix:sio:input-stream-p  (sobj:input-get  0))
assert true (afnix:sio:output-stream-p (sobj:output-get 0))

# check that both are ready
trans  vobj  (sobj:wait-all 0)
assert 2     (vobj:length)

# clean the temporary file
afnix:sio:rmfile tname
