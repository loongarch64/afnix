# ---------------------------------------------------------------------------
# - AXI0103.als                                                              -
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

# @info   vector access test module - we look for dead lock
# @author amaury darsch

# create  a vector
const  v (Vector)
assert 0 (v:length)

# add a number in the vector
const vadd nil (v:add (v:length))

# this function launch a thread until the vector is full

const vector-thr nil (while (< (v:length) 128) (launch (vadd)))
vector-thr
