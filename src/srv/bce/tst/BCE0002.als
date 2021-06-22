# ---------------------------------------------------------------------------
# - BCE0002.als                                                             -
# - afnix:bce service test unit                                             -
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

# @info   ledger test unit
# @author amaury darsch

# get the service
interp:library "afnix-bce"

# create a ledger
const  lgdr  (afnix:bce:Ledger)
assert true  (afnix:bce:ledger-p lgdr)
assert "Ledger" (lgdr:repr)

# check content
assert 0 (lgdr:length)
