# ---------------------------------------------------------------------------
# - ITU0003.als                                                             -
# - afnix:itu module test unit                                              -
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

# @info   asn integer node test unit
# @author amaury darsch

# get the module
interp:library "afnix-itu"

# this function check the asn node
# @param ival the integer value to check
# @param bval the buffer format value
# @param cval the content value
const asn-integer-check (ival bval cval) {
  # create an asn integer node
  trans  ain  (afnix:itu:AsnInteger ival)
  assert true (afnix:itu:asn-node-p ain)
  assert true (afnix:itu:asn-integer-p ain)
  
  # check the name and content
  assert "AsnInteger" (ain:repr)
  assert (Relatif ival) (ain:to-relatif)

  # create a buffer and check
  trans buf (Buffer)
  ain:write buf
  assert bval (buf:format)
  
  # parse the buffer and check
  const ani (afnix:itu:AsnBuffer buf)
  assert afnix:itu:AsnNode:UNIVERSAL (ani:get-class)
  assert true  (ani:primitive-p)
  assert false (ani:constructed-p)
  assert 2     (ani:get-tag-number)
  
  # check the content
  const  cbuf (ani:get-content-buffer)
  assert (cbuf:length) (ani:get-content-length)
  assert cval (cbuf:format)
  
  # map the node to a boolean
  trans ain (ani:node-map)
  assert true (afnix:itu:asn-node-p  ain)
  assert true (afnix:itu:asn-integer-p ain)
  
  # create a buffer and check
  trans buf (Buffer)
  ain:write buf
  assert bval (buf:format)
}

# check value 0
asn-integer-check 0 "020100" "00"
# check positive value
asn-integer-check 0x1234 "02021234" "1234"
asn-integer-check 0x1234567890 "02051234567890" "1234567890"
# check negative value
asn-integer-check -1     "0201FF"     "FF"
asn-integer-check -128   "020180"     "80"
asn-integer-check -65536 "0203FF0000" "FF0000"
