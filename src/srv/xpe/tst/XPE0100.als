# ---------------------------------------------------------------------------
# - XPE0100.als                                                             -
# - afnix:xpe service test unit                                             -
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

# @info   xml plist test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-xpe"

# create a xml plist
const  xlst (afnix:xpe:XmlPlist)
assert true (afnix:xpe:xml-plist-p xlst)

# set the plist name and check
xlst:set-name "xlst"
assert "xlst" (xlst:get-name)

# check the plist tag name
xlst:set-plist-tag-name "xplist"
assert "xplist" (xlst:get-plist-tag-name)

# check the property tag name
xlst:set-property-tag-name "xprop"
assert "xprop" (xlst:get-property-tag-name)

# check the property name attribute
xlst:set-name-attribute "xname"
assert "xname" (xlst:get-name-attribute)

# check the property value attribute
xlst:set-value-attribute "xvalue"
assert "xvalue" (xlst:get-value-attribute)

# create a property list
const plst (Plist)
plst:add "hello" " world"

# set the property list and get the root node
xlst:set-plist plst
const root (xlst:get-root)

# check the root content (decl+plist)
assert 2 (root:child-length)
