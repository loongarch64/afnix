# ---------------------------------------------------------------------------
# - SEC0010.als                                                             -
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

# @info   rc4 serial cipher test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# this procedure test the rc4 cipher
const test-rc4-cipher (kbuf ivec ovec) {
  # create the key
  const key  (afnix:sec:Key kbuf)
  # create the cipher and check
  const  rc4   (afnix:sec:Rc4 key)
  assert "RC4" (rc4:get-name)
  assert true  (afnix:sec:rc4-p rc4)
  # bind the buffers
  trans ib   (Buffer ivec)
  trans ob   (Buffer)
  # encode the vector
  trans blen (rc4:stream ob ib)
  trans vb   (Buffer ovec)
  # verify the output buffer
  loop (trans i 0) (< i blen) (i:++) {
    assert (vb:get i) (ob:get i)
  }
  # reset in decode mode
  trans vb (Buffer ivec)
  rc4:set-reverse true
  # decode and verify
  trans blen (rc4:stream ib ob)
  loop (trans i 0) (< i blen) (i:++) {
    assert (vb:get i) (ib:get i)
  }
}

# test 0 
trans kbuf "0123456789ABCDEF"
trans ivec (Vector
  0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
)
trans ovec (Vector
  0x74 0x94 0xc2 0xe7 0x10 0x4b 0x08 0x79
)
test-rc4-cipher kbuf ivec ovec

# test 1 
trans kbuf "618A63D2FB"
trans ivec (Vector
  0xdc 0xee 0x4c 0xf9 0x2c
)
trans ovec (Vector
  0xf1 0x38 0x29 0xc9 0xde
)
test-rc4-cipher kbuf ivec ovec
