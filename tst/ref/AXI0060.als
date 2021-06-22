# ---------------------------------------------------------------------------
# - AXI0060.als                                                              -
# - afnix engine test module                                                -
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

# @info   plist class test module
# @author amaury darsch

# create a plist
const  plst (Plist)
assert true (plist-p plst)
assert 0    (plst:length)

# set the name and info
const name "afnix"
const pval "programming language"
plst:set-name name
plst:set-info pval

# check name and info
assert name (plst:get-name)
assert pval (plst:get-info)

# add a new property
plst:add name pval

# check length and property
assert 1    (plst:length)
assert true (plst:exists-p name)

# get property and check
const  prop (plst:lookup name)
assert name (prop:get-name)
assert pval (prop:get-value)

# check value directly
assert pval (plst:get-value name)
