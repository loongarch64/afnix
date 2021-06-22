# ---------------------------------------------------------------------------
# - TXT0011.als                                                             -
# - afnix:txt module test unit                                              -
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

# @info   worder test unit
# @author amaury darsch

# get the module
interp:library "afnix-txt"

# create a  worder
const  wta  (afnix:txt:Worder)
assert true (afnix:txt:worder-p wta)

# check for empty
assert true (wta:empty-p)
assert 0    (wta:length)

# add some text and check
wta:add "hello"
wta:add "word"

assert false (wta:empty-p)
assert 2     (wta:length)
assert true  (wta:exists-p "hello")
assert true  (wta:exists-p "word")
assert false (wta:exists-p "world")

# check for matching
assert true  (wta:match-p "hello")
assert false (wta:match-p "world")

# check counter
assert 1     (wta:count [hello])
assert 1     (wta:count [wor$N*])

# check for unique
assert 2     (wta:unique-length)
wta:add "hello"
assert 3     (wta:length)
assert 2     (wta:unique-length)
wta:add "world"
assert 4     (wta:length)
assert 3     (wta:unique-length)

# check for matching
assert true  (wta:match-p "hello word")
assert true  (wta:match-p "world hello word")
assert false (wta:match-p "world hello word afnix")

# check counter
assert 2     (wta:count [hello])
assert 2     (wta:count [wor$N*])
