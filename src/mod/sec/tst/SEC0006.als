# ---------------------------------------------------------------------------
# - SEC0006.als                                                             -
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

# @info   rsa test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# global key type
const KRSA afnix:sec:Key:KRSA

# the input vector test
const ivec (Vector
  0x20 0x54 0x68 0x69 0x73 0x20 0x69 0x73
  0x20 0x61 0x20 0x74 0x65 0x73 0x74 0x20
  0x6d 0x65 0x73 0x73 0x61 0x67 0x65 0x20
  0x21 0x21 0x21
)

# this expression test a rsa cipher primitives
const test-rsa-primitives nil {
  # create a rsa key
  const  key (afnix:sec:Key KRSA 256)
  assert 256 (key:get-bits)
  
  # create the rsa cipher with a key
  const  rsa   (afnix:sec:Rsa key)
  assert "RSA" (rsa:get-name)
  
  # check for trivial case: 0
  trans msg 0
  rsa:set-reverse false
  trans ppm (rsa:pkcs-primitive msg)
  # enable reverse mode
  rsa:set-reverse true
  trans rpm (rsa:pkcs-primitive ppm)
  # and now the test
  assert rpm msg
  
  # check for trivial case: 1
  trans msg 1
  rsa:set-reverse false
  trans ppm (rsa:pkcs-primitive msg)
  # enable reverse mode
  rsa:set-reverse true
  trans rpm (rsa:pkcs-primitive ppm)
  # and now the test
  assert rpm msg
  
  # check for single message
  trans msg 2
  rsa:set-reverse false
  trans ppm (rsa:pkcs-primitive msg)
  # enable reverse mode
  rsa:set-reverse true
  trans rpm (rsa:pkcs-primitive ppm)
  # and now the test
  assert rpm msg
  
  # check again
  trans msg 1234567890R
  rsa:set-reverse false
  trans ppm (rsa:pkcs-primitive msg)
  # enable reverse mode
  rsa:set-reverse true
  trans rpm (rsa:pkcs-primitive ppm)
  # and now the test
  assert rpm msg
  
  # check again and again
  trans msg 0xFEDC_BA98_7654_3210R
  rsa:set-reverse false
  trans ppm (rsa:pkcs-primitive msg)
  # enable reverse mode
  rsa:set-reverse true
  trans rpm (rsa:pkcs-primitive ppm)
  # and now the test
  assert rpm msg
}


# this expression run the test with a rsa cipher
const test-rsa-cipher (rsa estr) {
  # create an input buffer
  trans ib (Buffer ivec)
  # create an output buffer
  const ob (Buffer)
  
  # stream the input buffer
  trans  blen 0
  while (not (ib:empty-p)) (blen:+= (rsa:stream ob ib))
  assert 27  blen
  assert 0   (ib:length)
  if (string-p estr) (assert estr (ob:format))

  # reverse the cipher
  rsa:set-reverse true
  assert true (rsa:get-reverse)
  
  # stream the output buffer
  trans blen  0
  while (not (ob:empty-p)) (blen:+= (rsa:stream ib ob))
  assert 27   (ib:length)
  assert 0    (ob:length)

  # create a verification buffer
  trans vb (Buffer ivec)
  # verify the input buffer
  loop (trans i 0) (< i 27) (i:++) {
    assert (vb:get i) (ib:get i)
  }
}

# this expression test a rsa cipher with a padding mode
const test-rsa-with-pkcs-padding (pmod) {
  # create a rsa key
  const  key (afnix:sec:Key KRSA 256)
  assert 256 (key:get-bits)
  
  # create the rsa cipher with a key and padding mode
  const rsa (afnix:sec:Rsa key)
  rsa:set-padding-mode pmod
  assert pmod (rsa:get-padding-mode)

  # test the cipher
  test-rsa-cipher rsa nil
}

# this expression test a rsa cipher with the iso vectors
const test-rsa-with-iso-vectors nil {
  # the key modulus
  const pmod 1096769317767533941413945645147207342367965840228428205076◀
  ▶13945978304098920529412415619708851314423671483225500317195833435789◀
  ▶17449141787186426037506627888557423265325642543429611377397387454273◀
  ▶33226003651562339652352922811469386523033747515254261027325307114304◀
  ▶73466903656428846184387282528950095967567885381R
  # the key public exponent
  const pexp 65537
  # the key secret exponent
  const sexp 3660471991017176541579143551933238659019258864964281265488◀
  ▶25829742503556111622417530074597815707217139379677337753944255114060◀
  ▶24286292981236354535359029519290638239564098647918888913628461944486◀
  ▶69544519319080994844659626904296671413376421170774304178924754428466◀
  ▶0831177834928904892102326793526435136473548141R

  # initialize the key
  const key (afnix:sec:Key KRSA (Vector pmod pexp sexp))
  # initialize the label and seed
  const labl "Label"
  const seed "D6E168C5F256A2DCFF7EF12FACD390F393C7A88D"
  trans hash (afnix:sec:Sha1)

  # ISO 18033-2 test vector SHA-1 (KDF1)
  trans mode afnix:sec:Rsa:PAD-OAEP-K1
  trans estr "4712734B1D3C9E43BC8CA30F4D93C88B6273075CB59A63ED2DE383CF1A719AFC4299919813F3B775153EF66121FEA89821E6EF57427CBB03628884DB2AED8E980BCE931205EFDD3D6EE2E2FFC32A8266176CEEE26DDA7E3ED664C70C97C21187E97E1CCAFA0C1B2E504552FF81D2AA683D89C77B37E9F7818AAF09B7FB585DAF"

  # create the rsa cipher in oaep mode with label and seed
  trans rsa (afnix:sec:Rsa key hash labl)
  rsa:set-padding-mode mode
  rsa:set-padding-seed seed

  # standard verification
  assert mode (rsa:get-padding-mode)
  assert labl (rsa:get-padding-label)
  assert seed (rsa:get-padding-seed)

  # test the cipher
  test-rsa-cipher rsa estr

  # ISO 18033-2 test vector SHA-1 (KDF2)
  trans mode afnix:sec:Rsa:PAD-OAEP-K2
  trans estr "7E72DB6F8D55E9EF81E7486A891DD6F3399CD6275F817CF2978A64577FC276E8A8B0108D42D671867E22FD76EE2B59CCA834A548AEB7B8F1E635AD719A9530B435D2BC8D2B15EEB2E162E9573D9765BCC9E4FBEDEDDF6F1EF277AED2449214FFCB998734E1D1BA948E84E79F67D2C2A441509899222DE4131819718BDE30C471"

  # create the rsa cipher in oaep mode with label and seed
  trans rsa (afnix:sec:Rsa key hash labl)
  rsa:set-padding-mode mode
  rsa:set-padding-seed seed

  # standard verification
  assert mode (rsa:get-padding-mode)
  assert labl (rsa:get-padding-label)
  assert seed (rsa:get-padding-seed)

  # test the cipher
  test-rsa-cipher rsa estr

  # ISO 18033-2 test vector SHA-256 (OUTLEN=20) (KDF1)
  trans mode afnix:sec:Rsa:PAD-OAEP-K1
  trans hash (afnix:sec:Sha256 20)
  trans estr "04652A946C1B2A9CADE87C46F1995F1A531008CD04BC10D46C850094234C856DD857140A46F9D4D059B5E184DC5F57BAAC374655911EBA712D0B2CD4A92AF5EBCFD5EFCDC484B8236E85F237C2EEC163FE836AD4C002D6604FE0021F4B3835028C98AF97E3C37C646227E1C488BFF9BFD4DC430AC04B4AADC7A9CF6D335E4913"

  # create the rsa cipher in oaep mode with label and seed
  trans rsa (afnix:sec:Rsa key hash labl)
  rsa:set-padding-mode mode
  rsa:set-padding-seed seed

  # standard verification
  assert mode (rsa:get-padding-mode)
  assert labl (rsa:get-padding-label)
  assert seed (rsa:get-padding-seed)

  # test the cipher
  test-rsa-cipher rsa estr

  # ISO 18033-2 test vector SHA-256 (OUTLEN=20) (KDF3)
  trans mode afnix:sec:Rsa:PAD-OAEP-K2
  trans hash (afnix:sec:Sha256 20)
  trans estr "4565D8B8EDD717044FBEE766D4E7B20E17AC060DB1A3CC7087CF4DEE0ADC68EEB1B91958C83187419730595237A31DDB24277754705DB809DA5B4B3C2A9A0E711AAD622FC1E334785D2EB2EA673F883D2036247AC3CAAC578EB14915126000CBB06A8AD716A4B39A80C184387E3B170193D2DF02864672F5ABCA52AC0A638419"

  # create the rsa cipher in oaep mode with label and seed
  trans rsa (afnix:sec:Rsa key hash labl)
  rsa:set-padding-mode mode
  rsa:set-padding-seed seed

  # standard verification
  assert mode (rsa:get-padding-mode)
  assert labl (rsa:get-padding-label)
  assert seed (rsa:get-padding-seed)

  # test the cipher
  test-rsa-cipher rsa estr
}

# test the rsa primitives
test-rsa-primitives

# test the rsa padding modes
test-rsa-with-pkcs-padding afnix:sec:Rsa:PAD-PKCS-11
test-rsa-with-pkcs-padding afnix:sec:Rsa:PAD-PKCS-12

# test rsa with iso vectors
test-rsa-with-iso-vectors
