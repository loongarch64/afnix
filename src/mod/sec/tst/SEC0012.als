# ---------------------------------------------------------------------------
# - SEC0012.als                                                             -
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

# @info   dsa test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# global key type
const KDSA afnix:sec:Key:KDSA

# the key parameters
const p 0xE0A67598_CD1B763B_C98C8ABB_333E5DDA_0CD3AA0E_5E1FB5BA◀
         ▶8A7B4EAB_C10BA338_FAE06DD4_B90FDA70_D7CF0CB0_C638BE33◀
         ▶41BEC0AF_8A7330A3_307DED22_99A0EE60_6DF03517_7A239C34◀
         ▶A912C202_AA5F83B9_C4A7CF02_35B5316B_FC6EFB9A_24841125◀
         ▶8B30B839_AF172440_F3256305_6CB67A86_1158DDD9_0E6A894C◀
         ▶72A5BBEF_9E286C6BR

const q 0xE950511E_AB424B9A_19A2AEB4_E159B784_4C589C4FR

const g 0xD29D5121_B0423C27_69AB2184_3E5A3240_FF19CACC_792264E3◀
         ▶BB6BE4F7_8EDD1B15_C4DFF7F1_D905431F_0AB16790_E1F773B5◀
         ▶CE01C804_E509066A_9919F519_5F4ABC58_189FD9FF_987389CB◀
         ▶5BEDF21B_4DAB4F8B_76A055FF_E2770988_FE2EC2DE_11AD9221◀
         ▶9F0B3518_69AC24DA_3D7BA870_11A701CE_8EE7BFE4_9486ED45◀
         ▶27B7186C_A4610A75R

const x 0xD0EC4E50_BB290A42_E9E355C7_3D880934_5DE2E139R

const y 0x25282217_F5730501_DD8DBA3E_DFCF349A_AFFEC209_21128D70◀
         ▶FAC44110_332201BB_A3F10986_140CBB97_C7269380_60473C8E◀
         ▶C97B4731_DB004293_B5E73036_3609DF97_80F8D883_D8C4D41D◀
         ▶ED6A2F1E_1BBBDC97_9E1B9D6D_3C940301_F4E978D6_5B19041F◀
         ▶CF1E8B51_8F5C0576_C770FE5A_7A485D83_29EE2914_A2DE1B5D◀
         ▶A4A6128C_EAB70F79R

const k 0x349C5564_8DCF992F_3F33E802_6CFAC87C_1D2BA075R
const i 0xD557A1B4_E7346C4A_55427A28_D4719138_1C269BDER

const r 0x636155AC_9A4633B4_665D179F_9E4117DF_68601F34R
const s 0x6C540B02_D9D4852F_89DF8CFC_99963204_F4347704R
const z 0xA9993E36_4706816A_BA3E2571_7850C26C_9CD0D89DR

# key verification
assert 1024 (p:get-msb)
assert  160 (q:get-msb)

# secret verification
assert true (> k 0R)
assert true (< k q)
assert i    (k:mmi q)

# signature verification (r)
const  tr   (g:pow k p)
assert r    (tr:mod q)

# signature verification (s)
const  ts   (* i (+ z (* x r)))
assert s    (ts:mod q)

# create the dsa key
const  key (afnix:sec:Key KDSA (Vector p q g x y))

# create the dsa object
const  dsa  (afnix:sec:Dsa key k)
assert true (afnix:sec:dsa-p dsa)

# compute the signature
const  sgn  (dsa:compute "abc")
assert true (afnix:sec:signature-p sgn)

# check the components
assert s (sgn:get-relatif-component afnix:sec:Signature:DSA-S-COMPONENT)
assert r (sgn:get-relatif-component afnix:sec:Signature:DSA-R-COMPONENT)

# verify the signature
assert true (dsa:verify sgn "abc")

# get the public key
const  pkey (sgn:get-public-key)
# verify the signature
const  pdsa (afnix:sec:Dsa pkey)
assert true (pdsa:verify sgn "abc")


