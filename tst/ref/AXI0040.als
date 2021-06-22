# ---------------------------------------------------------------------------
# - AXI0040.als                                                              -
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

# @info   queue test module
# @author amaury darsch

# create a queue
const  q1      (Queue)
assert true    (queue-p q1)
assert true    (q1:empty-p)

# create a queue with objects
const  q2      (Queue "hello" "world")
assert false   (q2:empty-p)
assert 2       (q2:length) 

# dequeue some object
assert "hello" (q2:dequeue)
assert "world" (q2:dequeue)
assert true    (q2:empty-p)
