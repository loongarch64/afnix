# ---------------------------------------------------------------------------
# - MTH0003.als                                                             -
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

# @info   arithmetic test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# define two simple primes
const p (afnix:mth:get-random-prime 128)
const q (afnix:mth:get-random-prime 128)

# get the modulus and totient (lcm in new pkcs)
const n (* p q)
const m (afnix:mth:lcm (- p 1) (- q 1))

# fix exponent as 65537
const e 65537R
# check that e is coprime to m
assert 1R (afnix:mth:gcd m e)

# compute mmi of e with m
const d   (afnix:mth:mmi e m)
trans t   (* e d)
assert 1R (t:mod m)

# here we have the rsa keys 
# public: (n, e) private: (d, e)

# original message
const om 0x123456789R
# encrypted message
const em (om:pow e n)
# decrypted message
const dm (em:pow d n)
# check message
assert dm om

# second method based on crt
const dp (afnix:mth:mmi e (- p 1))
const dq (afnix:mth:mmi e (- q 1))
const qi (afnix:mth:mmi q p)

# compute m1 and m2
const m1 (em:pow dp p)
const m2 (em:pow dq q)
const ht (* (- m1 m2) qi)
const h  (ht:mod p)
const cm (+ m2 (* q h))
# check message
assert cm om
