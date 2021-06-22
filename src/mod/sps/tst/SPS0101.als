# ---------------------------------------------------------------------------
# - SPS0101.als                                                             -
# - afnix:sps module test unit                                              -
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

# @info   transit test unit
# @author amaury darsch

# get the module
interp:library "afnix-sps"

# create a new sps transit
const  st  (afnix:sps:SpsTransit)
assert true (afnix:sps:transit-p st)

# import a csv into a sheet
st:import "SPS010X.csv"
const sht (st:get-sheet)

# check sheet data
assert true (afnix:sps:sheet-p sht)
assert 10    (sht:length)

assert "Boolean" (sht:map 0 0)
assert "Integer" (sht:map 0 1)
assert "Real"    (sht:map 0 2)
assert "String"  (sht:map 0 3)

assert false (sht:map 1 0)
assert 0     (sht:map 1 1)
assert 0.0   (sht:map 1 2)
assert "0"   (sht:map 1 3)

assert true  (sht:map 2 0)
assert 1     (sht:map 2 1)
assert 1.0   (sht:map 2 2)
assert "1"   (sht:map 2 3)

assert "Integer" (sht:map 3 0)
assert "Integer" (sht:map 3 1)
assert "Real"    (sht:map 3 2)
assert "Real"    (sht:map 3 3)

assert 0     (sht:map 4 0)
assert 0     (sht:map 4 1)
assert 0.0   (sht:map 4 2)
assert 0.0   (sht:map 4 3)

assert  3    (sht:map 5 0)
assert -3    (sht:map 5 1)
assert  3.14 (sht:map 5 2)
assert -3.14 (sht:map 5 3)

assert "Boolean"   (sht:map 6 0)
assert "Character" (sht:map 6 1)
assert "Relatif"   (sht:map 6 2)
assert "Regex"     (sht:map 6 3)

assert  false (sht:map 7 0)
assert 'f'    (sht:map 7 1)
assert  0R    (sht:map 7 2)

assert "Date" (sht:map 8 0)
assert "Time" (sht:map 8 1)

assert 20000101 (sht:map 9 0)
assert "00:00:00" (sht:map 9 1)
