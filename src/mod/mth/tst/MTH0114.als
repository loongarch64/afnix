# ---------------------------------------------------------------------------
# - MTH0114.als                                                             -
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

# @info   qr update test unit
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

# create 3x3 matrix
trans m (afnix:mth:Rmatrix 3)
mth-set-3x3 m
# set lhs and factorize row 0
qr:set-lhs m
# factorize row 0
qr:factorize 0 1
# update row 1-2
qr:update 1 2
# get the qr matrix and compare
trans qrm (qr:get-qr)
assert true (qrm:?= m)

# create 3x3 matrix
trans m (afnix:mth:Rmatrix 3)
mth-set-3x3 m
# set lhs and factorize row 0
qr:set-lhs m
# factorize row 2
qr:factorize 2 1
# update row 0-1
qr:update 0 2
# get the qr matrix and compare
trans qrm (qr:get-qr)
assert true (qrm:?= m)

# create 9x9 matrix
trans m (afnix:mth:Rmatrix 9)
mth-set-9x9 m
# set lhs and factorize row 0-3
qr:set-lhs m
# factorize row 0-3
qr:factorize 0 4
# update row 4-8
qr:update 4 5
# get the qr matrix and compare
trans qrm (qr:get-qr)
assert true (qrm:?= m)

# create 9x9 matrix
trans m (afnix:mth:Rmatrix 9)
mth-set-9x9 m
# set lhs and factorize row 0-3
qr:set-lhs m
# factorize row 6-6
qr:factorize 6 3
# update row 0-6
qr:update 0 6
# get the qr matrix and compare
trans qrm (qr:get-qr)
assert true (qrm:?= m)

# create a random matrix
trans m (afnix:mth:get-random-r-matrix 17 100.0 true)
# set lhs and factorize
qr:set-lhs m
# factorize row 0-9
qr:factorize 0 10
# update row 10-16
qr:update 10 7
# get the qr matrix and compare
trans qrm (qr:get-qr)
assert true (qrm:?= m)

# create a random matrix
trans m (afnix:mth:get-random-r-matrix 64 100.0 true)
# set lhs and factorize
qr:set-lhs m
# factorize row 0-31
qr:factorize 0 32
# update row 32-63
qr:update 32 32
# get the qr matrix and compare
trans qrm (qr:get-qr)
assert true (qrm:?= m)

# create a random matrix
trans m (afnix:mth:get-random-r-matrix 128 100.0 true)
# set lhs and factorize
qr:set-lhs m
# factorize row 96-127
qr:factorize 96 32
# update row 0-95
qr:update 0 96
# get the qr matrix and compare
trans qrm (qr:get-qr)
assert true (qrm:?= m)
