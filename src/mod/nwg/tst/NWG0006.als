# ---------------------------------------------------------------------------
# - NWG0006.als                                                             -
# - afnix:nwg module test unit                                              -
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

# @info   http response test unit
# @author amaury darsch

# get the module
interp:library "afnix-nwg"

# create a response
trans  hrs   (afnix:nwg:HttpResponse)
assert true  (afnix:nwg:http-proto-p    hrs)
assert true  (afnix:nwg:http-response-p hrs)

# check the default http response
assert false (hrs:status-p)
assert false (hrs:status-ok-p)
assert false (hrs:location-p)
assert 0     (hrs:get-status-code)

assert false (hrs:media-type-p)
assert false (hrs:encoding-mode-p)

# set the location
trans  uri   "http://www.afnix.org"
hrs:set-location uri
assert true  (hrs:location-p)
assert 303   (hrs:get-status-code)
assert true  (string-p (hrs:map-status-code))
assert uri   (hrs:get-location)

# test with a code
trans  hrs   (afnix:nwg:HttpResponse 404 "application/xhtml+xml; charset=UTF-8")

assert true  (hrs:status-p)
assert true  (hrs:status-error-p)
assert false (hrs:status-ok-p)
assert false (hrs:location-p)
assert 404   (hrs:get-status-code)

assert true  (hrs:media-type-p)
assert "application/xhtml+xml" (hrs:get-media-type)

assert true    (hrs:encoding-mode-p)
assert "UTF-8" (hrs:get-encoding-mode)
