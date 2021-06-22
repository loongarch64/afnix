# ---------------------------------------------------------------------------
# - AXI0105.als                                                              -
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

# @info   cons access test module - we look for dead lock
# @author amaury darsch

# create  a vector
const  c (Cons 1)
assert 1 (c:length)

# add a number in the cons list
const cadd nil (c:add (c:length))

# this function launch a thread until the list is full

const cons-thr nil (while (< (c:length) 128) (launch (cadd)))
cons-thr
