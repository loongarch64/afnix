# ---------------------------------------------------------------------------
# - SEC0011.als                                                             -
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

# @info   rc5 block cipher test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# this procedure test the rc5 cipher without padding
const test-rc5-cipher-no-padding (kbuf ivec ovec) {
  # create the key
  const key  (afnix:sec:Key kbuf)
  # create the cipher and check
  const rc5  (afnix:sec:Rc5 key)
  assert "RC5" (rc5:get-name)
  assert true  (afnix:sec:rc5-p rc5)
  # bind the buffers
  trans ib   (Buffer ivec)
  trans ob   (Buffer)
  # encode the vector
  trans blen (rc5:stream ob ib)
  trans vb   (Buffer ovec)
  # verify the output buffer
  loop (trans i 0) (< i blen) (i:++) {
    assert (vb:get i) (ob:get i)
  }
  # reset in decode mode
  trans vb (Buffer ivec)
  rc5:set-reverse true
  # decode and verify
  trans blen (rc5:stream ib ob)
  loop (trans i 0) (< i blen) (i:++) {
    assert (vb:get i) (ib:get i)
  }
}

# this procedure test the rc5 cipher with padding
const test-rc5-cipher-with-padding (kbuf ivec pmod cmod) {
  # create the key
  const key  (afnix:sec:Key kbuf)
  # create the cipher and check
  const  rc5   (afnix:sec:Rc5 key)
  assert "RC5" (rc5:get-name)
  assert true  (afnix:sec:rc5-p rc5)

  # set the cipher padding mode
  rc5:set-padding-mode pmod
  rc5:set-block-mode   cmod
  assert pmod (rc5:get-padding-mode)
  assert cmod (rc5:get-block-mode)

  # bind the buffer
  trans ib (Buffer ivec)
  # encode the buffer
  trans ob (Buffer)
  rc5:stream ob ib
  # decode the buffer
  trans vb (Buffer)
  rc5:set-reverse true
  rc5:stream vb ob
  # rebuild the input buffer
  assert 0 (ib:length)
  trans  ib (Buffer ivec)
  trans  blen (ib:length)
  assert blen (vb:length)
  loop (trans i 0) (< i blen) (i:++) {
    assert (vb:get i) (ib:get i)
  }
}

# this procedure test the rc5 cipher
const test-rc5-cipher (kbuf ivec ovec) {
  const PAD-NONE      afnix:sec:ModeCipher:PAD-NONE
  const PAD-BIT-MODE  afnix:sec:ModeCipher:PAD-BIT-MODE
  const PAD-ANSI-X923 afnix:sec:ModeCipher:PAD-ANSI-X923
  const PAD-NIST-800  afnix:sec:ModeCipher:PAD-NIST-800
  
  const MODE-ECB      afnix:sec:ModeCipher:MODE-ECB
  const MODE-CBC      afnix:sec:ModeCipher:MODE-CBC
  const MODE-CFB      afnix:sec:ModeCipher:MODE-CFB
  const MODE-OFB      afnix:sec:ModeCipher:MODE-OFB
  
  # test without padding
  test-rc5-cipher-no-padding kbuf ivec ovec
  
  # test in pad bit mode
  test-rc5-cipher-with-padding kbuf ivec PAD-BIT-MODE  MODE-ECB
  test-rc5-cipher-with-padding kbuf ivec PAD-BIT-MODE  MODE-CBC
  test-rc5-cipher-with-padding kbuf ivec PAD-BIT-MODE  MODE-CFB
  test-rc5-cipher-with-padding kbuf ivec PAD-BIT-MODE  MODE-OFB

  # test in ansi mode
  test-rc5-cipher-with-padding kbuf ivec PAD-ANSI-X923 MODE-ECB
  test-rc5-cipher-with-padding kbuf ivec PAD-ANSI-X923 MODE-CBC
  test-rc5-cipher-with-padding kbuf ivec PAD-ANSI-X923 MODE-CFB
  test-rc5-cipher-with-padding kbuf ivec PAD-ANSI-X923 MODE-OFB

  # test in nist mode
  test-rc5-cipher-with-padding kbuf ivec PAD-NIST-800  MODE-ECB
  test-rc5-cipher-with-padding kbuf ivec PAD-NIST-800  MODE-CBC
  test-rc5-cipher-with-padding kbuf ivec PAD-NIST-800  MODE-CFB
  test-rc5-cipher-with-padding kbuf ivec PAD-NIST-800  MODE-OFB
}

# test 1 
trans kbuf "00000000000000000000000000000000"
trans ivec (Vector
  0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
)
trans ovec (Vector
  0xee 0xdb 0xa5 0x21 0x6d 0x8f 0x4b 0x15
)
test-rc5-cipher kbuf ivec ovec

# test 2 
trans kbuf "915F4619BE41B2516355A50110A9CE91"
trans ivec (Vector
  0xee 0xdb 0xa5 0x21 0x6d 0x8f 0x4b 0x15
)
trans ovec (Vector
  0xac 0x13 0xc0 0xf7 0x52 0x89 0x2b 0x5b
)
test-rc5-cipher kbuf ivec ovec

# test 3
trans kbuf "783348E75AEB0F2FD7B169BB8DC16787"
trans ivec (Vector
  0xac 0x13 0xc0 0xf7 0x52 0x89 0x2b 0x5b
)
trans ovec (Vector
  0xb7 0xb3 0x42 0x2f 0x92 0xfc 0x69 0x03
)
test-rc5-cipher kbuf ivec ovec

# test 4
trans kbuf "DC49DB1375A5584F6485B413B5F12BAF"
trans ivec (Vector
  0xb7 0xb3 0x42 0x2f 0x92 0xfc 0x69 0x03
)
trans ovec (Vector
  0xb2 0x78 0xc1 0x65 0xcc 0x97 0xd1 0x84
)
test-rc5-cipher kbuf ivec ovec

# test 5
trans kbuf "5269F149D41BA0152497574D7F153125"
trans ivec (Vector
  0xb2 0x78 0xc1 0x65 0xcc 0x97 0xd1 0x84
)
trans ovec (Vector
  0x15 0xe4 0x44 0xeb 0x24 0x98 0x31 0xda
)
test-rc5-cipher kbuf ivec ovec
