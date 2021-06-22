# ---------------------------------------------------------------------------
# - ITU0021.als                                                             -
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

# @info   asn universal string node test unit
# @author amaury darsch

# get the module
interp:library "afnix-itu"

# this function check the asn node
# @param sval the string to check
# @param bval the buffer format value
# @param cval the content value
const asn-unvs-check (sval bval cval) {
  # create an asn ia string node
  trans  ain  (afnix:itu:AsnUnvs    sval)
  assert true (afnix:itu:asn-node-p ain)
  assert true (afnix:itu:asn-unvs-p ain)

  # check name
  assert "AsnUnvs" (ain:repr)
  assert sval      (ain:to-string)

  # create a buffer and check
  trans buf (Buffer)
  ain:write buf
  assert bval (buf:format)

  # parse the buffer and check
  const ani (afnix:itu:AsnBuffer buf)
  assert afnix:itu:AsnNode:UNIVERSAL (ani:get-class)
  assert true  (ani:primitive-p)
  assert false (ani:constructed-p)
  assert 28    (ani:get-tag-number)

  # check the content
  const  cbuf (ani:get-content-buffer)
  assert (cbuf:length) (ani:get-content-length)
  assert cval (cbuf:format)

  # map the node to a bit string
  trans ain (ani:node-map)
  assert true (afnix:itu:asn-node-p   ain)
  assert true (afnix:itu:asn-octets-p ain)
  
  # create a buffer and check
  trans buf (Buffer)
  ain:write buf
  assert bval (buf:format)
}

# check value afnix
const afnix-der-l "1C1400000061000000660000006E0000006900000078"
const afnix-der-s "00000061000000660000006E0000006900000078"
asn-unvs-check "afnix" afnix-der-l afnix-der-s
asn-unvs-check "α"     "1C04000003B1" "000003B1"
