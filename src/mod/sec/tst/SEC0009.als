# ---------------------------------------------------------------------------
# - SEC0009.als                                                             -
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

# @info   rc2 block cipher test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# this procedure test the rc2 cipher
const test-rc2-cipher-no-padding (kbuf ivec ovec ekey) {
  # create the key
  const key  (afnix:sec:Key kbuf)
  # create the cipher and check
  const rc2  (afnix:sec:Rc2 key)
  assert "RC2" (rc2:get-name)
  assert true  (afnix:sec:rc2-p rc2)
  # set the effective key
  rc2:set-effective-key ekey
  # bind the buffers
  trans ib   (Buffer ivec)
  trans ob   (Buffer)
  # encode the vector
  trans blen (rc2:stream ob ib)
  trans vb   (Buffer ovec)
  # verify the output buffer
  loop (trans i 0) (< i blen) (i:++) {
    assert (vb:get i) (ob:get i)
  }
  # reset in decode mode
  trans vb (Buffer ivec)
  rc2:set-reverse true
  # decode and verify
  trans blen (rc2:stream ib ob)
  loop (trans i 0) (< i blen) (i:++) {
    assert (vb:get i) (ib:get i)
  }
}

# this procedure test the rc2 cipher with padding
const test-rc2-cipher-with-padding (kbuf ivec ekey pmod cmod) {
  # create the key
  const key  (afnix:sec:Key kbuf)
  # create the cipher and check
  const  rc2   (afnix:sec:Rc2 key)
  assert "RC2" (rc2:get-name)
  assert true  (afnix:sec:rc2-p rc2)
  # set the effective key
  rc2:set-effective-key ekey

  # set the cipher padding mode
  rc2:set-padding-mode pmod
  rc2:set-block-mode   cmod
  assert pmod (rc2:get-padding-mode)
  assert cmod (rc2:get-block-mode)

  # bind the buffer
  trans ib (Buffer ivec)
  # encode the buffer
  trans ob (Buffer)
  rc2:stream ob ib
  # decode the buffer
  trans vb (Buffer)
  rc2:set-reverse true
  rc2:stream vb ob
  
  # rebuild the input buffer
  assert 0 (ib:length)
  trans  ib (Buffer ivec)
  trans  blen (ib:length)
  assert blen (vb:length)
  loop (trans i 0) (< i blen) (i:++) {
    assert (vb:get i) (ib:get i)
  }
}

# this procedure test the rc2 cipher
const test-rc2-cipher (kbuf ivec ovec ekey) {
  const PAD-NONE      afnix:sec:ModeCipher:PAD-NONE
  const PAD-BIT-MODE  afnix:sec:ModeCipher:PAD-BIT-MODE
  const PAD-ANSI-X923 afnix:sec:ModeCipher:PAD-ANSI-X923
  const PAD-NIST-800  afnix:sec:ModeCipher:PAD-NIST-800
  
  const MODE-ECB      afnix:sec:ModeCipher:MODE-ECB
  const MODE-CBC      afnix:sec:ModeCipher:MODE-CBC
  const MODE-CFB      afnix:sec:ModeCipher:MODE-CFB
  const MODE-OFB      afnix:sec:ModeCipher:MODE-OFB
  
  # test without padding
  test-rc2-cipher-no-padding kbuf ivec ovec ekey
  
  # test in pad bit mode
  test-rc2-cipher-with-padding kbuf ivec ekey PAD-BIT-MODE  MODE-ECB
  test-rc2-cipher-with-padding kbuf ivec ekey PAD-BIT-MODE  MODE-CBC
  test-rc2-cipher-with-padding kbuf ivec ekey PAD-BIT-MODE  MODE-CFB
  test-rc2-cipher-with-padding kbuf ivec ekey PAD-BIT-MODE  MODE-OFB

  # test in ansi mode
  test-rc2-cipher-with-padding kbuf ivec ekey PAD-ANSI-X923 MODE-ECB
  test-rc2-cipher-with-padding kbuf ivec ekey PAD-ANSI-X923 MODE-CBC
  test-rc2-cipher-with-padding kbuf ivec ekey PAD-ANSI-X923 MODE-CFB
  test-rc2-cipher-with-padding kbuf ivec ekey PAD-ANSI-X923 MODE-OFB

  # test in nist mode
  test-rc2-cipher-with-padding kbuf ivec ekey PAD-NIST-800  MODE-ECB
  test-rc2-cipher-with-padding kbuf ivec ekey PAD-NIST-800  MODE-CBC
  test-rc2-cipher-with-padding kbuf ivec ekey PAD-NIST-800  MODE-CFB
  test-rc2-cipher-with-padding kbuf ivec ekey PAD-NIST-800  MODE-OFB
}

# test 0 
trans kbuf "0000000000000000"
trans ivec (Vector
  0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
)
trans ovec (Vector
  0xeb 0xb7 0x73 0xf9 0x93 0x27 0x8e 0xff
)
test-rc2-cipher kbuf ivec ovec 63

# test 1
trans kbuf "ffffffffffffffff"
trans ivec (Vector
  0xff 0xff 0xff 0xff 0xff 0xff 0xff 0xff 
)
trans ovec (Vector
  0x27 0x8b 0x27 0xe4 0x2e 0x2f 0x0d 0x49
)
test-rc2-cipher-no-padding kbuf ivec ovec 64

# test 2
trans kbuf "3000000000000000"
trans ivec (Vector
  0x10 0x00 0x00 0x00 0x00 0x00 0x00 0x01 
)
trans ovec (Vector
  0x30 0x64 0x9e 0xdf 0x9b 0xe7 0xd2 0xc2
)
test-rc2-cipher-no-padding kbuf ivec ovec 64

# test 3
trans kbuf "88"
trans ivec (Vector
  0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
)
trans ovec (Vector
  0x61 0xa8 0xa2 0x44 0xad 0xac 0xcc 0xf0
)
test-rc2-cipher-no-padding kbuf ivec ovec 64

# test 4
trans kbuf "88bca90e90875a"
trans ivec (Vector
  0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
)
trans ovec (Vector
  0x6c 0xcf 0x43 0x08 0x97 0x4c 0x26 0x7f
)
test-rc2-cipher-no-padding kbuf ivec ovec 64

# test 5
trans kbuf "88bca90e90875a7f0f79c384627bafb2"
trans ivec (Vector
  0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
)
trans ovec (Vector
  0x1a 0x80 0x7d 0x27 0x2b 0xbe 0x5d 0xb1
)
test-rc2-cipher-no-padding kbuf ivec ovec 64

# test 6
trans kbuf "88bca90e90875a7f0f79c384627bafb2"
trans ivec (Vector
  0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
)
trans ovec (Vector
  0x22 0x69 0x55 0x2a 0xb0 0xf8 0x5c 0xa6
)
test-rc2-cipher-no-padding kbuf ivec ovec 128
