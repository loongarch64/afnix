# ---------------------------------------------------------------------------
# - MTH0201.als                                                             -
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

# @info   real polynom test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# degree 0 polynom checker
const check-polynom-0 nil {
  # create a degree 0 polynom
  const p (afnix:mth:Rpolynom)

  # check predicate
  assert true (afnix:mth:rfi-p p)
  assert true (afnix:mth:r-polynom-p p)

  # check representation
  assert "Rpolynom" (p:repr)

  # check degree
  assert 0 (p:get-positive-degree)
  assert 0 (p:get-negative-degree)

  # check compute at 0.0 and 1.0
  assert 0.0 (p:compute 0.0)
  assert 0.0 (p:compute 1.0)

  # check derivate at 0.0 and 1.0
  assert 0.0 (p:derivate 0.0)
  assert 0.0 (p:derivate 1.0)
}

# degree 2 polynom checker
const check-polynom-2 nil {
  # create a degree 2 polynom
  const p (afnix:mth:Rpolynom 2)

  # check degree
  assert 2 (p:get-positive-degree)
  assert 0 (p:get-negative-degree)

  # set coefficients
  p:set-coefficient 0 1.0
  p:set-coefficient 1 1.0
  p:set-coefficient 2 2.0
  # check coefficients
  assert 1.0 (p:get-coefficient 0)
  assert 1.0 (p:get-coefficient 1)
  assert 2.0 (p:get-coefficient 2)
  # check compute
  assert 1.0 (p:compute  0.0)
  assert 4.0 (p:compute  1.0)
  assert 2.0 (p:compute -1.0)
  # check derivate
  assert  1.0 (p:derivate  0.0)
  assert  5.0 (p:derivate  1.0)
  assert -3.0 (p:derivate -1.0)
}

# check all degree
check-polynom-0
check-polynom-2
