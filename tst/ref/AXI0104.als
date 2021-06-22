# ---------------------------------------------------------------------------
# - AXI0104.als                                                              -
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

# @info   list access test module - we look for dead lock
# @author amaury darsch

# create  a vector
const  l (List)
assert 0 (l:length)

# add a number in the list
const ladd nil (l:add (l:length))

# this function launch a thread until the list is full

const list-thr nil (while (< (l:length) 128) (launch (ladd)))
list-thr
