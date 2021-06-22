# ---------------------------------------------------------------------------
# - MTH0121.als                                                             -
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

# @info   qr direct solver test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"
# load matrix generator
interp:load "MTHXXXX"

# create a qr solver
const qr (afnix:mth:Qr)  

# check representation
assert true (afnix:mth:qr-p qr)
assert true (afnix:mth:direct-p qr)
assert true (afnix:mth:solver-p qr)
assert "Qr" (qr:repr)

# create 3x3 block
trans m (afnix:mth:Rblock 3)
mth-set-3x3 m
# set lhs and factorize
qr:set-lhs m
qr:factorize
# get the qr block and compare
trans qrm (qr:get-qr)
assert true (qrm:?= m)

# create 3x3 block
trans m (afnix:mth:Rblock 3)
mth-set-3x3 m
# create a 3x3 permutation
trans p (afnix:mth:Permute 3)
p:set 0 2
p:set 1 0
p:set 2 1
# set lhs and factorize
qr:set-lhs m
qr:set-permutation p
qr:factorize
# get the qr block and compare
trans qrm (qr:get-qr)
assert true (qrm:?= m)

# create a 5x5 block
trans m (afnix:mth:Rblock 5)
mth-set-5x5 m
# set lhs and factorize
qr:set-lhs m
qr:factorize
# get the qr block and compare
trans qrm (qr:get-qr)
assert true (qrm:?= m)

# create a 9x9 block
trans m (afnix:mth:Rblock 9)
mth-set-9x9 m
# set lhs and factorize
qr:set-lhs m
qr:factorize
# get the qr block and compare
trans qrm (qr:get-qr)
assert true (qrm:?= m)

# create a random block
trans m (afnix:mth:get-random-r-block 17 100.0 true)
# set lhs and factorize
qr:set-lhs m
qr:factorize
# get the qr block and compare
trans qrm (qr:get-qr)
assert true (qrm:?= m)

# create a random block
trans m (afnix:mth:get-random-r-block 64 100.0 true)
# set lhs and factorize
qr:set-lhs m
qr:factorize
# get the qr block and compare
trans qrm (qr:get-qr)
assert true (qrm:?= m)

# create a random block
trans m (afnix:mth:get-random-r-block 128 100.0 true)
# set lhs and factorize
qr:set-lhs m
qr:factorize
# get the qr block and compare
trans qrm (qr:get-qr)
assert true (qrm:?= m)
