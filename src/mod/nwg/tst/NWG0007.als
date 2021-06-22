# ---------------------------------------------------------------------------
# - NWG0007.als                                                             -
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

# @info   uri path test unit
# @author amaury darsch

# get the module
interp:library "afnix-nwg"

# create a uri path
trans puri  (afnix:nwg:UriPath "/usr/bin" "index.xht")
assert true (afnix:nwg:uri-path-p puri)
assert "UriPath" (puri:repr)

# validate accessors
assert "/usr/bin"  (puri:get-root)
assert "index.xht" (puri:get-index)

# validate uri request
assert "/usr/bin/index.xht" (puri:map-request-uri "/")
assert "/usr/bin/index.xht" (puri:map-request-uri "/index.xht")
assert "/usr/bin/index.xht" (puri:map-request-uri "/.././../")
assert "/usr/bin/index.xht" (puri:map-request-uri "index.xht")

# validate combine path
assert "/usr/bin/xht/index.xht" (puri:map-request-uri "/xht/./index.xht")
assert "/usr/bin/xht/index.xht" (puri:map-request-uri "/xht/../xht/index.xht")
assert "/usr/bin/xht/index.xht" (puri:map-request-uri "/./../xht/index.xht")

# validate request uri
assert "/usr/bin/index.xht" (puri:map-request-uri "http://www.afnix.org")
assert "/usr/bin/index.xht" (puri:map-request-uri "http://www.afnix.org/")

# create a uri path by uri
trans puri (afnix:nwg:UriPath "file:/usr/bin/index.xht")
# validate accessors
assert "/usr/bin"  (puri:get-root)
assert "index.xht" (puri:get-index)
