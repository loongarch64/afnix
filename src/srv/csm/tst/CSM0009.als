# ---------------------------------------------------------------------------
# - CSM0009.als                                                             -
# - afnix:csm service test unit                                             -
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

# @info   tracker test unit
# @author amaury darsch

# get the module
interp:library "afnix-csm"

# create a default tracker
const name "tracker"
const info "afnix tracker test"
const tracker (afnix:csm:Tracker name info)

# check representation
assert true   (afnix:csm:blob-p tracker)
assert true   (afnix:csm:tracker-p tracker)
assert "Tracker" (tracker:repr)

# check accessor
assert name (tracker:get-name)
assert info (tracker:get-info)

# check host and port
const host "localhost"
const port 21900
tracker:set-host host
tracker:set-port port
assert host (tracker:get-host)
assert port (tracker:get-port)
