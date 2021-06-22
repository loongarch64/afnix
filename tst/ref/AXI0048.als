# ---------------------------------------------------------------------------
# - AXI0048.als                                                              -
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

# @info   librarian test module
# @author amaury darsch

# the librarian test file
const fname "test.xl"
# create an input librarian
const il    (Librarian)
assert true (librarian-p il)

# check for content
assert 0 (il:length)
il:add   "Makefile"
assert 1 (il:length)

# check for names
assert true       (il:exists-p "Makefile")
trans names       (il:get-names)
assert "Makefile" (names:get 0)

# write the librarian
il:write fname

# now open for reading
const  ol   (Librarian fname)
assert true (librarian-p ol)

# check again
assert 1            (ol:length)
trans  names        (il:get-names)
assert "Makefile"   (names:get 0)
