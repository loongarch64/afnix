# ---------------------------------------------------------------------------
# - ITU0010.als                                                             -
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

# @info   asn sequence test unit
# @author amaury darsch

# get the module
interp:library "afnix-itu"

# create a sequence node
trans  asn  (afnix:itu:AsnSequence)
assert true (afnix:itu:asn-node-p asn)
assert true (afnix:itu:asn-sequence-p  asn)

# check the name
assert "AsnSequence" (asn:repr)

# check the content length
assert 0 (asn:node-length)

# add some nodes and check
asn:node-add (afnix:itu:AsnBoolean)
asn:node-add (afnix:itu:AsnInteger 0x123456789)
assert 2 (asn:node-length)

# create a buffer and check
trans buf (Buffer)
asn:write buf
assert "300A01010002050123456789" (buf:format)

# parse the buffer and check
const ans (afnix:itu:AsnBuffer buf)
assert afnix:itu:AsnNode:UNIVERSAL (ans:get-class)
assert false (ans:primitive-p)
assert true  (ans:constructed-p)
assert 16    (ans:get-tag-number)

# check the content
const  cbuf (ans:get-content-buffer)
assert (cbuf:length) (ans:get-content-length)
assert "01010002050123456789" (cbuf:format)

# map the node to a sequence
trans asn (ans:node-map)
assert true (afnix:itu:asn-node-p asn)
assert true (afnix:itu:asn-sequence-p asn)

# verify the sequence
assert 2 (asn:node-length)

# get the first node
const abn (asn:node-get 0)
assert true  (afnix:itu:asn-boolean-p  abn)
assert false (abn:to-boolean)

# get the second node
const ain (asn:node-get 1)
assert true (afnix:itu:asn-integer-p  ain)
assert (Relatif 0x123456789) (ain:to-relatif)
