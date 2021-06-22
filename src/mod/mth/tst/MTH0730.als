# ---------------------------------------------------------------------------
# - MTH0730.als                                                             -
# - afnix:mth module test unit                                              -
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

# @info   float block matrix test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a simple square matrix
trans nm (afnix:mth:Nblock 2)

# check predicate
assert true (afnix:mth:ami-p nm)
assert true (afnix:mth:nmi-p nm)
assert true (afnix:mth:n-block-p nm)

# check representation
assert "Nblock" (nm:repr)

# check matrix size and elements
assert 2    (nm:get-row-size)
assert 2    (nm:get-col-size)

# check accessors
const nz (Numeral 0)
assert nz (nm:get 0 0)
nm:set 0 0 nz
assert nz (nm:get 0 0)

const no (Numeral 1)
assert nz (nm:get 0 0)
nm:set 1 1 no
assert no (nm:get 1 1)

# yet another block
trans nm (afnix:mth:Nblock 128 128 Numeral:FLOAT)
loop (trans i 0) (< i 128) (i:++) {
  loop (trans j 0) (< j 128) (j:++) {
    nm:set i j (if (== i j) 1.0 0.0)
  }
}

assert 128 (nm:get-row-size)
assert 128 (nm:get-col-size)
loop (trans i 0) (< i 128) (i:++) {
  loop (trans j 0) (< j 128) (j:++) {
    assert (nm:get i j) (if (== i j) 1.0 0.0)
  }
}
