# ---------------------------------------------------------------------------
# - PHY0001.als                                                             -
# - afnix:phy service test unit                                             -
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

# @info   physics test unit
# @author amaury darsch

# get the module
interp:library "afnix-phy"

# create a physics object by name
const c (afnix:phy:to-physics "c")
assert true (afnix:phy:physics-p c)
assert "Physics" (c:repr)
assert 299792458.0 (c:get-scaling-factor)
assert "m.s^-1" (c:to-si)

