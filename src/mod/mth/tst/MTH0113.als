# ---------------------------------------------------------------------------
# - MTH0113.als                                                             -
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

# @info   qr solver update test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

const chk-qr-update-3x3 (p) {
  # create 3x3 matrix
  const m (afnix:mth:Rmatrix 3)
  m:set 0 0  1.0
  m:set 1 1  1.0
  m:set 2 2  1.0
  # create the update vector
  const u (afnix:mth:Rvector 3)
  u:set 0 1.0
  u:set 1 2.0
  u:set 2 3.0
  const v (afnix:mth:Rvector 3)
  v:set 0 4.0
  v:set 1 5.0
  v:set 2 6.0
  # create the product matrix and total matrix
  const w (afnix:mth:Rmatrix u v)
  const a (+ m w)
  # verify a
  assert  5.0 (a:get 0 0)
  assert  5.0 (a:get 0 1)
  assert  6.0 (a:get 0 2)
  assert  8.0 (a:get 1 0)
  assert 11.0 (a:get 1 1)
  assert 12.0 (a:get 1 2)
  assert 12.0 (a:get 2 0)
  assert 15.0 (a:get 2 1)
  assert 19.0 (a:get 2 2)
  # factorize the total matrix
  trans qa (afnix:mth:Qr a)
  qa:set-permutation p
  qa:factorize
  assert true (a:?= (qa:get-qr))
  
  # factorize the m matrix
  trans qm (afnix:mth:Qr m)
  qm:set-permutation p
  qm:factorize
  qm:update u v
  assert true (a:?= (qm:get-qr))
}

# check direct update
chk-qr-update-3x3 nil
# create a 3x3 permutation
trans p (afnix:mth:Permute 3)
p:set 0 2
p:set 1 0
p:set 2 1
# check permuted update
chk-qr-update-3x3 p
