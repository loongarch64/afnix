# ---------------------------------------------------------------------------
# - SEC0104.als                                                             -
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

# @info   extra aes block cipher test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# null aes test
# 128 bits key buffer
trans kbuf "00000000000000000000000000000000"
# 16 bytes input vector 00000000000000000000000000000000
trans ivec (Vector
  0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
  0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
)

# create a key with a buffer
trans key (afnix:sec:Key kbuf)
assert 128 (key:get-bits)

trans aes (afnix:sec:Aes key)
aes:set-block-mode afnix:sec:ModeCipher:MODE-ECB
aes:set-padding-mode afnix:sec:ModeCipher:PAD-NONE

trans ib (Buffer ivec)
trans ob (Buffer)
assert 16 (aes:stream ob ib)
assert "66E94BD4EF8A2C3B884CFA59CA342B2E" (ob:format)

# 128 bits key buffer
trans kbuf "1112131415161718191A1B1C1D1E1F10"
# 16 bytes input vector 964C05FCFEC9675CD9110E8EFFF7AF1B
trans ivec (Vector
  0x96 0x4C 0x05 0xFC 0xFE 0xC9 0x67 0x5C
  0xD9 0x11 0x0E 0x8E 0xFF 0xF7 0xAF 0x1B
)

# create a key with a buffer
trans key (afnix:sec:Key kbuf)
assert 128 (key:get-bits)

trans aes (afnix:sec:Aes key)
aes:set-block-mode afnix:sec:ModeCipher:MODE-ECB
aes:set-padding-mode afnix:sec:ModeCipher:PAD-NONE

trans ib (Buffer ivec)
trans ob (Buffer)
assert 16 (aes:stream ob ib)
assert "FE3FEC11033344C074E9292422D348E8" (ob:format)

#  192 bits key buffer
trans kbuf "1112131415161718191A1B1C1D1E1F102122232425262728"
# create a key with a buffer
trans key (afnix:sec:Key kbuf)
assert 192 (key:get-bits)

trans aes (afnix:sec:Aes key)
aes:set-block-mode afnix:sec:ModeCipher:MODE-ECB
aes:set-padding-mode afnix:sec:ModeCipher:PAD-NONE

trans ib (Buffer ivec)
trans ob (Buffer)
assert 16 (aes:stream ob ib)
assert "6273719672F7A082E53EFE9C49B68F33" (ob:format)

#  256 bits key buffer
trans kbuf "1112131415161718191A1B1C1D1E1F102122232425262728292A2B2C2D2E2F20"
# create a key with a buffer
trans key (afnix:sec:Key kbuf)
assert 256 (key:get-bits)

trans aes (afnix:sec:Aes key)
aes:set-block-mode afnix:sec:ModeCipher:MODE-ECB
aes:set-padding-mode afnix:sec:ModeCipher:PAD-NONE

trans ib (Buffer ivec)
trans ob (Buffer)
assert 16 (aes:stream ob ib)
assert "3A229344A5201A4C3F247D15B9DB1C50" (ob:format)

trans kbuf "571BAB712BDE3F5D5C5264D23956A5A7E31D6415EB21B9804C87D2E576D312B8"
trans iv "4FBAFEE5753F58F5E7F9A7A3642707E8"

# crypted message 48 bytes
trans ivec (Vector 
  0x96 0x4C 0x05 0xFC 0xFE 0xC9 0x67 0x5C
  0xD9 0x11 0x0E 0x8E 0xFF 0xF7 0xAF 0x1B
  0xCB 0x8E 0x66 0x48 0x87 0xBB 0x98 0xDA
  0xEE 0xB8 0x5E 0x58 0xC4 0x77 0x3F 0x5B
  0x84 0x5A 0x20 0x6A 0x8C 0xBA 0x5A 0x33
  0x64 0x53 0x1D 0xB5 0xE5 0x58 0xED 0x5C
)

# 16 bytes input buffer 964C05FCFEC9675CD9110E8EFFF7AF1B
trans ivec (Vector
  0x96 0x4C 0x05 0xFC 0xFE 0xC9 0x67 0x5C
  0xD9 0x11 0x0E 0x8E 0xFF 0xF7 0xAF 0x1B
)

# create a key with a buffer
trans key (afnix:sec:Key kbuf)
assert 256 (key:get-bits)

trans aes (afnix:sec:Aes key true)
aes:set-block-mode afnix:sec:ModeCipher:MODE-CBC
aes:set-padding-mode afnix:sec:ModeCipher:PAD-NONE
aes:set-iv iv

trans ib (Buffer ivec)
trans ob (Buffer)
assert 16 (aes:stream ob ib)
assert "1400000CF03331199A9294C1127A68B4" (ob:format)
