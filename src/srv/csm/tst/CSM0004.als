# ---------------------------------------------------------------------------
# - CSM0004.als                                                             -
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

# @info   session test unit
# @author amaury darsch

# get the service
interp:library "afnix-csm"

# the session name
const name "session"
const urid "session-user-rid"
const shid "session-hash-id"
# create a new session
const  sess (afnix:csm:Session name)
assert true (afnix:csm:session-p sess)

# check the name
assert name (sess:get-name)

# check hash id
sess:set-hash-id shid
assert shid (sess:get-hash-id)

# set and check path
const path "session-path"
sess:set-path path
assert path (sess:get-path)

# check expiration time
assert false (sess:expire-p)
sess:set-max-age 100
assert 100 (sess:get-max-age)

# get the session cookie
trans c (sess:get-cookie "cookie")
assert 100 (c:get-max-age)

# get a closing session cookie
trans c (sess:close)
assert 0 (sess:get-max-age)
assert 0 (c:get-max-age)
