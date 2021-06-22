# ---------------------------------------------------------------------------
# - NWG0002.als                                                             -
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

# @info   uri query test unit
# @author amaury darsch

# get the module
interp:library "afnix-nwg"

# create a new query string
const  data  "name=hello&data=hello%40world"
const  sval  "name=hello&data=hello@world"
const  query (afnix:nwg:UriQuery    data)
assert true  (afnix:nwg:uri-query-p query)

# and now check everything
assert sval          (query:get-query)
assert 2             (query:length)
assert true          (query:exists-p  "name")
assert true          (query:exists-p  "data")
assert "hello"       (query:get-value "name")
assert "hello@world" (query:get-value "data")

# check the property
trans  prop          (query:get 0)
assert "name"        (prop:get-name)
assert "hello"       (prop:get-value)

# check for lookup
trans  prop          (query:lookup "data")
assert "data"        (prop:get-name)
assert "hello@world" (prop:get-value)

# check for property value access
assert "hello"       (query:get-value "name")
assert "hello@world" (query:get-value "data")
