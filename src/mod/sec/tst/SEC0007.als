# ---------------------------------------------------------------------------
# - SEC0007.als                                                             -
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

# @info   hmac test unit - FIPS PUB 198
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# ---------------------------------------------------------------------------
# - SHA-1 64 byte key                                                       -
# ---------------------------------------------------------------------------

# text
trans text "Sample #1"

# key
trans key0 "000102030405060708090a0b0c0d0e0f"
trans key1 "101112131415161718191a1b1c1d1e1f"
trans key2 "202122232425262728292a2b2c2d2e2f"
trans key3 "303132333435363738393a3b3c3d3e3f"
trans kstr (+ key0 (+ key1 (+ key2 key3)))
trans hkey (afnix:sec:Key afnix:sec:Key:KMAC kstr)

# hmac
trans hmac (afnix:sec:Hmac hkey)
hmac:compute text

# verification
assert 20 (hmac:get-hash-length)
assert 20 (hmac:get-result-length)

assert (Byte 0x4f) (hmac:get-byte 0)
assert (Byte 0x4c) (hmac:get-byte 1)
assert (Byte 0xa3) (hmac:get-byte 2)
assert (Byte 0xd5) (hmac:get-byte 3)

assert (Byte 0xd6) (hmac:get-byte 4)
assert (Byte 0x8b) (hmac:get-byte 5)
assert (Byte 0xa7) (hmac:get-byte 6)
assert (Byte 0xcc) (hmac:get-byte 7)

assert (Byte 0x0a) (hmac:get-byte 8)
assert (Byte 0x12) (hmac:get-byte 9)
assert (Byte 0x08) (hmac:get-byte 10)
assert (Byte 0xc9) (hmac:get-byte 11)

assert (Byte 0xc6) (hmac:get-byte 12)
assert (Byte 0x1e) (hmac:get-byte 13)
assert (Byte 0x9c) (hmac:get-byte 14)
assert (Byte 0x5d) (hmac:get-byte 15)

assert (Byte 0xa0) (hmac:get-byte 16)
assert (Byte 0x40) (hmac:get-byte 17)
assert (Byte 0x3c) (hmac:get-byte 18)
assert (Byte 0x0a) (hmac:get-byte 19)

# ---------------------------------------------------------------------------
# - SHA-1 20 byte key                                                       -
# ---------------------------------------------------------------------------

# text
trans text "Sample #2"

# key
trans kstr "303132333435363738393a3b3c3d3e3f40414243"
trans hkey (afnix:sec:Key afnix:sec:Key:KMAC kstr)

# hmac
trans hmac (afnix:sec:Hmac hkey)
hmac:compute text

# verification
assert 20 (hmac:get-hash-length)
assert 20 (hmac:get-result-length)

assert (Byte 0x09) (hmac:get-byte 0)
assert (Byte 0x22) (hmac:get-byte 1)
assert (Byte 0xd3) (hmac:get-byte 2)
assert (Byte 0x40) (hmac:get-byte 3)

assert (Byte 0x5f) (hmac:get-byte 4)
assert (Byte 0xaa) (hmac:get-byte 5)
assert (Byte 0x3d) (hmac:get-byte 6)
assert (Byte 0x19) (hmac:get-byte 7)

assert (Byte 0x4f) (hmac:get-byte 8)
assert (Byte 0x82) (hmac:get-byte 9)
assert (Byte 0xa4) (hmac:get-byte 10)
assert (Byte 0x58) (hmac:get-byte 11)

assert (Byte 0x30) (hmac:get-byte 12)
assert (Byte 0x73) (hmac:get-byte 13)
assert (Byte 0x7d) (hmac:get-byte 14)
assert (Byte 0x5c) (hmac:get-byte 15)

assert (Byte 0xc6) (hmac:get-byte 16)
assert (Byte 0xc7) (hmac:get-byte 17)
assert (Byte 0x5d) (hmac:get-byte 18)
assert (Byte 0x24) (hmac:get-byte 19)


# ---------------------------------------------------------------------------
# - SHA-1 100 bytes key                                                     -
# ---------------------------------------------------------------------------

# text
trans text "Sample #3"

# key
trans key0 "505152535455565758595a5b5c5d5e5f"
trans key1 "606162636465666768696a6b6c6d6e6f"
trans key2 "707172737475767778797a7b7c7d7e7f"
trans key3 "808182838485868788898a8b8c8d8e8f"
trans key3 "808182838485868788898a8b8c8d8e8f"
trans key4 "909192939495969798999a9b9c9d9e9f"
trans key5 "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3"
trans kstr (+ key0 (+ key1 (+ key2 (+ key3 (+ key4 key5)))))
trans hkey (afnix:sec:Key afnix:sec:Key:KMAC kstr)

# hmac
trans hmac (afnix:sec:Hmac hkey)
hmac:compute text

# verification
assert 20 (hmac:get-hash-length)
assert 20 (hmac:get-result-length)

assert (Byte 0xbc) (hmac:get-byte 0)
assert (Byte 0xf4) (hmac:get-byte 1)
assert (Byte 0x1e) (hmac:get-byte 2)
assert (Byte 0xab) (hmac:get-byte 3)

assert (Byte 0x8b) (hmac:get-byte 4)
assert (Byte 0xb2) (hmac:get-byte 5)
assert (Byte 0xd8) (hmac:get-byte 6)
assert (Byte 0x02) (hmac:get-byte 7)

assert (Byte 0xf3) (hmac:get-byte 8)
assert (Byte 0xd0) (hmac:get-byte 9)
assert (Byte 0x5c) (hmac:get-byte 10)
assert (Byte 0xaf) (hmac:get-byte 11)

assert (Byte 0x7c) (hmac:get-byte 12)
assert (Byte 0xb0) (hmac:get-byte 13)
assert (Byte 0x92) (hmac:get-byte 14)
assert (Byte 0xec) (hmac:get-byte 15)

assert (Byte 0xf8) (hmac:get-byte 16)
assert (Byte 0xd1) (hmac:get-byte 17)
assert (Byte 0xa3) (hmac:get-byte 18)
assert (Byte 0xaa) (hmac:get-byte 19)
