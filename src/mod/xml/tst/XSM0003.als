# ---------------------------------------------------------------------------
# - XSM0003.als                                                             -
# - afnix:xml module test unit                                              -
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

# @info   xso info test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-sio"

# create a info object
const info (afnix:xml:XsoInfo "hello" "world")
assert true (afnix:xml:xso-info-p info)

# check the values
assert "hello" (info:get-name)
assert "world" (info:get-text)

# change the name and value and check
info:set-name "afnix"
assert "afnix" (info:get-name)
info:set-text "xso"
assert "xso"   (info:get-text)

# set an atribute and check
info:set-attribute "id" "id1"
assert "id1" (info:get-attribute-value "id")
