# ---------------------------------------------------------------------------
# - AXI0052.als                                                              -
# - afnix engine test module                                                -
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

# @info   hash-table class test module
# @author amaury darsch

trans  hash (HashTable)
assert true (hashtable-p hash)

# check the length
assert 0 (hash:length)

# add key pair
hash:add "one"   1
hash:add "two"   2
hash:add "hello" "world"

# check the length
assert 3 (hash:length)

# check existence
assert true (hash:exists-p "one")
assert true (hash:exists-p "two")
assert true (hash:exists-p "hello")

# check the element by key
assert 1       (hash:get "one")
assert 2       (hash:get "two")
assert "world" (hash:get "hello")

# remove element
hash:remove  "hello"
assert false (hash:exists-p "hello")
assert 2     (hash:length)

# reset everything and check length
hash:reset
assert 0 (hash:length)

# create a case insensitive hash table
trans hash (HashTable true)
assert true (hash:get-case-flag)

hash:add "one" 1
assert 1    (hash:length)
assert true (hash:exists-p "ONE")
hash:add "One" 1
assert 1 (hash:length)
assert 1 (hash:length)
assert true (hash:exists-p "ONE")
hash:remove "ONE"
assert 0 (hash:length)
