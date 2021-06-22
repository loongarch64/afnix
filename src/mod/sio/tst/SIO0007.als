# ---------------------------------------------------------------------------
# - SIO0007.als                                                             -
# - afnix:sio module test unit                                              -
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

# @info   afnix:sio named fifo test unit
# @author amaury darsch

# get the module
interp:library "afnix-sio"

# create a default named fifo
trans  fifo (afnix:sio:NamedFifo)
assert true (afnix:sio:named-fifo-p fifo)

# check the size and local search flag
assert 0     (fifo:length)
assert true  (fifo:empty-p)
assert false (fifo:get-unique)

# add some element
fifo:add "hello"
fifo:add "world"
fifo:add "hello"
# check size and existence
assert 3    (fifo:length)
assert true (fifo:exists-p "hello")
assert true (fifo:exists-p "world")

# pop and check
assert "hello" (fifo:pop)
assert 2       (fifo:length)
assert true    (fifo:exists-p "hello")
assert true    (fifo:exists-p "world")

# pop and check
assert "world" (fifo:pop)
assert 1       (fifo:length)
assert true    (fifo:exists-p "hello")
assert false   (fifo:exists-p "world")

# pop and check
assert "hello" (fifo:pop)
assert 0       (fifo:length)
assert false   (fifo:exists-p "hello")
assert false   (fifo:exists-p "world")

# create a uniq named fifo
trans  fifo (afnix:sio:NamedFifo)
fifo:set-unique true

# check the size and local search flag
assert 0     (fifo:length)
assert true  (fifo:empty-p)
assert true  (fifo:get-unique)

# add some element
fifo:add "hello"
fifo:add "world"
fifo:add "hello"
# check size and existence
assert 2    (fifo:length)
assert true (fifo:exists-p "hello")
assert true (fifo:exists-p "world")

# pop and check
assert "world" (fifo:pop)
assert 1       (fifo:length)
assert true    (fifo:exists-p "hello")
assert false   (fifo:exists-p "world")

# pop and check
assert "hello" (fifo:pop)
assert 0       (fifo:length)
assert false   (fifo:exists-p "hello")
assert false   (fifo:exists-p "world")
