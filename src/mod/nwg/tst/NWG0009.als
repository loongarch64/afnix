# ---------------------------------------------------------------------------
# - NWG0009.als                                                             -
# - afnix:nwg module test unit                                              -
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

# @info   uuid test unit
# @author amaury darsch

# get the module
interp:library "afnix-nwg"

# nil uuid
trans uuid (afnix:nwg:Uuid)

assert true (afnix:nwg:uuid-p uuid)
assert "Uuid" (uuid:repr)

# check string value
assert "00000000-0000-0000-0000-000000000000" (uuid:to-string)

# generate a random value
uuid:random
# compare string values
const  uval (uuid:to-string)
trans  uuid (afnix:nwg:Uuid uval)
assert uval (uuid:to-string)
