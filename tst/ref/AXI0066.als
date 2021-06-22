# ---------------------------------------------------------------------------
# - AXI0066.als                                                              -
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

# @info   buffer test module
# @author amaury darsch

# create a buffer
trans  buf  (Buffer "afnix")
assert true (buffer-p buf)

# check predicate
assert false (buf:empty-p)
assert false (buf:full-p)
assert true  (buf:resize-p)

# check content
assert 5            (buf:length)
assert "afnix"      (buf:to-string)
assert "61666E6978" (buf:format)

# reset and check again
buf:reset
assert 0 (buf:length)

# add and check
assert 5            (buf:add "afnix")
assert "afnix"      (buf:to-string)
assert "61666E6978" (buf:format)
