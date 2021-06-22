# ---------------------------------------------------------------------------
# - ITU0004.als                                                             -
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

# @info   asn oid node test unit
# @author amaury darsch

# get the module
interp:library "afnix-itu"

# create an asn oid node
trans  aon  (afnix:itu:AsnOid 2 100 3)
assert true (afnix:itu:asn-node-p aon)
assert true (afnix:itu:asn-oid-p  aon)

# check name and contents
assert "AsnOid" (aon:repr)

# create a buffer and check
trans buf (Buffer)
aon:write buf
assert "0603813403" (buf:format)

# parse the buffer and check
const ano (afnix:itu:AsnBuffer buf)
assert afnix:itu:AsnNode:UNIVERSAL (ano:get-class)
assert true  (ano:primitive-p)
assert false (ano:constructed-p)
assert 6     (ano:get-tag-number)

# check the content
const  cbuf (ano:get-content-buffer)
assert (cbuf:length) (ano:get-content-length)
assert "813403" (cbuf:format)

# map the node to an oid
trans aon (ano:node-map)
assert true (afnix:itu:asn-node-p aon)
assert true (afnix:itu:asn-oid-p  aon)

# create a buffer and check
trans buf (Buffer)
aon:write buf
assert "0603813403" (buf:format)
