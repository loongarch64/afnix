# ---------------------------------------------------------------------------
# - ITU0200.als                                                             -
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

# @info   asn random bitset test unit
# @author amaury darsch

# get the module
interp:library "afnix-itu"

# this function check the asn node
# @param encr encoding rule
const asn-bits-check (encr cstf) {
  # create a random bitset
  const  abs  (afnix:itu:asn-random-bits 8192)
  assert true (afnix:itu:asn-bits-p abs)
  const  bs   (abs:to-bits)
  assert 8192 (bs:length)
  
  # create a buffer and check
  trans buf (Buffer)
  abs:write encr buf
  trans bval (buf:format)

  # parse the buffer and check
  const anb (afnix:itu:AsnBuffer buf)
  assert afnix:itu:AsnNode:UNIVERSAL (anb:get-class)
  assert cstf (anb:constructed-p)
  assert 3    (anb:get-tag-number)
  
  # map the node to a bit string
  trans abn (anb:node-map)
  assert true (afnix:itu:asn-node-p abn)
  assert true (afnix:itu:asn-bits-p abn)
  
  # create a buffer and check
  trans buf (Buffer)
  abn:write encr buf
  assert bval (buf:format)
}

# check in ber mode
asn-bits-check afnix:itu:AsnNode:BER false
# check in der mode
asn-bits-check afnix:itu:AsnNode:DER false
# check in cer mode
asn-bits-check afnix:itu:AsnNode:CER true
