# ---------------------------------------------------------------------------
# - NWG0003.als                                                             -
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

# @info   cookie test unit
# @author amaury darsch

# get the module
interp:library "afnix-nwg"

# create a new cookie
const  cookie (afnix:nwg:Cookie "name" "value")
assert true   (afnix:nwg:cookie-p cookie)

# and now check everything
assert "name"  (cookie:get-name)
assert "value" (cookie:get-value)
assert  0      (cookie:get-version)
assert -1      (cookie:get-max-age)
assert -1      (cookie:get-port)

# check cookie version
cookie:set-version 1
assert "name=value; Version=1; Path=/" (cookie:to-string)

# check for max age
cookie:set-max-age 60
assert 60 (cookie:get-max-age)

# check for comment
cookie:set-comment "hello world"
assert "hello world" (cookie:get-comment)

# check for comment url
cookie:set-comment-url "http://hello"
assert "http://hello" (cookie:get-comment-url)

# check for path
cookie:set-path "/cgi"
assert "/cgi" (cookie:get-path)

# check for domain
cookie:set-domain ".afnix.org"
assert ".afnix.org" (cookie:get-domain)

# check for port
cookie:set-port 80
assert 80 (cookie:get-port)

# check for discard
assert false (cookie:get-discard)
# check for secure
assert false (cookie:get-secure)
# check for http only
assert false (cookie:get-http-only)

# parse a cookie string header
const hval "name=value; version=1; path=\"/\"; domain=.afnix.org; port=8080"
cookie:parse hval

# check each argument
assert "name"       (cookie:get-name)
assert "value"      (cookie:get-value)
assert 1            (cookie:get-version)
assert "/"          (cookie:get-path)
assert ".afnix.org" (cookie:get-domain)
assert 8080         (cookie:get-port)

# add extra arguments and parse again
hval:+= "; Comment=\"afnix\"; CommentUrl=\"domain\"; Discard; Secure; HttpOnly"
cookie:parse hval

assert "afnix"      (cookie:get-comment)
assert "domain"     (cookie:get-comment-url)
assert true         (cookie:get-discard)
assert true         (cookie:get-secure)
assert true         (cookie:get-http-only)

# add an expiration time and parse again
hval:+= "; expires=Sun, 31 Dec 2000 23:59:00 GMT"
cookie:parse hval
assert 63145526340 (cookie:get-expire-time)

# complex combined test
const cval "PREF=ID=123456789:FF=0:TM=1234; path=/; domain=.afnix.org; Secure"
const cook (afnix:nwg:Cookie cval)
assert "PREF" (cook:get-name)
assert "ID=123456789:FF=0:TM=1234" (cook:get-value)
assert "/" (cook:get-path)
assert ".afnix.org" (cook:get-domain)
assert true (cook:get-secure)

# check cookie expiration
const c (afnix:nwg:Cookie "name" "value" 0)
trans cexp "name=value; Expires=Thu, 01 Jan 1970 00:00:00 GMT; Path=/"
assert (c:to-string) cexp

# set version 1
c:set-version 1
trans cexp "name=value; Version=1; Max-Age=0; Path=/"
assert (c:to-string) cexp

