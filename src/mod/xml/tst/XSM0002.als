# ---------------------------------------------------------------------------
# - XSM0002.als                                                             -
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

# @info   xsm reader test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# create a reader
const  xsm  (afnix:xml:XsmReader)
assert true (afnix:xml:xsm-reader-p xsm)

# bind a single node stream
xsm:set-input-stream "afnix"
trans node     (xsm:get-node)
assert true    (node:text-p)
assert true    (node:textable-p)
assert "afnix" (node:to-string)

# bind a tag list
xsm:set-input-stream "<p>hello world</p>"

# read first node
trans node     (xsm:get-node)
assert true    (node:tag-p)
assert true    (node:normal-p)
assert "p"     (node:get-name)
assert "p"     (node:to-string)
assert "<p>"   (node:to-literal)

# read second node
trans node     (xsm:get-node)
assert true    (node:text-p)
assert true    (node:textable-p)
assert "hello world" (node:to-string)
assert "hello world" (node:to-literal)

# read third node
trans node     (xsm:get-node)
assert true    (node:end-p)
assert "p"     (node:get-name)
assert "p"     (node:to-string)
assert "</p>"  (node:to-literal)

# check for nil
assert nil (xsm:get-node)
