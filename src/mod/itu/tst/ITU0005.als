# ---------------------------------------------------------------------------
# - ITU0005.als                                                             -
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

# @info   asn roid node test unit
# @author amaury darsch

# get the module
interp:library "afnix-itu"

# create an asn oid node
trans  arn  (afnix:itu:AsnRoid 8571 3 2)
assert true (afnix:itu:asn-node-p arn)
assert true (afnix:itu:asn-roid-p arn)

# check name and contents
assert "AsnRoid" (arn:repr)

# create a buffer and check
trans buf (Buffer)
arn:write buf
assert "0D04C27B0302" (buf:format)

# parse the buffer and check
const anr (afnix:itu:AsnBuffer buf)
assert afnix:itu:AsnNode:UNIVERSAL (anr:get-class)
assert true  (anr:primitive-p)
assert false (anr:constructed-p)
assert 13    (anr:get-tag-number)

# check the content
const  cbuf (anr:get-content-buffer)
assert (cbuf:length) (anr:get-content-length)
assert "C27B0302" (cbuf:format)

# map the node to a roid
trans  arn  (anr:node-map)
assert true (afnix:itu:asn-node-p arn)
assert true (afnix:itu:asn-roid-p arn)

# create a buffer and check
trans buf (Buffer)
arn:write buf
assert "0D04C27B0302" (buf:format)
