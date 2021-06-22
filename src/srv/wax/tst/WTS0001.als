# ---------------------------------------------------------------------------
# - WTS0001.als                                                             -
# - afnix:wax service test unit                                              -
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

# @info   wax test suite activator
# @author amaury darsch

# get the module
interp:library "afnix-wax"

# create a xsm html document and check
const  xsmd (afnix:wax:XsmHtml "WTS0001.htm")
assert true (afnix:wax:xsm-html-p xsmd)
assert true (> (xsmd:length) 0)
