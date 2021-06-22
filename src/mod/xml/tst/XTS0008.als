# ---------------------------------------------------------------------------
# - XTS0008.als                                                             -
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

# @info   xml test suite
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# create a xml document
trans xdoc (afnix:xml:XmlDocument "XTS0008.xml")
# get the root node and try to copy
trans root (xdoc:get-root)
# get the last node
trans node (root:get-child (- (root:child-length) 1))
assert "p" (node:get-name)

# make sure we have one node
assert 1 (node:child-length)
# get the first text node
trans cn (node:get-child 0)
assert true (afnix:xml:text-p cn)


