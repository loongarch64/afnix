# ---------------------------------------------------------------------------
# - MTH0116.als                                                             -
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

# @info   block/sparse matrix comparator
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# compare block/sparse matrix
const mth-blk-sps-cmp-seq (size) {
  # create the matrix
  trans mb (afnix:mth:get-random-r-block size 100.0 true)
  trans ms (afnix:mth:Rmatrix size)
  loop (trans row 0) (< row size) (row:++) {
    loop (trans col 0) (< col size) (col:++) {
      ms:set row col (mb:get row col)
      assert (mb:get row col) (ms:get row col)
    }
  }
  assert true (ms:?= mb)
  # iterate
  trans mv (afnix:mth:Rblock size)
  for (is) (ms) {
    mv:set (is:get-row) (is:get-col) (is:get-val)
  }
  assert true (mv:?= mb)
}

# compare block/sparse matrix by column iterator
const mth-blk-sps-cmp-col (size) {
  # create the matrix
  trans mb (afnix:mth:get-random-r-block size 100.0 true)
  trans ms (afnix:mth:Rmatrix size)
  loop (trans row 0) (< row size) (row:++) {
    loop (trans col 0) (< col size) (col:++) {
      ms:set row col (mb:get row col)
      assert (mb:get row col) (ms:get row col)
    }
  }
  assert true (ms:?= mb)
  # iterate by row
  trans mv (afnix:mth:Rblock size)
  loop (trans row 0) (< row size) (row:++) {
    trans it (ms:iterator)
    it:set-type afnix:mth:Rmit:COL
    it:move row 0
    while (it:valid-p) {
      trans rmd (it:get-object)
      assert (rmd:get-row) row
      mv:set (rmd:get-row) (rmd:get-col) (rmd:get-val)
      it:next
    }
  }
  assert true (mv:?= mb)
}

# compare block/sparse matrix by column iterator
const mth-blk-sps-cmp-row (size) {
  # create the matrix
  trans mb (afnix:mth:get-random-r-block size 100.0 true)
  trans ms (afnix:mth:Rmatrix size)
  loop (trans row 0) (< row size) (row:++) {
    loop (trans col 0) (< col size) (col:++) {
      ms:set row col (mb:get row col)
      assert (mb:get row col) (ms:get row col)
    }
  }
  assert true (ms:?= mb)
  # iterate by row
  trans mv (afnix:mth:Rblock size)
  loop (trans col 0) (< col size) (col:++) {
    trans it (ms:iterator)
    it:set-type afnix:mth:Rmit:ROW
    it:move 0 col
    while (it:valid-p) {
      trans rmd (it:get-object)
      assert (rmd:get-col) col
      mv:set (rmd:get-row) (rmd:get-col) (rmd:get-val)
      it:next
    }
  }
  assert true (mv:?= mb)
}

mth-blk-sps-cmp-seq 3
mth-blk-sps-cmp-seq 16
mth-blk-sps-cmp-seq 17
mth-blk-sps-cmp-seq 32
mth-blk-sps-cmp-seq 33
mth-blk-sps-cmp-seq 63
mth-blk-sps-cmp-seq 64
mth-blk-sps-cmp-seq 65

mth-blk-sps-cmp-col 3
mth-blk-sps-cmp-col 16
mth-blk-sps-cmp-col 17
mth-blk-sps-cmp-col 32
mth-blk-sps-cmp-col 33
mth-blk-sps-cmp-col 63
mth-blk-sps-cmp-col 64
mth-blk-sps-cmp-col 65

mth-blk-sps-cmp-row 3
mth-blk-sps-cmp-row 16
mth-blk-sps-cmp-row 17
mth-blk-sps-cmp-row 32
mth-blk-sps-cmp-row 33
mth-blk-sps-cmp-row 63
mth-blk-sps-cmp-row 64
mth-blk-sps-cmp-row 65
