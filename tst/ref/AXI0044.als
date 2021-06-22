# ---------------------------------------------------------------------------
# - AXI0044.als                                                              -
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

# @info   nameset test module
# @author amaury darsch

# create a top level nameset
const nset  (nameset)
assert true (nameset-p  nset)

# bind some symbols and test
const  nset:ival 2000
assert nset:ival 2000
const  nset:fval 2000.0
assert nset:fval 2000.0

trans  nset:bval true
assert nset:bval true
trans  nset:bval false
assert nset:bval false

# unreference and test
unref  nset:ival
unref  nset:fval

# test again
const  nset:ival 1000
assert nset:ival 1000
const  nset:fval 1000.0
assert nset:fval 1000.0
