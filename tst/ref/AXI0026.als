# ---------------------------------------------------------------------------
# - AXI0026.als                                                              -
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

# @info   gcd computation with euclid algorithm
# @author amaury darsch (for the afnix version)

const gcd (u v) {
  while (!= v 0) {
    trans r (u:mod v)
    u:= v
    v:= r
  }
  eval u
}

assert 34 (gcd 40902 24140)
