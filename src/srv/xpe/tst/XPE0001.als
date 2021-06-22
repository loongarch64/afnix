# ---------------------------------------------------------------------------
# - XPE0001.als                                                             -
# - afnix:xpe service test unit                                             -
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

# @info   xml processor test unit
# @author amaury darsch

# get the module
interp:library "afnix-xpe"

# create a xml processor
const  xprc (afnix:xpe:XmlProcessor)
assert true (afnix:xpe:xml-processor-p xprc)

# check feature
assert 0    (xprc:feature-length)
