# ---------------------------------------------------------------------------
# - ITU0007.als                                                             -
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

# @info   asn octet string node test unit
# @author amaury darsch

# get the module
interp:library "afnix-itu"

# this function check the asn node
# @param sval the string to check
# @param bval the buffer format value
# @param cval the content value
const asn-octs-check (sval bval cval) {
  # create an asn octet string node
  trans  aon  (afnix:itu:AsnOctets    sval)
  assert true (afnix:itu:asn-node-p   aon)
  assert true (afnix:itu:asn-octets-p aon)

  # check name
  assert "AsnOctets" (aon:repr)
  
  # create a buffer and check
  trans buf (Buffer)
  aon:write buf
  assert bval (buf:format)

  # parse the buffer and check
  const ano (afnix:itu:AsnBuffer buf)
  assert afnix:itu:AsnNode:UNIVERSAL (ano:get-class)
  assert true  (ano:primitive-p)
  assert false (ano:constructed-p)
  assert 4     (ano:get-tag-number)

  # check the content
  const  cbuf (ano:get-content-buffer)
  assert (cbuf:length) (ano:get-content-length)
  assert cval (cbuf:format)

  # map the node to a bit string
  trans aon (ano:node-map)
  assert true (afnix:itu:asn-node-p   aon)
  assert true (afnix:itu:asn-octets-p aon)
  
  # create a buffer and check
  trans buf (Buffer)
  aon:write buf
  assert bval (buf:format)
}

# check value 0
asn-octs-check "" "0400" ""

# check value afnix
asn-octs-check "afnix" "040561666E6978" "61666E6978"
