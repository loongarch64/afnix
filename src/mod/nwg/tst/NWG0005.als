# ---------------------------------------------------------------------------
# - NWG0005.als                                                             -
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

# @info   http request test unit
# @author amaury darsch

# get the module
interp:library "afnix-nwg"

# create a request
trans  hrq  (afnix:nwg:HttpRequest)
assert true (afnix:nwg:http-proto-p   hrq)
assert true (afnix:nwg:http-request-p hrq)

# check the default http request
assert "GET"      (hrq:get-method)
assert "/"        (hrq:get-uri)
assert "HTTP/1.1" (hrq:get-version)
assert ""         (hrq:get-query)

# test with a uri
trans uri   (afnix:nwg:Uri "http://www.afnix.org")
trans hrq   (afnix:nwg:HttpRequest "HEAD" uri)

assert "HEAD"     (hrq:get-method)
assert "/"        (hrq:get-uri)
assert "HTTP/1.1" (hrq:get-version)

# check the header
assert 2 (hrq:header-length)
assert "www.afnix.org" (hrq:header-map "Host")
assert "close"         (hrq:header-map "Connection") 
