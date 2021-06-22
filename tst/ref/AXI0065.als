# ---------------------------------------------------------------------------
# - AXI0065.als                                                              -
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

# @info   counter test module
# @author amaury darsch

# create a counter
trans cntr  (Counter 10)

# check the counter 
assert "Counter" (cntr:repr)
assert true (counter-p cntr)

# check the count
trans step 0
loop (cntr:step-p) (step:++)
assert 10 step

# reset the counter and step
trans step 0
cntr:reset
# loop again
loop (cntr:step-p) (assert (cntr:get) (step:++))
