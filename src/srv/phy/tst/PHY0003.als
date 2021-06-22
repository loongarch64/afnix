# ---------------------------------------------------------------------------
# - PHY0002.als                                                             -
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

# @info   unit test unit
# @author amaury darsch

# get the module
interp:library "afnix-phy"

# create a periodic table
const ptbl (afnix:phy:Periodic)
assert true (afnix:phy:periodic-p ptbl)
assert "Periodic" (ptbl:repr)

# check the hydrogen atom
assert true (ptbl:exists-p "H")
trans  atom (ptbl:get-atom "H")
assert true (afnix:phy:atom-p atom)
assert "Atom" (atom:repr)
assert 1 (atom:get-atomic-number)
