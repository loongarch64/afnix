# ---------------------------------------------------------------------------
# - AXI0101.als                                                              -
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

# @info   basic shared object test module
# @author amaury darsch

# create simple symbol
const  v     (Vector "hello" "world")

# check the vector elements
trans  v0    (v:get 0)
trans  v1    (v:get 1)

# turn on the thread system
launch (nil)

# check the vector elements
trans  v0   (v:get 0)
trans  v1   (v:get 1)

