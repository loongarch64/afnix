# ---------------------------------------------------------------------------
# - GFX0003.als                                                             -
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

# @info   graph test unit
# @author amaury darsch

# get the module
interp:library "afnix-gfx"

# create a graph
const  g    (afnix:gfx:Graph)
assert true (afnix:gfx:graph-p g)

# create an edge and add vertices
const edge (afnix:gfx:Edge)
edge:add   (afnix:gfx:Vertex "hello") 
edge:add   (afnix:gfx:Vertex "world")
assert 2   (edge:cardinality)

# add the edge to the graph and check
g:add edge
assert 1 (g:number-of-edges)
assert 2 (g:number-of-vertices)

# check for nodes and edges
assert true (afnix:gfx:edge-p   (g:get-edge   0))
assert true (afnix:gfx:vertex-p (g:get-vertex 0))
assert true (afnix:gfx:vertex-p (g:get-vertex 1))

# release the graph
g:release
