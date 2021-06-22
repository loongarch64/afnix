# ---------------------------------------------------------------------------
# - ITU0006.als                                                             -
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

# @info   asn bit string node test unit
# @author amaury darsch

# get the module
interp:library "afnix-itu"

# this function check the asn node
# @param bset the bitset to check
# @param bval the buffer format value
# @param cval the content value
const asn-bits-check (bset bval cval) {
  # create an asn bit string node
  trans  abn  (afnix:itu:AsnBits bset)
  assert true (afnix:itu:asn-node-p abn)
  assert true (afnix:itu:asn-bits-p  abn)

  # check name
  assert "AsnBits" (abn:repr)
  
  # create a buffer and check
  trans buf (Buffer)
  abn:write buf
  assert bval (buf:format)

  # parse the buffer and check
  const anb (afnix:itu:AsnBuffer buf)
  assert afnix:itu:AsnNode:UNIVERSAL (anb:get-class)
  assert true  (anb:primitive-p)
  assert false (anb:constructed-p)
  assert 3     (anb:get-tag-number)

  # check the content
  const  cbuf (anb:get-content-buffer)
  assert (cbuf:length) (anb:get-content-length)
  assert cval (cbuf:format)

  # map the node to a bit string
  trans abn (anb:node-map)
  assert true (afnix:itu:asn-node-p abn)
  assert true (afnix:itu:asn-bits-p abn)
  
  # create a buffer and check
  trans buf (Buffer)
  abn:write buf
  assert bval (buf:format)
}

# check value 0
asn-bits-check (Bitset) "030100" "00"

# check value 0x0A3B5F291CD
asn-bits-check (Bitset "0x0A3B5F291CD") "0307040A3B5F291CD0" "040A3B5F291CD0"
