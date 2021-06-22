# ---------------------------------------------------------------------------
# - NWG0008.als                                                             -
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

# @info   cookie jar test unit
# @author amaury darsch

# get the module
interp:library "afnix-nwg"

# base cookie jar
const cjar (afnix:nwg:CookieJar)

assert true (afnix:nwg:cookie-jar-p cjar)
assert "CookieJar" (cjar:repr)

# parse a header string
const hval "SID=afnix;PREF=en"
cjar:parse hval

assert 2 (cjar:length)
assert true (cjar:exists-p "SID")
assert true (cjar:exists-p "PREF")

# check cookie 0
trans  cook (cjar:get 0)
assert "SID"   (cook:get-name)
assert "afnix" (cook:get-value)
 
# check cookie 1
trans  cook (cjar:get-cookie "PREF")
assert "PREF" (cook:get-name)
assert "en"   (cook:get-value)
