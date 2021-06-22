# ---------------------------------------------------------------------------
# - GFX0002.als                                                             -
# - afnix:gfx module test unit                                              -
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

# @info   vertex test unit
# @author amaury darsch

# get the module
interp:library "afnix-gfx"

# create a vertex
const  vrtx (afnix:gfx:Vertex)
assert true (afnix:gfx:vertex-p vrtx)

# add an edge to that vertex
vrtx:add (afnix:gfx:Edge)
assert 1 (vrtx:degree)

# release the vertex
vrtx:release
