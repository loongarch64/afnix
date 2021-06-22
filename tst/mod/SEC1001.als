# ---------------------------------------------------------------------------
# - SEC1001.als                                                             -
# - afnix:sec module global test unit                                       -
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

# @info   aes block mode test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"
interp:library "afnix-sio"

# this expression test an input cipher
const test-input-cipher (ice icd) {
  # verify input ciphers
  assert true (afnix:sec:input-cipher-p ice)
  assert true (afnix:sec:input-cipher-p icd)

  # the test string
  const ts "afnix security module test suite"
  # create an input string
  const is (afnix:sio:InputString ts)
  # create an input/output stream
  const io (afnix:sio:InputOutput)
  # create an output string
  const os (afnix:sio:OutputString)
  
  # bind the input ciphers
  ice:set-input-stream is
  icd:set-input-stream io
  
  # encode the string
  while (ice:valid-p) (io:write (ice:read))
  # decode the string
  while (icd:valid-p) (os:write (icd:read))
  assert ts (os:to-string)
}

# create a symmetric key
const  key (afnix:sec:Key afnix:sec:Key:KSYM)
# create the encoding block cipher with a key
const  bce (afnix:sec:Aes key)
# create the decoding block cipher with a key
const  bcd (afnix:sec:Aes key true)

# create the input ciphers
trans ice (afnix:sec:InputCipher bce)
trans icd (afnix:sec:InputCipher bcd)

# test the input cipher
test-input-cipher ice icd

