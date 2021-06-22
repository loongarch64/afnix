# ---------------------------------------------------------------------------
# - ITU0201.als                                                             -
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

# @info   asn random octet string test unit
# @author amaury darsch

# get the module
interp:library "afnix-itu"

# this function check a random node
# @param encr encoding rule
# @param cstf the constructed flag
const asn-octs-check (encr cstf) {
  # create a random octets string
  const  aos  (afnix:itu:asn-random-octets 1001)
  assert true (afnix:itu:asn-octets-p aos)
  const  bs   (aos:to-buffer)
  assert 1001 (bs:length)
  
  # create a buffer and check
  trans buf (Buffer)
  aos:write encr buf
  trans bval (buf:format)

  # parse the buffer and check
  const ano (afnix:itu:AsnBuffer buf)
  assert afnix:itu:AsnNode:UNIVERSAL (ano:get-class)
  assert cstf (ano:constructed-p)
  assert 4    (ano:get-tag-number)
  
  # map the node to a bit string
  trans aon   (ano:node-map)
  assert true (afnix:itu:asn-node-p   aon)
  assert true (afnix:itu:asn-octets-p aon)
  
  # create a buffer and check
  trans buf (Buffer)
  aon:write encr buf
  assert bval (buf:format)
}

# check in ber mode
asn-octs-check afnix:itu:AsnNode:BER false
# check in der mode
asn-octs-check afnix:itu:AsnNode:DER false
# check in cer mode
asn-octs-check afnix:itu:AsnNode:CER true
