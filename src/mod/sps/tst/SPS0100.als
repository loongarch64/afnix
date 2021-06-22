# ---------------------------------------------------------------------------
# - SPS0100.als                                                             -
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

# @info   csv reader test unit
# @author amaury darsch

# get the module
interp:library "afnix-sps"

# create a new csv reader
const  csv  (afnix:sps:Csv "SPS010X.csv")
assert true (afnix:sps:csv-p csv)

# parse line 1
trans form (csv:parse)
assert "Boolean" (form:get-car)
assert "Integer" (form:get-cadr)
assert "Real"    (form:get-caddr)
assert "String"  (form:get-cadddr)
# parse line 2
trans form   (csv:parse)
assert false (eval (form:get-car))
assert  0    (eval (form:get-cadr))
assert  0.0  (eval (form:get-caddr))
assert  "0"  (eval (form:get-cadddr))
# parse line 3
trans  form  (csv:parse)
assert true  (eval (form:get-car))
assert  1    (eval (form:get-cadr))
assert  1.0  (eval (form:get-caddr))
assert  "1"  (eval (form:get-cadddr))
# parse line 4
trans form (csv:parse)
assert "Integer" (form:get-car)
assert "Integer" (form:get-cadr)
assert "Real"    (form:get-caddr)
assert "Real"    (form:get-cadddr)
# parse line 5
trans form   (csv:parse)
assert 0     (eval (form:get-car))
assert 0     (eval (form:get-cadr))
assert 0.0   (eval (form:get-caddr))
assert 0.0   (eval (form:get-cadddr))
# parse line 6
trans form    (csv:parse)
assert  3     (eval (form:get-car))
assert -3     (eval (form:get-cadr))
assert  3.14  (eval (form:get-caddr))
assert -3.14  (eval (form:get-cadddr))
# parse line 7
trans form (csv:parse)
assert "Boolean"   (form:get-car)
assert "Character" (form:get-cadr)
assert "Relatif"   (form:get-caddr)
assert "Regex"     (form:get-cadddr)
# parse line 8
trans form   (csv:parse)
assert false (eval (form:get-car))
assert 'f'   (eval (form:get-cadr))
assert 0R    (eval (form:get-caddr))
