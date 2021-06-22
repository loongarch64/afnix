# ---------------------------------------------------------------------------
# - AXI0049.als                                                              -
# - afnix engine test module                                                -
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

# @info   resolver test module
# @author amaury darsch

# test the interpreter resolver
assert true (resolver-p interp:resolver)

# create a new resolver
const rslv (Resolver)
assert true (resolver-p rslv)

# add the local directory on the search path
rslv:add "."
assert true (rslv:valid-p "Makefile")
