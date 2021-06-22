# ---------------------------------------------------------------------------
# - WAX0013.als                                                             -
# - afnix:wax module test unit                                              -
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

# @info   xhtml mime test unit
# @author amaury darsch

# get the module
interp:library "afnix-wax"

# create a new xhtml mime
const  xhtm (afnix:wax:XhtmlMime)
assert true (afnix:wax:xml-mime-p   xhtm)
assert true (afnix:wax:xhtml-mime-p xhtm)


