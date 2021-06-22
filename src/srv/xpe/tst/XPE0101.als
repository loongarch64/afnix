# ---------------------------------------------------------------------------
# - XPE0101.als                                                             -
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

# @info   xml print table test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-xpe"

# create a xml plist
const  xtbl (afnix:xpe:XmlPrintTable)
assert true (afnix:xpe:xml-print-table-p xtbl)

# set the name and check
xtbl:set-name "xtbl"
assert "xtbl" (xtbl:get-name)

# check the table tag name
xtbl:set-table-tag-name "xtable"
assert "xtable" (xtbl:get-table-tag-name)

# check the head tag name
xtbl:set-head-tag-name "xhead"
assert "xhead" (xtbl:get-head-tag-name)

# check the body tag name
xtbl:set-body-tag-name "xbody"
assert "xbody" (xtbl:get-body-tag-name)

# check the row tag name
xtbl:set-row-tag-name "xtr"
assert "xtr" (xtbl:get-row-tag-name)

# check the data tag name
xtbl:set-data-tag-name "xtd"
assert "xtd" (xtbl:get-data-tag-name)

# create a print table
const ptbl (PrintTable 2)
ptbl:add-head "Title" "Value"
ptbl:add      "hello" " world"
ptbl:add      "Hello" " Planet"

# set the print table and get the root node
xtbl:set-print-table ptbl
const root (xtbl:get-root)

# check the root content (decl+plist)
assert 2 (root:child-length)

