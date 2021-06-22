# ---------------------------------------------------------------------------
# - WJS0001.als                                                             -
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

# @info   wax json writer test unit
# @author amaury darsch

# get the module
interp:library "afnix-wax"
interp:library "afnix-sio"

# create a json mime
const  json (afnix:wax:JsonMime)
assert true (afnix:wax:json-mime-p json)
assert "JsonMime" (json:repr)
assert "application/json" (json:get-mime)

# check a vector
const jvec (Vector 2000 "json" true)
json:reset
assert true (json:stringify jvec)
assert "[2000,\"json\",true]" (json:to-string)

# create an output string
const os  (afnix:sio:OutputString)
const buf (Buffer)

# write the data and compare
json:write os
json:write buf
assert (os:to-string) (buf:to-string)

