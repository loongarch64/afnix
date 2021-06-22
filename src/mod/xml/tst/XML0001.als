# ---------------------------------------------------------------------------
# - XML0001.als                                                             -
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

# @info   tag node test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# create a tag node
const  node (afnix:xml:XmlTag "afnix")
assert true (afnix:xml:node-p node)
assert true (afnix:xml:tag-p  node)

# check the name
assert "afnix" (node:get-name)

# set and check the name
const name "hello"
node:set-name name
assert name (node:get-name)

# check the attributes
assert 0 (node:attribute-length)

# add a new attribute
node:add-attribute (Property "afnix" "programming language")
assert 1    (node:attribute-length)
assert true (node:attribute-p "afnix")

# set a new attribute
node:add-attribute (Property "hello" "world")
assert 2 (node:attribute-length)

# check attribute as a property
trans attr (node:get-attribute 1)
assert true    (node:attribute-p "hello" "world")
assert "hello" (attr:get-name)
assert "world" (attr:get-value)


# set a new attribute
node:add-attribute (Property "xmlns" "http://www.w3.org/1999/xhtml")
assert 3 (node:attribute-length)

# check attribute as a property
trans attr (node:get-attribute 2)
assert "xmlns" (attr:get-name)
assert "http://www.w3.org/1999/xhtml" (attr:get-value)

# clone the node and check
const  ncln (node:clone)
assert name (ncln:get-name)
assert 3    (ncln:attribute-length)

# check attribute as a property
trans attr  (ncln:get-attribute 2)
assert "xmlns" (attr:get-name)
assert "http://www.w3.org/1999/xhtml" (attr:get-value)

# copy the node and check
const  ncpy (node:copy)
assert name (ncpy:get-name)
assert 3    (ncpy:attribute-length)

# check attribute as a property
trans attr  (ncpy:get-attribute 2)
assert "xmlns" (attr:get-name)
assert "http://www.w3.org/1999/xhtml" (attr:get-value)
