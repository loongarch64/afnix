# ---------------------------------------------------------------------------
# - AXI0068.als                                                              -
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

# @info   reader test module
# @author amaury darsch

# create a reader by string
trans rd (Reader "trans i 0")
assert true     (reader-p rd)
assert "Reader" (rd:repr)

# evaluate and check
trans form (rd:parse)
assert 0 (eval form)
assert 0 i

# evaluate again and check
assert nil (rd:parse)
