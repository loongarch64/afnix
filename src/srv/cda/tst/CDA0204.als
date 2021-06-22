# ---------------------------------------------------------------------------
# - CDA0204                                                             -
# - afnix:cda module test unit                                              -
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

# @info   linear streamer test unit
# @author amaury darsch

# load the cda generator
interp:load "CDA020X"

# check a lnr streamer
const lnr (new-lnr-streamer 1)
# create a streamable localizer
const sdl (afnix:cda:Stmdl lnr)

# check predicate
assert true (afnix:cda:localizer-p sdl)
assert true (afnix:cda:streamable-localizer-p sdl)
assert "Stmdl" (sdl:repr)

# locate in forward mode
assert 0.0 (sdl:locate 0.0)
assert 2.0 (sdl:locate 2.0)
assert 4.5 (sdl:locate 4.5)
assert 9.0 (sdl:locate 9.0)
assert 9.0 (sdl:locate true)

# verify at position
assert 0.0 (sdl:get-real 0.0)
assert 2.0 (sdl:get-real 2.0)
assert 4.5 (sdl:get-real 4.5)
assert 9.0 (sdl:get-real 9.0)
