# ---------------------------------------------------------------------------
# - SVG0009.als                                                             -
# - afnix:svg service test unit                                             -
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

# @info   svg transform test unit
# @author amaury darsch

# get the module
interp:library "afnix-svg"

# create a transform object
const tr (afnix:svg:Transform)

# check predicate and representation
assert true (afnix:svg:transform-p tr)
assert "Transform" (tr:repr)

# check translation
tr:add-translate 5 10
assert "translate(5, 10)" (tr:to-string)
tr:reset
assert true (tr:nil-p)

# check scaling
tr:add-scaling 2.0
assert "scale(2.000000, 2.000000)" (tr:to-string)
tr:reset
assert true (tr:nil-p)

# check rotate
tr:add-rotate 90.0
assert "rotate(90.000000)" (tr:to-string)
tr:reset
assert true (tr:nil-p)

tr:add-rotate 90.0 5 10
assert "rotate(90.000000, 5, 10)" (tr:to-string)
tr:reset
assert true (tr:nil-p)

# check skew x
tr:add-skew-x 90.0
assert "skewX(90.000000)" (tr:to-string)
tr:reset
assert true (tr:nil-p)

# check skew y
tr:add-skew-y 90.0
assert "skewY(90.000000)" (tr:to-string)
tr:reset
assert true (tr:nil-p)

# check complex transformation
tr:add-translate -10 -20
tr:add-scaling     2.0
tr:add-rotate     45.0
const  s "translate(-10, -20) scale(2.000000, 2.000000) rotate(45.000000)"
assert s (tr:to-string)
