# ---------------------------------------------------------------------------
# - MTH0105.als                                                             -
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

# @info   vector/point/matrix test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a vector 1 object
trans v1 (afnix:mth:Rvector1)
assert true (afnix:mth:r-vector-1-p v1)
assert "Rvector1" (v1:repr)
# create a new vector and check
trans v1 (afnix:mth:Rvector1 1.0)
assert 1.0 (v1:get-x)

# create a vector 2 object
trans v2 (afnix:mth:Rvector2)
assert true (afnix:mth:r-vector-2-p v2)
assert "Rvector2" (v2:repr)
# create a new vector and check
trans v2 (afnix:mth:Rvector2 1.0 2.0)
assert 1.0 (v2:get-x)
assert 2.0 (v2:get-y)

# create a vector 3 object
trans v3 (afnix:mth:Rvector3)
assert true (afnix:mth:r-vector-3-p v3)
assert "Rvector3" (v3:repr)
# create a new vector and check
trans v3 (afnix:mth:Rvector3 1.0 2.0 3.0)
assert 1.0 (v3:get-x)
assert 2.0 (v3:get-y)
assert 3.0 (v3:get-z)

# create a vector 4 object
trans v4 (afnix:mth:Rvector4)
assert true (afnix:mth:r-vector-4-p v4)
assert "Rvector4" (v4:repr)
# create a new vector and check
trans v4 (afnix:mth:Rvector4 1.0 2.0 3.0 0.0)
assert 1.0 (v4:get-x)
assert 2.0 (v4:get-y)
assert 3.0 (v4:get-z)
assert 0.0 (v4:get-w)

# create a point 1 object
trans p1 (afnix:mth:Rpoint1)
assert true (afnix:mth:r-point-1-p p1)
assert "Rpoint1" (p1:repr)
# create a new point and check
trans p1 (afnix:mth:Rpoint1 1.0)
assert 1.0 (p1:get-x)

# create a point 2 object
trans p2 (afnix:mth:Rpoint2)
assert true (afnix:mth:r-point-2-p p2)
assert "Rpoint2" (p2:repr)
# create a new point and check
trans p2 (afnix:mth:Rpoint2 1.0 2.0)
assert 1.0 (p2:get-x)
assert 2.0 (p2:get-y)

# create a point 3 object
trans p3 (afnix:mth:Rpoint3)
assert true (afnix:mth:r-point-3-p p3)
assert "Rpoint3" (p3:repr)
# create a new point and check
trans p3 (afnix:mth:Rpoint3 1.0 2.0 3.0)
assert 1.0 (p3:get-x)
assert 2.0 (p3:get-y)
assert 3.0 (p3:get-z)

# create a point 4 object
trans p4 (afnix:mth:Rpoint4)
assert true (afnix:mth:r-point-4-p p4)
assert "Rpoint4" (p4:repr)
# create a new point and check
trans p4 (afnix:mth:Rpoint4 1.0 2.0 3.0 0.0)
assert 1.0 (p4:get-x)
assert 2.0 (p4:get-y)
assert 3.0 (p4:get-z)
assert 0.0 (p4:get-w)

# create a matrix 1 object
trans m1 (afnix:mth:Rmatrix1)
assert true (afnix:mth:r-matrix-1-p m1)
assert "Rmatrix1" (m1:repr)
# check for identity
assert 1.0 (m1:get 0 0)
# check for vector multiplication
trans v  (afnix:mth:Rvector1 1.0)
trans vr (m1:mul v)
assert 1.0 (vr:get 0)
# check for point multiplication
trans p  (afnix:mth:Rpoint1 -1.0)
trans pr (m1:mul p)
assert -1.0 (pr:get 0)
# check for matrix multiplication
trans m (afnix:mth:Rmatrix1)
trans mr (m1:mul m)
assert  1.0 (mr:get 0 0)

# create a matrix 2 object
trans m2 (afnix:mth:Rmatrix2)
assert true (afnix:mth:r-matrix-2-p m2)
assert "Rmatrix2" (m2:repr)
# check for identity
assert 1.0 (m2:get 0 0)
assert 1.0 (m2:get 1 1)
assert 0.0 (m2:get 0 1)
assert 0.0 (m2:get 1 0)
# check accessor
m2:set  0 1  1.0
m2:set  1 0 -1.0
assert  1.0 (m2:get 0 0)
assert  1.0 (m2:get 1 1)
assert  1.0 (m2:get 0 1)
assert -1.0 (m2:get 1 0)
# check for vector multiplication
trans v  (afnix:mth:Rvector2 1.0 1.0)
trans vr (m2:mul v)
assert 2.0 (vr:get 0)
assert 0.0 (vr:get 1)
# check for point multiplication
trans p  (afnix:mth:Rpoint2 -1.0 -1.0)
trans pr (m2:mul p)
assert -2.0 (pr:get 0)
assert  0.0 (pr:get 1)
# check for matrix multiplication
trans m (afnix:mth:Rmatrix2)
m:set 0 0  1.0
m:set 1 0  1.0
m:set 0 1 -1.0
m:set 1 1 -1.0
trans mr (m2:mul m)
assert  2.0 (mr:get 0 0)
assert  0.0 (mr:get 1 0)
assert -2.0 (mr:get 0 1)
assert  0.0 (mr:get 1 1)

# create a matrix 3 object
trans m3 (afnix:mth:Rmatrix3)
assert true (afnix:mth:r-matrix-3-p m3)
assert "Rmatrix3" (m3:repr)
# check for identity
assert 1.0 (m3:get 0 0)
assert 1.0 (m3:get 1 1)
assert 1.0 (m3:get 2 2)
assert 0.0 (m3:get 0 1)
assert 0.0 (m3:get 0 2)
assert 0.0 (m3:get 1 0)
assert 0.0 (m3:get 1 2)
assert 0.0 (m3:get 2 0)
assert 0.0 (m3:get 2 1)
# check accessor
m3:set  0 2  1.0
m3:set  2 0 -1.0
assert  1.0 (m3:get 0 2)
assert -1.0 (m3:get 2 0)
# check for vector multiplication
trans v  (afnix:mth:Rvector3 1.0 1.0 1.0)
trans vr (m3:mul v)
assert 2.0 (vr:get 0)
assert 1.0 (vr:get 1)
assert 0.0 (vr:get 2)
# check for point multiplication
trans p  (afnix:mth:Rpoint3 -1.0 -1.0 -1.0)
trans pr (m3:mul p)
assert -2.0 (pr:get 0)
assert -1.0 (pr:get 1)
assert  0.0 (pr:get 2)
# check for matrix multiplication
trans m (afnix:mth:Rmatrix3)
m:set 0 0  1.0
m:set 1 0  1.0
m:set 2 0  1.0
m:set 0 1 -1.0
m:set 1 1 -1.0
m:set 2 1 -1.0
trans mr (m3:mul m)
assert  2.0 (mr:get 0 0)
assert  1.0 (mr:get 1 0)
assert  0.0 (mr:get 2 0)
assert -2.0 (mr:get 0 1)
assert -1.0 (mr:get 1 1)
assert  0.0 (mr:get 2 1)
assert  1.0 (mr:get 0 2)
assert  0.0 (mr:get 1 2)
assert  1.0 (mr:get 2 2)

# create a matrix 4 object
trans m4 (afnix:mth:Rmatrix4)
assert true (afnix:mth:r-matrix-4-p m4)
assert "Rmatrix4" (m4:repr)
# check for identity
assert 1.0 (m4:get 0 0)
assert 1.0 (m4:get 1 1)
assert 1.0 (m4:get 2 2)
assert 1.0 (m4:get 3 3)
assert 0.0 (m4:get 0 1)
assert 0.0 (m4:get 0 2)
assert 0.0 (m4:get 0 3)
assert 0.0 (m4:get 1 0)
assert 0.0 (m4:get 1 2)
assert 0.0 (m4:get 1 3)
assert 0.0 (m4:get 2 0)
assert 0.0 (m4:get 2 1)
assert 0.0 (m4:get 2 3)
assert 0.0 (m4:get 3 0)
assert 0.0 (m4:get 3 1)
assert 0.0 (m4:get 3 2)
# check accessor
m4:set  0 3  1.0
m4:set  3 0 -1.0
assert  1.0 (m4:get 0 3)
assert -1.0 (m4:get 3 0)
# check for vector multiplication
trans v  (afnix:mth:Rvector4 1.0 1.0 1.0 1.0)
trans vr (m4:mul v)
assert 2.0 (vr:get 0)
assert 1.0 (vr:get 1)
assert 1.0 (vr:get 2)
assert 0.0 (vr:get 3)
# check for point multiplication
trans p  (afnix:mth:Rpoint4 -1.0 -1.0 -1.0 -1.0)
trans pr (m4:mul p)
assert -2.0 (pr:get 0)
assert -1.0 (pr:get 1)
assert -1.0 (pr:get 2)
assert  0.0 (pr:get 3)
# check for matrix multiplication
trans m (afnix:mth:Rmatrix4)
m:set 0 0  1.0
m:set 1 0  1.0
m:set 2 0  1.0
m:set 3 0  1.0
m:set 0 1 -1.0
m:set 1 1 -1.0
m:set 2 1 -1.0
m:set 3 1 -1.0
trans mr (m4:mul m)
assert  2.0 (mr:get 0 0)
assert  1.0 (mr:get 1 0)
assert  1.0 (mr:get 2 0)
assert  0.0 (mr:get 3 0)
assert -2.0 (mr:get 0 1)
assert -1.0 (mr:get 1 1)
assert -1.0 (mr:get 2 1)
assert  0.0 (mr:get 3 1)
assert  0.0 (mr:get 0 2)
assert  0.0 (mr:get 1 2)
assert  1.0 (mr:get 2 2)
assert  0.0 (mr:get 3 2)
assert  1.0 (mr:get 0 3)
assert  0.0 (mr:get 1 3)
assert  0.0 (mr:get 2 3)
assert  1.0 (mr:get 3 3)

# create a transformation object
trans tfrm (afnix:mth:Rtransform)

# create a rotation 2 matrix
trans m (tfrm:rotate afnix:mth:HALF-PI)
# create a point
trans p (afnix:mth:Rpoint2 1.0 1.0)
# rotate the point
trans q (m:mul p)
# validate the rotation
assert -1.0 (q:get-x) false
assert  1.0 (q:get-y) false

# create a rotation x matrix
trans m (tfrm:rotate afnix:mth:HALF-PI 0.0 0.0)
# create a point
trans p (afnix:mth:Rpoint3 0.0 1.0 1.0)
# rotate the point
trans q (m:mul p)
# validate the rotation
assert  0.0 (q:get-x) false
assert -1.0 (q:get-y) false
assert  1.0 (q:get-z) false

# create a rotation y matrix
trans m (tfrm:rotate 0.0 afnix:mth:HALF-PI 0.0)
# create a point
trans p (afnix:mth:Rpoint3 1.0 0.0 1.0)
# rotate the point
trans q (m:mul p)
# validate the rotation
assert  1.0 (q:get-x) false
assert  0.0 (q:get-y) false
assert -1.0 (q:get-z) false

# create a rotation z matrix
trans m (tfrm:rotate 0.0 0.0 afnix:mth:HALF-PI)
# create a point
trans p (afnix:mth:Rpoint3 1.0 1.0 0.0)
# rotate the point
trans q (m:mul p)
# validate the rotation
assert -1.0 (q:get-x) false
assert  1.0 (q:get-y) false
assert  0.0 (q:get-z) false

