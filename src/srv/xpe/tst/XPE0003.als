# ---------------------------------------------------------------------------
# - XPE0003.als                                                             -
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

# @info   xml include test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-xpe"

# create a xml include feature
const  xinc (afnix:xpe:XmlInclude)
assert true (afnix:xpe:xml-feature-p xinc)
assert true (afnix:xpe:xml-include-p xinc)

# create the base content
trans xcnt (afnix:xpe:XmlContent "XPE0001.xml")
# create a tree and save parameters
trans tree (afnix:xml:XneTree (xcnt:get-root))
trans siz1 (tree:size)

# create a xml content
trans xcnt (afnix:xpe:XmlContent "XPE0002.xml")
# get the root length
trans tree (afnix:xml:XneTree (xcnt:get-root))
trans siz2 (tree:size)

# process the content
trans xres (xinc:process xcnt)
# get the root node
trans tree (afnix:xml:XneTree (xres:get-root))
trans sizt (tree:size)

# check the total size
# tree 1 => remove root node
# tree 2 => remove root node + declaration + xi:include tag
# tree t => remove root node 
assert (- sizt 1) (+ (- siz2 3) (- siz1 1))
