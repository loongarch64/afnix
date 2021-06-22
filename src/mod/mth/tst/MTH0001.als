# ---------------------------------------------------------------------------
# - MTH0001.als                                                             -
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

# @info   random number test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# get a integer random number
const irnd (afnix:mth:get-random-integer)
assert true (integer-p irnd)

# get a integer random number
const lrnd (afnix:mth:get-random-integer 256)
assert true (integer-p lrnd)
assert true (<= lrnd 256)

# get a real random number
const rrnd (afnix:mth:get-random-real)
assert true (real-p rrnd)

# get a relatif random number
const  ernd (afnix:mth:get-random-relatif 256 false)
assert true (relatif-p ernd)
assert true (ernd:even-p)
assert 256  (ernd:get-msb)

const  ornd (afnix:mth:get-random-relatif 256 true)
assert true (relatif-p ornd)
assert true (ornd:odd-p)
assert 256  (ornd:get-msb)

# get a random prime number
const  prnd (afnix:mth:get-random-prime 128)
assert true (relatif-p prnd)
assert true (afnix:mth:prime-probable-p prnd)
assert 128  (prnd:get-msb)

# get a random safe prime number
const  srnd (afnix:mth:get-random-safe 128)
assert true (relatif-p srnd)
assert true (afnix:mth:prime-probable-p srnd)
assert true (afnix:mth:prime-safe-p srnd)
assert 128  (srnd:get-msb)

# get a random bitset
const  brnd (afnix:mth:get-random-bitset 1234)
assert true (bitset-p brnd)
assert 1234  (brnd:length)
