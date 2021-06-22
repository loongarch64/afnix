# ----------------------------------------------------------------------------
# - std-adp-xmlnp                                                            -
# - afnix:adp xml node processing module                                     -
# ----------------------------------------------------------------------------
# - This program is  free software;  you can  redistribute it and/or  modify -
# - it provided that this copyright notice is kept intact.                   -
# -                                                                          -
# - This  program  is  distributed in the hope  that it  will be useful, but -
# - without  any   warranty;  without  even   the   implied    warranty   of -
# - merchantability  or fitness for a particular purpose. In not event shall -
# - the copyright holder be  liable for  any direct, indirect, incidental or -
# - special damages arising in any way out of the use of this software.      -
# ----------------------------------------------------------------------------
# - copyright (c) 1999-2021 amaury darsch                                    -
# ----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# - global section                                                           -
# ----------------------------------------------------------------------------

# define the node processing class
const afnix:adp:xmlnp (class)

# ----------------------------------------------------------------------------
# - public section                                                           -
# ----------------------------------------------------------------------------

# get a vector of child nodes by name
# @param node the node to process
# @param name the child tag name to select
const afnix:adp:xmlnp:get-child-vector (node name) {
  # create a xne condition
  const xcnd (afnix:xml:XneCond)
  # add a condition name
  xcnd:add afnix:xml:Xne:NAME name
  # create a tree with the node
  const tree (afnix:xml:XneTree node)
  # select the node by condition
  tree:select xcnd
}

# get the maximum number of child node from a vector of nodes
# @param nvec the vector of nodes
const afnix:adp:xmlnp:max-child-length (nvec) {
  # initialize result
  trans result 0
  # loop in the node vector
  for (n) (nvec) {
    # get the number of children
    trans clen (n:child-length)
    if (> clen result) (result:= clen)
  }
  # here it is
  eval result
}

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the context environment
trans afnix:adp:xmlnp:preset nil {
  # create the tag dispatch table
  const this:xtbl (HashTable)
}

# ----------------------------------------------------------------------------
# - method section                                                           -
# ----------------------------------------------------------------------------

# add a tag dispatch by name and object
# @param name the tag name
# @param xobj the object to bind
trans afnix:adp:xmlnp:add-dispatch-tag (name xobj) {
  this:xtbl:add name xobj
}

# dispatch a tag to the appropriate method
# @param node the node to dispatch
trans afnix:adp:xmlnp:dispatch-tag (node) {
  # do nothing for non tag node
  if (not (afnix:xml:tag-p node)) (return)
  # get the tag name and dispatch
  const name (node:get-name)
  # look into the table
  if (this:xtbl:exists-p name) {
    # get the method
    trans func (this:xtbl:get name)
    # dispatch with the node
    func node
  } (errorln "[xmlnp] no handler for tag " name)
}

# dispatch a node to the appropriate method
# @param node the node to dispatch
trans afnix:adp:xmlnp:dispatch-node (node) {
  # check for a declaration node
  if (afnix:xml:decl-p node) (return)
  # check for a tag - all other nodes are ignored
  if (afnix:xml:tag-p node) (return (this:dispatch-tag node))
}

# dispatch all child nodes
# @param node the node to dispatch
trans afnix:adp:xmlnp:expands-cnod (node) {
  # get all children and loop
  const clen (node:child-length)
  loop (trans i 0) (< i clen) (i:++) {
    trans cnod (node:get-child i)
    this:dispatch-node cnod
  }
}

# # dispatch a root node to the appropriate method
# @param node the node to dispatch
trans afnix:adp:xmlnp:expands-root (node) {
  # use regular node dispatch for non root node
  if (not (afnix:xml:root-p node)) (return (this:dispatch-node node))
  # dispatch all child nodes
  this:expands-cnod node
}
