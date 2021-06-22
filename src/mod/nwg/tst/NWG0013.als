# ---------------------------------------------------------------------------
# - NWG0013.als                                                             -
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

# @info   uuid test unit
# @author amaury darsch

# get the module
interp:library "afnix-nwg"

# nil json
trans json (afnix:nwg:Json)
assert true (afnix:nwg:json-p json)
assert "Json" (json:repr)

# check a literal
json:reset
assert true (json:stringify 2000)
assert "2000" (json:to-string)

json:reset
assert true (json:stringify "json")
assert "\"json\"" (json:to-string)

json:reset
assert true (json:stringify true)
assert "true" (json:to-string)

# check a vector
const jvec (Vector 2000 "json" true)
json:reset
assert true (json:stringify jvec)
assert "[2000,\"json\",true]" (json:to-string)

# check a set
const jset (Set 2000 "json" true)
json:reset
assert true (json:stringify jset)
assert "[2000,\"json\",true]" (json:to-string)

# check a hashtable - keys makes the order
const jhsh (HashTable)
jhsh:add "0" 2000
jhsh:add "1" "json"
jhsh:add "2" true
json:reset
assert true (json:stringify jhsh)
assert "{\"0\":2000,\"1\":\"json\",\"2\":true}" (json:to-string)

# check a plist 
const jlst (Plist)
jlst:add "0" 2000
jlst:add "1" "json"
jlst:add "2" true
json:reset
assert true (json:stringify jlst)
assert "{\"0\":2000,\"1\":\"json\",\"2\":true}" (json:to-string)

# check a printtable 
const jtbl (PrintTable 2)
trans row (jtbl:add)
jtbl:set row 0 "00"
jtbl:set row 1 "01"
trans row (jtbl:add)
jtbl:set row 0 "10"
jtbl:set row 1 "11"
json:reset
assert true (json:stringify jtbl)
assert "[[\"00\",\"01\"],[\"10\",\"11\"]]"(json:to-string)
