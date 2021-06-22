# ---------------------------------------------------------------------------
# - AXI0067.als                                                              -
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

# @info   block buffer test module
# @author amaury darsch

# create a buffer
trans  bbuf (BlockBuffer)
assert true (block-buffer-p bbuf)

# check predicate
assert true  (bbuf:empty-p)
assert false (bbuf:full-p)
assert false (bbuf:resize-p)

# reset and check again
bbuf:reset
assert 0 (bbuf:length)

# add and check
assert 5            (bbuf:add "afnix")
assert "afnix"      (bbuf:to-string)
assert "61666E6978" (bbuf:format)
