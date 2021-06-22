# ---------------------------------------------------------------------------
# - MTH0202.als                                                             -
# - afnix:mth module function test unit                                     -
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

# @info   newton solver test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# set the system precision
interp:set-absolute-precision 1.0E-8
interp:set-relative-precision 1.0E-14

# create a degre 2 polynom
const p (afnix:mth:Rpolynom 2)
# set coefficients
p:set-coefficient 0 -2.0
p:set-coefficient 2 1.0

# check compute
assert -2.0 (p:compute  0.0)
assert -1.0 (p:compute  1.0)

# create a newton solver
const  ntw  (afnix:mth:Newton)

# check type and representation
assert true     (afnix:mth:newton-p ntw)
assert "Newton" (ntw:repr)

# solve the square root of 2
const  t 2.0
const  r (ntw:solve p 1.0)
assert r (t:sqrt)
