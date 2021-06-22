# ---------------------------------------------------------------------------
# - TXT0008.als                                                             -
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

# create a default trie
const  trie (afnix:txt:Trie)
assert true (afnix:txt:trie-p trie)

# add a binding to the trie
trie:add "hello" "hello"
assert false (trie:exists-p "hell")
assert true  (trie:exists-p "hello")
assert 1     (trie:length)
assert 1     (trie:count)

# add sub binding
trie:add "hell" "hello hell"
assert true  (trie:exists-p "hell")
assert true  (trie:exists-p "hello")
assert 2     (trie:length)

# check elements
assert "hello hell" (trie:get "hell")
assert "hello"      (trie:get "hello")
assert "hello hell" (trie:lookup "hell")
assert "hello"      (trie:lookup "hello")

# add another element
trie:add "afnix" "afnix"
assert true  (trie:exists-p "afnix")
assert 3     (trie:length)

# add another element
trie:add "afnor" "la norme"
assert true  (trie:exists-p "afnor")
assert 4     (trie:length)

# check marking
trie:mark "hello"
assert 2  (trie:count "hello")
