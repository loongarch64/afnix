# ---------------------------------------------------------------------------
# - AXI0051.als                                                              -
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

# @info   print table test module
# @author amaury darsch

# create a simple print table
trans tbl (PrintTable)
assert true (print-table-p tbl)
assert 1    (tbl:get-columns)
assert 0    (tbl:get-rows)

# create a 2 columns table
trans tbl   (PrintTable 2)
assert true (print-table-p tbl)
assert 2    (tbl:get-columns)
assert 0    (tbl:get-rows)

# add elements
tbl:add "Library"   "Nameset"
tbl:add "afnix-sio" "afnix:sio"
tbl:add "afnix-sys" "afnix:sys"
tbl:add "afnix-txt" "afnix:txt"
tbl:add "afnix-net" "afnix:net"
tbl:add "afnix-www" "afnix:www"
tbl:add "afnix-odb" "afnix:odb"

# check data
assert "Library"   (tbl:get 0 0)
assert "afnix-sio" (tbl:get 1 0)
assert "afnix:www" (tbl:get 5 1)

# check tag
assert false (tbl:tag-p 0 0)
assert false (tbl:tag-p 1 0)
assert false (tbl:tag-p 5 1)

# set the tag by value and check
tbl:set-tag 0 0 "core"
assert "core" (tbl:get-tag 0 0)
tbl:set-tag 5 1 "core"
assert "core" (tbl:get-tag 5 1)
