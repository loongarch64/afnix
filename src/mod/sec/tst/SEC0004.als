# ---------------------------------------------------------------------------
# - SEC0004.als                                                             -
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

# @info   aes block cipher test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# FIPS-197 128 bits key buffer
const kbuf "2B7E151628AED2A6ABF7158809CF4F3C"

# FIPS-197 16 bytes input vector
const ivec (Vector 
  0x32 0x43 0xF6 0xA8 0x88 0x5A 0x30 0x8D
  0x31 0x31 0x98 0xA2 0xE0 0x37 0x07 0x34
)

# FIPS-197 16 bytes output vector
const ovec (Vector 
  0x39 0x25 0x84 0x1D 0x02 0xDC 0x09 0xFB
  0xDC 0x11 0x85 0x97 0x19 0x6A 0x0B 0x32
)

# simple 8 bytes test buffer
const tvec (Vector 
  0x32 0x43 0xF6 0xA8 0x88 0x5A 0x30 0x8D
)

# create a key with a buffer
const  key (afnix:sec:Key kbuf)
assert 128 (key:get-bits)

# this expression test the aes cipher with no padding
const test-aes-cipher-no-padding (cmod) {
  # create the aes cipher with a key
  const  aes   (afnix:sec:Aes key)
  assert "AES" (aes:get-name)

  # force no padding
  aes:set-padding-mode afnix:sec:ModeCipher:PAD-NONE
  # set the block mode
  aes:set-block-mode cmod

  # create an input buffer
  trans ib (Buffer ivec)
  # create an output buffer
  const ob (Buffer)
  
  # stream the input buffer
  trans  blen (aes:stream ob ib)
  assert 16   blen
  assert 16   (ob:length)
  assert 0    (ib:length)
  
  if (== cmod afnix:sec:ModeCipher:MODE-ECB) {
    # create a verification buffer
    trans vb (Buffer ovec)
    # verify the output buffer
    loop (trans i 0) (< i blen) (i:++) {
      assert (vb:get i) (ob:get i)
    }
  }

  # reverse the cipher
  aes:set-reverse true
  assert true (aes:get-reverse)

  # stream the output buffer
  trans  blen (aes:stream ib ob)
  assert 16   blen
  assert 16   (ib:length)
  assert 0    (ob:length)
  
  # create a verification buffer
  trans vb (Buffer ivec)
  # verify the input buffer
  loop (trans i 0) (< i blen) (i:++) {
    assert (vb:get i) (ib:get i)
  }
}

# this expression test the aes cipher with a padding
const test-aes-cipher-with-padding (pmod cmod) {
  # create the aes cipher with a key
  const  aes   (afnix:sec:Aes key)
  assert "AES" (aes:get-name)
  
  # set the cipher padding mode
  aes:set-padding-mode pmod
  aes:set-block-mode   cmod
  assert pmod (aes:get-padding-mode)
  assert cmod (aes:get-block-mode)

  # create an input buffer
  trans ib (Buffer tvec)
  # create an output buffer
  const ob (Buffer)
  
  # stream the input buffer
  trans  blen (aes:stream ob ib)
  assert 16   blen
  assert 16   (ob:length)
  assert 0    (ib:length)
  
  # reverse the cipher
  aes:set-reverse true
  assert true (aes:get-reverse)
  
  # stream the output buffer
  trans  blen (aes:stream ib ob)
  assert 8    blen
  assert 8    (ib:length)
  assert 0    (ob:length)
  
  # create a verification buffer
  trans vb   (Buffer tvec)
  # verify the input buffer
  loop (trans i 0) (< i blen) (i:++) {
    assert (vb:get i) (ib:get i)
  }
}

const PAD-NONE      afnix:sec:ModeCipher:PAD-NONE
const PAD-BIT-MODE  afnix:sec:ModeCipher:PAD-BIT-MODE
const PAD-ANSI-X923 afnix:sec:ModeCipher:PAD-ANSI-X923
const PAD-NIST-800  afnix:sec:ModeCipher:PAD-NIST-800

const MODE-ECB      afnix:sec:ModeCipher:MODE-ECB
const MODE-CBC      afnix:sec:ModeCipher:MODE-CBC
const MODE-CFB      afnix:sec:ModeCipher:MODE-CFB
const MODE-OFB      afnix:sec:ModeCipher:MODE-OFB

# test the aes cipher with no padding
test-aes-cipher-no-padding MODE-ECB
test-aes-cipher-no-padding MODE-CBC
test-aes-cipher-no-padding MODE-CFB
test-aes-cipher-no-padding MODE-OFB

# test the aes cipher: bit padding
test-aes-cipher-with-padding PAD-BIT-MODE   MODE-ECB
test-aes-cipher-with-padding PAD-BIT-MODE   MODE-CBC
test-aes-cipher-with-padding PAD-BIT-MODE   MODE-CFB
test-aes-cipher-with-padding PAD-BIT-MODE   MODE-OFB

# test the aes cipher: ansi padding
test-aes-cipher-with-padding PAD-ANSI-X923  MODE-ECB
test-aes-cipher-with-padding PAD-ANSI-X923  MODE-CBC
test-aes-cipher-with-padding PAD-ANSI-X923  MODE-CFB
test-aes-cipher-with-padding PAD-ANSI-X923  MODE-OFB

# test the aes cipher: nist padding
test-aes-cipher-with-padding PAD-NIST-800   MODE-ECB
test-aes-cipher-with-padding PAD-NIST-800   MODE-CBC
test-aes-cipher-with-padding PAD-NIST-800   MODE-CFB
test-aes-cipher-with-padding PAD-NIST-800   MODE-OFB
