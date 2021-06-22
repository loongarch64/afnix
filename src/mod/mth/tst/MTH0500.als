# ---------------------------------------------------------------------------
# - MTH0500.als                                                             -
# - afnix:mth parser test unit                                              -
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

# @info   object parsing test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

const sv "0.0, 1.0, 2.0"
const rv (afnix:mth:to-rvi sv)
assert 3 (rv:get-size)
assert 0.0 (rv:get 0)
assert 1.0 (rv:get 1)
assert 2.0 (rv:get 2)
