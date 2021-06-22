# ---------------------------------------------------------------------------
# - ITU0100.als                                                             -
# - afnix:itu module test unit                                              -
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

# @info   oid test unit
# @author amaury darsch

# get the module
interp:library "afnix-itu"

# create a oid object
const  oid  (afnix:itu:Oid)
assert true (afnix:itu:oid-p oid)

# check the name
assert "Oid" (oid:repr)

# check 0 content
assert 0 (oid:length)

# add X500 common name (CN)
oid:add 2 5 4 3

assert 4 (oid:length)
assert 2 (oid:get 0)
assert 5 (oid:get 1)
assert 4 (oid:get 2)
assert 3 (oid:get 3)

# reset and check
oid:reset
assert 0 (oid:length)
