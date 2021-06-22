# ---------------------------------------------------------------------------
# - XTS0001.als                                                             -
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

# create a xsm document and check
const  xsmd (afnix:xml:XsmDocument "XTS0001.xml")
assert true (afnix:xml:xsm-document-p xsmd)
assert true (> (xsmd:length) 0)

# get the words and check
const  wrds (xsmd:get-words)
assert 0    (wrds:length)

# create a xml document
trans xdoc (afnix:xml:XmlDocument "XTS0001.xml")

# get the root node and try to copy
trans root (xdoc:get-root)
const node (root:copy)
assert (root:child-length) (node:child-length)
node:release

# create a xml document
xdoc:set-text "XTS0001.xml"

# get the root node and check length
trans  root (xdoc:get-root)
assert 2    (root:child-length)
