# ---------------------------------------------------------------------------
# - AXI0002.als                                                              -
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

# @info   symbol definition test module
# @author amaury darsch

const  ival 2000
assert ival 2000
const  fval 2000.0
assert fval 2000.0

trans  bval true
assert bval true
trans  bval false
assert bval false

# unreference and test
unref  ival
unref  fval

# test again
const  ival 1000
assert ival 1000
const  fval 1000.0
assert fval 1000.0
