# ---------------------------------------------------------------------------
# - AXI0064.als                                                              -
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

# @info   instance closure binding test module
# @author amaury darsch

trans x 0
trans c (class)
trans c:preset nil {
  const this:getx:x (+ x 1)
}
trans c:getx nil {
  eval x
}

# check for preset binding
trans  i (c)
assert 1 c:getx:x
trans  x 2
assert 1 (i:getx)

# check for indirect binding
const  i:getx:y 2
assert 2 c:getx:y
