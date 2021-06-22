# ---------------------------------------------------------------------------
# - NWG0012.als                                                             -
# - afnix:nwg module test unit                                              -
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

# @info   basexx test unit
# @author amaury darsch

# get the module
interp:library "afnix-nwg"

# default encode codec
trans encd (afnix:nwg:Basexx)
encd:set-split-mode true
# default decode codec
trans decd (afnix:nwg:Basexx)
decd:set-reverse true

# create an input vector and serialize
trans iv (Vector 0 1 2 3 4 "afnix" false 2000)
trans eb (encd:serialize iv)
# deserialize and verify
trans ov (decd:unserialize eb)
for (x y) (iv ov) (assert x y)

# create an input vector and serialize
trans vt (Vector 0 1 2 3 4)
trans vb (Vector "afnix" false 2000)
trans et (encd:serialize vt true)
trans eb (encd:serialize vb true)
# create a target buffer
trans tb (Buffer "\n")
tb:add et
tb:add "\n\n"
tb:add eb
# deserialize and verify
trans ot (decd:unserialize tb true)
trans ob (decd:unserialize tb true)
for (x y) (vt ot) (assert x y)
for (x y) (vb ob) (assert x y)
