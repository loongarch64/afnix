# ---------------------------------------------------------------------------
# - ITU0002.als                                                             -
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

# @info   asn boolean node test unit
# @author amaury darsch

# get the module
interp:library "afnix-itu"

# create an asn boolean node
trans  abn  (afnix:itu:AsnBoolean true)
assert true (afnix:itu:asn-node-p abn)
assert true (afnix:itu:asn-boolean-p  abn)

# check name and contents
assert "AsnBoolean" (abn:repr)
assert true (abn:to-boolean)

# create a buffer and check
trans buf (Buffer)
abn:write buf
assert "0101FF" (buf:format)

# parse the buffer and check
const anb (afnix:itu:AsnBuffer buf)
assert afnix:itu:AsnNode:UNIVERSAL (anb:get-class)
assert true  (anb:primitive-p)
assert false (anb:constructed-p)
assert 1     (anb:get-tag-number)

# check the content
const  cbuf (anb:get-content-buffer)
assert (cbuf:length) (anb:get-content-length)
assert "FF" (cbuf:format)

# map the node to a boolean
trans abn (anb:node-map)
assert true (afnix:itu:asn-node-p abn)
assert true (afnix:itu:asn-boolean-p  abn)

# create a buffer and check
trans buf (Buffer)
abn:write buf
assert "0101FF" (buf:format)
