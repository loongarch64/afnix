# ---------------------------------------------------------------------------
# - AXI0074.als                                                             -
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

# @info   locale object test
# @author amaury darsch

# create a default locale
const ld (Locale)
assert true (locale-p ld)
assert "Locale" (ld:repr)

# check for name and info
ld:set-name "locale"
assert "locale" (ld:get-name)
ld:set-info "default"
assert "default" (ld:get-info)

# check reel
assert 1.0 (ld:to-real "1.0")
assert 1000.0 (ld:to-real "1 000.0")

# set the locale
ld:set-parameters "N=afnix&I=locale&D=,&S= "
assert "afnix"  (ld:get-name)
assert "locale" (ld:get-info)
assert ',' (ld:get-decimal-point)
assert ' ' (ld:get-decimal-separator)

assert 1.0 (ld:to-real "1,0")
assert 1000.0 (ld:to-real "1 000,0")
