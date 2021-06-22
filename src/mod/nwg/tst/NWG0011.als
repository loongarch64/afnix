# ---------------------------------------------------------------------------
# - NWG0011.als                                                             -
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

# @info   hyperlink test unit
# @author amaury darsch

# get the module
interp:library "afnix-nwg"

# nil hyperlink
trans hlnk (afnix:nwg:Hyperlink)

assert true (afnix:nwg:hyperlink-p hlnk)
assert "Hyperlink" (hlnk:repr)

# check uri
hlnk:set-uri interp:afnix-uri
assert interp:afnix-uri (hlnk:get-uri)
