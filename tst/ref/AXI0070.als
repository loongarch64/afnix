# ---------------------------------------------------------------------------
# - AXI0070.als                                                             -
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

# @info   fifo class test module
# @author amaury darsch

# create a two object fifo
const  fifo (Fifo 2)

# check base methods 
assert true    (fifo-p fifo)
assert "Fifo"  (fifo:repr)
assert true    (fifo:empty-p)
assert false   (fifo:full-p)
assert 2       (fifo:get-size)
assert 0       (fifo:length)

# check contents
fifo:push "hello"
fifo:push "world"

assert false   (fifo:empty-p)
assert true    (fifo:full-p)
assert 2       (fifo:length)

assert "world" (fifo:pop)
assert "hello" (fifo:pop)

# reset and check
fifo:reset
assert true    (fifo:empty-p)
assert false   (fifo:full-p)
assert 0       (fifo:length)

# push again
fifo:push "hello"
fifo:push "world"

assert 2       (fifo:length)
assert "hello" (fifo:get 1)
assert "world" (fifo:get 0)

# resize and check
fifo:resize 100
assert 100     (fifo:get-size)
assert false   (fifo:full-p)
assert 2       (fifo:length)

fifo:resize 1
assert 1       (fifo:get-size)
assert 1       (fifo:length)
assert true    (fifo:full-p)
assert "world" (fifo:pop)
