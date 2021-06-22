# ---------------------------------------------------------------------------
# - AXI0056.als                                                              -
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

# @info   logger class test module
# @author amaury darsch

# create a default logger
const  log  (Logger)
assert true (logger-p log)

# add a message in the logger
const msg "hello logger"
log:add Logger:INFO msg
assert  1    (log:length)
assert  msg  (log:get-message 0)
