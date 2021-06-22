# ---------------------------------------------------------------------------
# - CSM0056.als                                                             -
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

# @info   domain test unit
# @author amaury darsch

# get the service
interp:library "afnix-csm"

# create a nil domain
trans domn (afnix:csm:Domain)

# check predicate and representation
assert true (afnix:csm:domain-p domn)
assert "Domain" (domn:repr)

# create an integer carrier blob
const ica (afnix:csm:Carrier 1)

# check for empty
assert true  (domn:empty-p)
assert 0     (domn:length)

# add the carrier
trans  kid   (domn:add-part ica)
assert false (domn:empty-p)
assert 1     (domn:length)
assert true  (domn:exists-p (kid:to-string))

# get the carrier by key id and check
trans  blb   (domn:get-by-kid (kid:to-string))
assert 1     (blb:get-object)

# test the rid
domn:set-rid "RID"
assert true  (domn:rid-p "RID")
assert "RID" (domn:get-rid)

# test the whois
const wois (domn:whois)
assert (domn:get-rid) (wois:get-rid)
assert (domn:get-creation-time)  (wois:get-creation-time)
assert (domn:get-modification-time)  (wois:get-modification-time)
assert (domn:length) (wois:length)
