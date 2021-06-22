# ---------------------------------------------------------------------------
# - AXI0078.als                                                              -
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

# @info   relatif extra testing [2019 universal question discovery]
# @author amaury darsch

# base diaphantine numbers: x^3 + y^3 + z^3 = 42
trans x -80538738812075974R
trans y  80435758145817515R
trans z  12602123297335631R

# exponent checks
assert (x:pow 3) -522413599036979150280966144853653247149764362110424R
assert (y:pow 3)  520412211582497361738652718463552780369306583065875R
assert (z:pow 3)  2001387454481788542313426390100466780457779044591R

# final magic check
assert 42R (+ (+ (x:pow 3) (y:pow 3)) (z:pow 3))
