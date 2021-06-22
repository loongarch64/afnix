# ---------------------------------------------------------------------------
# - NWG0010.als                                                             -
# - afnix:nwg module test unit                                              -
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

# @info   basexx test unit
# @author amaury darsch

# get the module
interp:library "afnix-nwg"

# default base codec
trans encd (afnix:nwg:Basexx)
trans decd (afnix:nwg:Basexx)
decd:set-reverse true

assert true (afnix:nwg:basexx-p encd)
assert "Basexx" (encd:repr)

# the base test function
const test-base-xx (si so) {
  trans  ib (Buffer si)
  trans  eb (Buffer)
  trans  elen (encd:stream eb ib)
  if (not (encd:get-split-mode)) (assert elen (eb:length))
  assert so (eb:to-string)
  trans  db (Buffer)
  trans  dlen (decd:stream db eb)
  if (not (encd:get-split-mode)) (assert dlen (db:length))
  assert si (db:to-string)  
}

# test various buffers
test-base-xx "" ""
test-base-xx "0" "MA=="
test-base-xx "00" "MDA="
test-base-xx "0123456789" "MDEyMzQ1Njc4OQ=="

# rfc 4346 test vectors
test-base-xx "f" "Zg=="
test-base-xx "fo" "Zm8="
test-base-xx "foo" "Zm9v"
test-base-xx "foob" "Zm9vYg=="
test-base-xx "fooba" "Zm9vYmE="
test-base-xx "foobar" "Zm9vYmFy"

# base 32 codec
trans encd (afnix:nwg:Basexx afnix:nwg:Basexx:SC32)
trans decd (afnix:nwg:Basexx afnix:nwg:Basexx:SC32)
decd:set-reverse true

# test various buffers
test-base-xx "" ""
test-base-xx "0" "GA======"
test-base-xx "00" "GAYA===="
test-base-xx "0123456789" "GAYTEMZUGU3DOOBZ"

# rfc 4346 test vectors
test-base-xx "f" "MY======"
test-base-xx "fo" "MZXQ===="
test-base-xx "foo" "MZXW6==="
test-base-xx "foob" "MZXW6YQ="
test-base-xx "fooba" "MZXW6YTB"
test-base-xx "foobar" "MZXW6YTBOI======"

# base 16 codec
trans encd (afnix:nwg:Basexx afnix:nwg:Basexx:SC16)
trans decd (afnix:nwg:Basexx afnix:nwg:Basexx:SC16)
decd:set-reverse true

# rfc 4346 test vectors
test-base-xx "" ""
test-base-xx "f" "66"
test-base-xx "fo" "666F"
test-base-xx "foo" "666F6F"
test-base-xx "foob" "666F6F62"
test-base-xx "fooba" "666F6F6261"
test-base-xx "foobar" "666F6F626172"

# base 64 codec
trans encd (afnix:nwg:Basexx afnix:nwg:Basexx:SC64)
trans decd (afnix:nwg:Basexx afnix:nwg:Basexx:SC64)
decd:set-reverse true

# test in split line mode
encd:set-split-mode true
assert true (encd:get-split-mode)

# test long string
trans imsg ""
imsg:+= "0123456789012345678901234567890123456789"
imsg:+= "0123456789012345678901234567890123456789"
imsg:+= "0123456789012345678901234567890123456789"
imsg:+= "0123456789012345678901234567890123456789"

trans emsg ""
emsg:+= "MDEyMzQ1Njc4OTAxMjM0NTY3ODkwMTIzNDU2Nzg5MDEyMzQ1Njc4OTAxMjM0NTY3\n"
emsg:+= "ODkwMTIzNDU2Nzg5MDEyMzQ1Njc4OTAxMjM0NTY3ODkwMTIzNDU2Nzg5MDEyMzQ1\n"
emsg:+= "Njc4OTAxMjM0NTY3ODkwMTIzNDU2Nzg5MDEyMzQ1Njc4OTAxMjM0NTY3ODkwMTIz\n"
emsg:+= "NDU2Nzg5MDEyMzQ1Njc4OQ=="
test-base-xx imsg emsg

# test serialize/deserialize
trans ival 123456789
trans sbuf (encd:serialize ival true)
assert ival (decd:unserialize sbuf true)

# test serialize/deserialize
trans ival 123456789
trans sbuf (encd:serialize ival true)
const sval (sbuf:to-string)
assert ival (decd:unserialize (Buffer sval) true)
