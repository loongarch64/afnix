# ---------------------------------------------------------------------------
# - MTH0731.als                                                             -
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

# @info   numeral array test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a simple array
trans na (afnix:mth:Narray 2)

# check predicate
assert true (afnix:mth:aai-p na)
assert true (afnix:mth:nai-p na)
assert true (afnix:mth:n-array-p na)

# check representation
assert "Narray" (na:repr)

# check array size and elements
assert 2 (na:get-size)
assert 1 (na:get-stride)

# check accessors
const nz (Numeral 0)
assert nz (na:get 0 0)
na:set 0 0 nz
assert nz (na:get 0 0)

const no (Numeral 1)
assert nz (na:get 0 0)
na:set 1 0 no
assert no (na:get 1 0)

# yet another block
trans na (afnix:mth:Narray 128 4 Numeral:FLOAT)
loop (trans aidx 0) (< aidx 128) (aidx:++) {
  loop (trans sidx 0) (< sidx 4) (sidx:++) {
    na:set aidx sidx (* aidx sidx)
  }
}

assert 128 (na:get-size)
assert 4   (na:get-stride)
loop (trans aidx 0) (< aidx 128) (aidx:++) {
  loop (trans sidx 0) (< sidx 4) (sidx:++) {
    assert (na:get aidx sidx) (* aidx sidx)
  }
}
