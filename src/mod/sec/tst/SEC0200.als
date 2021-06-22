# ---------------------------------------------------------------------------
# - SEC0200.als                                                             -
# - afnix:sec module test unit                                              -
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

# @info   galois field test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# create a rinjdael galois field
trans gf (afnix:sec:Galois 0x11BR)
assert true (afnix:sec:galois-p gf)
assert "Galois" (gf:repr)

# check modulo
assert 1R (gf:mod 0x3F7ER)
# check addition
assert 0x99R (gf:add 0x53R 0xCAR)
# check multiplication
assert 0x1R  (gf:mul 0x53R 0xCAR)
assert 0xCAR (gf:inv 0x53R)
assert 0x53R (gf:inv 0xCAR)

# check modular
trans  p  0x100000000000000000000000000000087R
trans  gf (afnix:sec:Galois p)
assert 0R (gf:mod p)
assert 1R (gf:add p 1R)
assert 0R (gf:add p p)

# check addition
trans  x 0x0388DACE60B6A392F328C2B971B2FE78R
trans  y 0x66E94BD4EF8A2C3B884CFA59CA342B2ER
assert x (gf:add p x)
assert y (gf:add p y)

# check multiplication
assert 0R (gf:mul p 0R)
assert 0R (gf:mul p x)
assert 0R (gf:mul p y)

# check inverse
assert 1R (gf:mul x (gf:inv x))
assert 1R (gf:mul y (gf:inv y))

trans  z (gf:mul x y)
assert x (gf:mul z (gf:inv y))
assert y (gf:mul z (gf:inv x))

# check divide
assert y (gf:div z x)
assert x (gf:div z y)

# check gcm multiplication - gmm is reverse of mul
trans p 0x100000000000000000000000000000087R
trans sz (- (p:get-msb) 1)
trans gf (afnix:sec:Galois p)

trans x 0x0388DACE60B6A392F328C2B971B2FE78R
trans y 0x66E94BD4EF8A2C3B884CFA59CA342B2ER
trans z (gf:gmm x y)
trans w (gf:mul (x:reverse sz) (y:reverse sz))
assert  0x5E2EC746917062882C85B0685353DEB7R z
assert  z (w:reverse sz)

trans x 0x5E2EC746917062882C85B0685353DE37R
trans y 0x66E94BD4EF8A2C3B884CFA59CA342B2ER
trans z (gf:gmm x y)
trans w (gf:mul (x:reverse sz) (y:reverse sz))
assert  0xF38CBB1AD69223DCC3457AE5B6B0F885R z
assert  z (w:reverse sz)

trans x 0xBA471E049DA20E40495E28E58CA8C555R
trans y 0xB83B533708BF535D0AA6E52980D53B78R
trans z (gf:gmm x y)
trans w (gf:mul (x:reverse sz) (y:reverse sz))
assert  0xB714C9048389AFD9F9BC5C1D4378E052R z
assert  z (w:reverse sz)

