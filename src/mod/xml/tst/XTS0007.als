# ---------------------------------------------------------------------------
# - XTS0007.als                                                             -
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

# @info   xml test suite activator
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# create a xml document
trans xdoc (afnix:xml:XmlDocument "XTS0007.xml")

# get the root node and try to copy
trans root (xdoc:get-root)
# get the last node
trans node (root:get-child (- (root:child-length) 1))
assert "plist" (node:get-name)
# get the first parameter node
trans pn (node:get-child 0)
assert "param-0" (pn:get-attribute-value "name")
assert "0" (pn:get-attribute-value "value")
# get the second parameter node
trans pn (node:get-child 1)
assert "param-1" (pn:get-attribute-value "name")
assert "&1" (pn:get-attribute-value "value")

