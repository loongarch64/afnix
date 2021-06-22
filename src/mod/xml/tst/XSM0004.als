# ---------------------------------------------------------------------------
# - XSM0004.als                                                             -
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

# @info   xsm text node extraction test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# create a text node
trans  node (afnix:xml:XsmNode "afnix programming language")
assert true (afnix:xml:xsm-node-p node)

# get the text vector
trans  wrds    (node:get-words)
assert 3       (wrds:length)

# check the words
assert "afnix"       (wrds:get 0)
assert "programming" (wrds:get 1)
assert "language"    (wrds:get 2)

# create a text node
trans  node (afnix:xml:XsmNode "  'afnix")
assert true (afnix:xml:xsm-node-p node)

# get the text vector
trans  wrds    (node:get-words)
assert 1       (wrds:length)
assert "afnix" (wrds:get 0)

# create a text node
trans  node (afnix:xml:XsmNode "  !0 @1 #2 $3 %4 &5 *6 -7 +8 =9")
assert true (afnix:xml:xsm-node-p node)

# get the text vector
trans  wrds (node:get-words)
assert 10   (wrds:length)
assert "0"  (wrds:get 0)
assert "1"  (wrds:get 1)
assert "2"  (wrds:get 2)
assert "3"  (wrds:get 3)
assert "4"  (wrds:get 4)
assert "5"  (wrds:get 5)
assert "6"  (wrds:get 6)
assert "7"  (wrds:get 7)
assert "8"  (wrds:get 8)
assert "9"  (wrds:get 9)

# create a text node with special stuff
trans  node (afnix:xml:XsmNode "The&nbsp;Test &lt; &gt;")
assert true (afnix:xml:xsm-node-p node)

# get the text vector
trans  wrds (node:get-words)
#assert 4   (wrds:length)
#assert "The"  (wrds:get 0)
#assert "Test" (wrds:get 1)
#assert "<"    (wrds:get 2)
#assert ">"    (wrds:get 3)
