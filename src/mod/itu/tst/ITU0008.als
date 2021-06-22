# ---------------------------------------------------------------------------
# - ITU0008.als                                                             -
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

# @info   asn null node test unit
# @author amaury darsch

# get the module
interp:library "afnix-itu"

# create an asn null node
trans  ann  (afnix:itu:AsnNull)
assert true (afnix:itu:asn-node-p ann)
assert true (afnix:itu:asn-null-p ann)

# check name and contents
assert "AsnNull" (ann:repr)

# create a buffer and check
trans buf (Buffer)
ann:write buf
assert "0500" (buf:format)

# parse the buffer and check
const nna (afnix:itu:AsnBuffer buf)
assert afnix:itu:AsnNode:UNIVERSAL (nna:get-class)
assert true  (nna:primitive-p)
assert false (nna:constructed-p)
assert 5     (nna:get-tag-number)

# check the content
const  cbuf (nna:get-content-buffer)
assert (cbuf:length) (ann:get-content-length)

# map the node to a null node
trans ann (nna:node-map)
assert true (afnix:itu:asn-node-p ann)
assert true (afnix:itu:asn-null-p ann)

# create a buffer and check
trans buf (Buffer)
ann:write buf
assert "0500" (buf:format)
