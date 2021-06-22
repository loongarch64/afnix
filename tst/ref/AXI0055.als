# ---------------------------------------------------------------------------
# - AXI0055.als                                                              -
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

# @info   qualified reference count torture
# @author amaury darsch

# create the first instance
const i (Instance)
trans i:preset nil {
  const this:x (c:new)
}

# create the class generator
const c (class)
const c:new nil {
  const result (Instance)
  eval result
}

# run the test
i:preset
