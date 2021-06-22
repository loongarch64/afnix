# ---------------------------------------------------------------------------
# - SEC0005.als                                                             -
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

# @info   kdf test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# ISO/SEC 18033-2 KDF1 test vector
# SHA-1 KEYLEN=128
trans ostr (+
  "032E45326FA859A72EC235ACFF929B15D1372E30B207255F0611B8F785D76437"
  "4152E0AC009E509E7BA30CD2F1778E113B64E135CF4E2292C75EFE5288EDFDA4")

trans dkey-1 (+
  "5F8DE105B5E96B2E490DDECBD147DD1DEF7E3B8E0E6A26EB7B956CCB8B3BDC1C"
  "A975BC57C3989E8FBAD31A224655D800C46954840FF32052CDF0D640562BDFAD")
trans dkey-2 (+
  "FA263CFCCF3C52B29F2AF4A1869959BC77F854CF15BD7A25192985A842DBFF8E"
  "13EFEE5B7E7E55BBE4D389647C686A9A9AB3FB889B2D7767D3837EEA4E0A2F04")
trans dkey (+ dkey-1 dkey-2)

trans  md (afnix:sec:Sha1)
trans  kd (afnix:sec:Kdf1 md 128)
assert dkey (kd:derive ostr)

# ISO/SEC 18033-2 KDF1 test vector
# SHA-256 RLEN=20 KEYLEN=128

trans dkey-1 (+
  "09E2DECF2A6E1666C2F6071FF4298305E2643FD510A2403DB42A8743CB989DE8"
  "6E668D168CBE604611AC179F819A3D18412E9EB45668F2923C087C12FEE0C5A0")
trans dkey-2 (+
  "D2A8AA70185401FBBD99379EC76C663E875A60B4AACB1319FA11C3365A8B79A4"
  "4669F26FB555C80391847B05ECA1CB5CF8C2D531448D33FBACA19F6410EE1FCB")
trans dkey (+ dkey-1 dkey-2)

trans  md (afnix:sec:Sha256 20)
trans  kd (afnix:sec:Kdf1 md 128)
assert dkey (kd:derive ostr)

# ISO/SEC 18033-2 KDF2 test vector
# SHA-1 KEYLEN=128

trans dkey-1 (+
  "0E6A26EB7B956CCB8B3BDC1CA975BC57C3989E8FBAD31A224655D800C4695484"
  "0FF32052CDF0D640562BDFADFA263CFCCF3C52B29F2AF4A1869959BC77F854CF")
trans dkey-2 (+
  "15BD7A25192985A842DBFF8E13EFEE5B7E7E55BBE4D389647C686A9A9AB3FB88"
  "9B2D7767D3837EEA4E0A2F04B53CA8F50FB31225C1BE2D0126C8C7A4753B0807")
trans dkey (+ dkey-1 dkey-2)

trans  md (afnix:sec:Sha1)
trans  kd (afnix:sec:Kdf2 md 128)
assert dkey (kd:derive ostr)

# ISO/SEC 18033-2 KDF2 test vector
# SHA-256 RLEN=20 KEYLEN=128

trans dkey-1 (+
  "10A2403DB42A8743CB989DE86E668D168CBE604611AC179F819A3D18412E9EB4"
  "5668F2923C087C12FEE0C5A0D2A8AA70185401FBBD99379EC76C663E875A60B4")
trans dkey-2 (+
  "AACB1319FA11C3365A8B79A44669F26FB555C80391847B05ECA1CB5CF8C2D531"
  "448D33FBACA19F6410EE1FCB260892670E0814C348664F6A7248AAF998A3ACC6")
trans dkey (+ dkey-1 dkey-2)

trans  md (afnix:sec:Sha256 20)
trans  kd (afnix:sec:Kdf2 md 128)
assert dkey (kd:derive ostr)
