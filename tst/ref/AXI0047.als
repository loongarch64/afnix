# ---------------------------------------------------------------------------
# - AXI0047.als                                                              -
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

# @info   closure functionnal check
# @author amaury darsch

# create a default closure
const  clo  (Closure)
assert true (closure-p clo)

# bind some closed variable
const  clo:year 2000
const  clo:mesg "hello"
assert 2000     clo:year
assert "hello"  clo:mesg

# eval some form binding
clo:set-form   (protect (eval year))
assert 2000    (clo)
clo:set-form   (protect (eval mesg))
assert "hello" (clo)

# add some argument
clo:add-argument (protect x)
clo:set-form     (protect (eval x))
assert 0         (clo 0)
