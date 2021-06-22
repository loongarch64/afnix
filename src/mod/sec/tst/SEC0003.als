# ---------------------------------------------------------------------------
# - SEC0003.als                                                             -
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

# @info   des block cipher test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# NIST 800-67 64 bits key buffer
const kbuf "0123456789ABCDEF"

# NIST 800-67 64 bits input test vector
const ivec (Vector
  0x54 0x68 0x65 0x20 0x71 0x75 0x66 0x63 
)

# NIST 800-67 64 bits output test vector
const ovec (Vector 
  0xA2 0x8E 0x91 0x72 0x4C 0x4B 0xBA 0x31
)

# simple 5 bytes test buffer
const tvec (Vector 
  0x54 0x68 0x65 0x20 0x71
)

# create a key with a buffer
const  key (afnix:sec:Key kbuf)
assert 64  (key:get-bits)

# this expression test the des cipher with no padding
const test-des-cipher-no-padding (cmod) {
  # create the des cipher with a key
  const  des   (afnix:sec:Des key)
  assert "DES" (des:get-name)

  # force no padding
  des:set-padding-mode afnix:sec:ModeCipher:PAD-NONE
  # set the block mode
  des:set-block-mode cmod

  # create an input buffer
  trans ib (Buffer ivec)
  # create an output buffer
  const ob (Buffer)
  
  # stream the input buffer
  trans  blen (des:stream ob ib)
  assert 8    blen
  assert 8    (ob:length)
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
  des:set-reverse true
  assert true (des:get-reverse)

  # stream the output buffer
  trans  blen (des:stream ib ob)
  assert 8    blen
  assert 8    (ib:length)
  assert 0    (ob:length)
  
  # create a verification buffer
  trans vb (Buffer ivec)
  # verify the input buffer
  loop (trans i 0) (< i blen) (i:++) {
    assert (vb:get i) (ib:get i)
  }
}

# this expression test the des cipher with a padding
const test-des-cipher-with-padding (pmod cmod) {
  # create the aes cipher with a key
  const  des   (afnix:sec:Des key)
  assert "DES" (des:get-name)
  
  # set the cipher padding mode
  des:set-padding-mode pmod
  des:set-block-mode   cmod
  assert pmod (des:get-padding-mode)
  assert cmod (des:get-block-mode)

  # create an input buffer
  trans ib (Buffer tvec)
  # create an output buffer
  const ob (Buffer)
  
  # stream the input buffer
  trans  blen (des:stream ob ib)
  assert 8    blen
  assert 8    (ob:length)
  assert 0    (ib:length)
  
  # reverse the cipher
  des:set-reverse true
  assert true (des:get-reverse)
  
  # stream the output buffer
  trans  blen (des:stream ib ob)
  assert 5    blen
  assert 5    (ib:length)
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

# test the des cipher with no padding
test-des-cipher-no-padding MODE-ECB
test-des-cipher-no-padding MODE-CBC
test-des-cipher-no-padding MODE-CFB
test-des-cipher-no-padding MODE-OFB

# test the des cipher: bit padding
test-des-cipher-with-padding PAD-BIT-MODE   MODE-ECB
test-des-cipher-with-padding PAD-BIT-MODE   MODE-CBC
test-des-cipher-with-padding PAD-BIT-MODE   MODE-CFB
test-des-cipher-with-padding PAD-BIT-MODE   MODE-OFB

# test the aes cipher: ansi padding
test-des-cipher-with-padding PAD-ANSI-X923  MODE-ECB
test-des-cipher-with-padding PAD-ANSI-X923  MODE-CBC
test-des-cipher-with-padding PAD-ANSI-X923  MODE-CFB
test-des-cipher-with-padding PAD-ANSI-X923  MODE-OFB

# test the aes cipher: nist padding
test-des-cipher-with-padding PAD-NIST-800   MODE-ECB
test-des-cipher-with-padding PAD-NIST-800   MODE-CBC
test-des-cipher-with-padding PAD-NIST-800   MODE-CFB
test-des-cipher-with-padding PAD-NIST-800   MODE-OFB
