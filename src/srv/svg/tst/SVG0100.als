# ---------------------------------------------------------------------------
# - SVG0100.als                                                             -
# - afnix:svg service test unit                                             -
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

# @info   svg plot2d test unit
# @author amaury darsch

# get the module
interp:library "afnix-svg"

# create a plot2d object
const ptd (afnix:svg:Plot2d)

# check predicate and representation
assert true (afnix:svg:plot2d-p ptd)
assert "Plot2d" (ptd:repr)
