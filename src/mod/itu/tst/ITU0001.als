# ---------------------------------------------------------------------------
# - ITU0001.als                                                             -
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

# @info   asn node test unit
# @author amaury darsch

# get the module
interp:library "afnix-itu"

# create a eoc node
const  eoc  (afnix:itu:AsnEoc)
assert true (afnix:itu:asn-node-p eoc)
assert true (afnix:itu:asn-eoc-p  eoc)

# check the name
assert "AsnEoc" (eoc:repr)

# create a buffer and check
const buf (Buffer)
eoc:write buf
assert "0000" (buf:format)

# create a buffer node
const anb (afnix:itu:AsnBuffer)
