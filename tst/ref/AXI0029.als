# ---------------------------------------------------------------------------
# - AXI0029.als                                                              -
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

# @info   a point class test module
# @author amaury darsch

const  point (class)
assert true  (class-p point)
   
trans point:preset (x y) {
  trans this:x x
  trans this:y y
}

trans point:set-x (x) (this:x:= x)
trans point:get-x nil x

# check direct point creation
const  dp         (point 0 0)
assert true       (instance-p dp)
assert "Instance" (dp:repr)

# check method call
assert 0 dp:x
assert 0 dp:y
assert 1 (dp:set-x 1)
assert 1 (dp:get-x)
assert 1 dp:x

# check indirect creation
const  ip         (Instance)
assert true       (instance-p ip)
assert "Instance" (ip:repr)

# bind a symbol
const  ip:x 1
assert 1    ip:x
unref  ip:x
assert true (combo-p ip:x)

# set meta and check class
ip:mute point 1 2
assert 1 ip:x
assert 2 ip:y
assert 3 (ip:set-x 3)
assert 3 (ip:get-x)
assert 3 ip:x
