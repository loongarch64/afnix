# ---------------------------------------------------------------------------
# - TXT0005.als                                                             -
# - afnix:txt module test unit                                              -
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

# @info   scanner test unit
# @author amaury darsch

# get the module
interp:library "afnix-txt"

# create the scanner regex element
const LONG (afnix:txt:Pattern "LONG" [$d+])
const REAL (afnix:txt:Pattern "REAL" [$d+'.'$d*])

# create a default scanner
const  scan (afnix:txt:Scanner)
assert true (afnix:txt:scanner-p scan)
assert 0    (scan:length)


# add the regex element to the scanner
scan:add LONG
assert 0 (LONG:get-tag)
scan:add REAL
assert 1 (REAL:get-tag)

# get the scanner length and check regex element
assert 2 (scan:length)

# check index 0
trans re (scan:get 0)
assert "LONG" (re:get-name)
# check index 1
trans re (scan:get 1)
assert "REAL" (re:get-name)
