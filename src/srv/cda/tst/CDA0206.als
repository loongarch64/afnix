# ---------------------------------------------------------------------------
# - CDA0206                                                             -
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

# @info   sps sampler test unit
# @author amaury darsch

# load the cda generator
interp:load "CDA020X"

# create a linear streamer
const lnr (new-lnr-streamer 1)
# create a streamable localizer
const sdl (afnix:cda:Stmdl lnr)
# create a sampler
const spl (afnix:cda:Sampler sdl lnr)

# check predicate
assert true (afnix:cda:sampler-p spl)
assert "Sampler" (spl:repr)

# check localizer
assert afnix:cda:Localizer:ABSOLUTE-FORWARD (sdl:get-localization-method)

# sample in real mode
assert 0.0 (spl:map-real 0.0)
assert 1.0 (spl:map-real 1.0)
assert 2.5 (spl:map-real 2.5)
assert 9.0 (spl:map-real 9.0)
assert 9.0 (spl:map-real 10.0)

# validate a localized position
assert true  (sdl:validate 0.0)
assert true  (sdl:validate 1.0)
assert true  (sdl:validate 2.5)
assert true  (sdl:validate 9.0)
assert false (sdl:validate 10.0)

# validate a localized position
assert true  (spl:validate 0.0)
assert true  (spl:validate 1.0)
assert true  (spl:validate 4.0)
assert true  (spl:validate 9.0)
assert false (spl:validate 10.0)
