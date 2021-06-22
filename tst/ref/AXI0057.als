# ---------------------------------------------------------------------------
# - AXI0057.als                                                             -
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

# @info   heap class test module
# @author amaury darsch

# create a default heap
const  heap (Heap)
assert true (heap-p heap)

# check size and flag
assert 0    (heap:length)
assert true (heap:empty-p)

# add element in the heap
loop (trans i 9) (>= i 0) (i:--) {
  heap:add i (i:clone)
}
assert 10 (heap:length)

# check the heap content
loop (trans i 0) (< i 10) (i:++) {
  assert i (heap:pop)
}
# check size and flag
assert 0    (heap:length)
assert true (heap:empty-p)

# reset and check
heap:reset
assert 0     (heap:length)
assert true  (heap:empty-p)
assert false (heap:min-key-p)
assert false (heap:max-key-p)

# set a min key and check
heap:set-min-key 10
assert true  (heap:min-key-p)
assert 10    (heap:get-min-key)
heap:add 9 9 
assert 0     (heap:length)

# set a max key and check
heap:set-max-key 100
assert true  (heap:max-key-p)
assert 100   (heap:get-max-key)
heap:add 110 110 
assert 0     (heap:length)

# check in the middle
heap:add 50 50
assert 1 (heap:length)

# reset and check
heap:reset-min-key
heap:reset-max-key
assert false (heap:min-key-p)
assert false (heap:max-key-p)

# reset and check
heap:reset
assert 0 (heap:length)
