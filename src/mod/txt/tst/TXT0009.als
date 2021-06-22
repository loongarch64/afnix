# ---------------------------------------------------------------------------
# - TXT0009.als                                                             -
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

# @info   trie test unit
# @author amaury darsch

# get the module
interp:library "afnix-txt"

# create a default lexicon
const  tlex (afnix:txt:Lexicon)
assert true (afnix:txt:lexicon-p tlex)

# check length
assert true (tlex:empty-p)
assert 0    (tlex:length)

# check empty string
tlex:add ""
assert true (tlex:empty-p)
assert 0    (tlex:length)

# single character
tlex:add "a"
assert true  (tlex:exists-p "a")
assert false (tlex:exists-p "b")
assert 1     (tlex:length)
assert false (tlex:empty-p)

# add a binding to the tlex
tlex:add "hello"
assert false (tlex:exists-p "hell")
assert true  (tlex:exists-p "hello")
assert 2     (tlex:length)

# add sub binding
tlex:add "hell"
assert true  (tlex:exists-p "hell")
assert true  (tlex:exists-p "hello")
assert 3     (tlex:length)

# add another element
tlex:add "afnix"
assert true  (tlex:exists-p "afnix")
assert 4     (tlex:length)

# add another element
tlex:add "afnor"
assert true  (tlex:exists-p "afnor")
assert 5     (tlex:length)
