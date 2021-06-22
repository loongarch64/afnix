# ---------------------------------------------------------------------------
# - MTH0002.als                                                             -
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

# @info   primality test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# check the fermat theorem
assert true  (afnix:mth:fermat-p 2  7)
assert true  (afnix:mth:fermat-p 2R 7R)

assert true  (afnix:mth:fermat-p 2  561)
assert true  (afnix:mth:fermat-p 2R 561R)

# check the miller-rabin test
assert true  (afnix:mth:miller-rabin-p 2  7)
assert true  (afnix:mth:miller-rabin-p 2R 7R)

assert false (afnix:mth:miller-rabin-p 2  561)
assert false (afnix:mth:miller-rabin-p 2R 561R)
assert false (afnix:mth:miller-rabin-p 2  221)
assert false (afnix:mth:miller-rabin-p 2R 221R)

assert true  (afnix:mth:miller-rabin-p 174  221)
assert true  (afnix:mth:miller-rabin-p 174R 221R)

# probable prime test
assert false (afnix:mth:prime-probable-p 0)
assert true  (afnix:mth:prime-probable-p 1)
assert true  (afnix:mth:prime-probable-p 2)
assert true  (afnix:mth:prime-probable-p 17863)
assert false (afnix:mth:prime-probable-p 17864)
assert true  (afnix:mth:prime-probable-p 130003)

assert true  (afnix:mth:prime-probable-p 17863R)
assert false (afnix:mth:prime-probable-p 17864R)
assert true  (afnix:mth:prime-probable-p 130003R)
assert true  (afnix:mth:prime-probable-p 130003R)

assert true  (afnix:mth:prime-probable-p 17863   20)
assert false (afnix:mth:prime-probable-p 17864   30)
assert true  (afnix:mth:prime-probable-p 130003R 40)
assert true  (afnix:mth:prime-probable-p 130003R 50)
