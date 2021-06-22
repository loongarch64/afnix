# ---------------------------------------------------------------------------
# - ITU0018.als                                                             -
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

# @info   asn printable string node test unit
# @author amaury darsch

# get the module
interp:library "afnix-itu"

# this function check the asn node
# @param sval the string to check
# @param bval the buffer format value
# @param cval the content value
const asn-prts-check (sval bval cval) {
  # create an asn ia string node
  trans  ain  (afnix:itu:AsnPrts    sval)
  assert true (afnix:itu:asn-node-p ain)
  assert true (afnix:itu:asn-prts-p ain)

  # check name
  assert "AsnPrts" (ain:repr)
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
  assert 19    (ani:get-tag-number)

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

# check value 1234
asn-prts-check "afnix" "130561666E6978" "61666E6978"
