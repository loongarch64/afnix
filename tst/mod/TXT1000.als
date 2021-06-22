# ---------------------------------------------------------------------------
# - TXT1002.als                                                             -
# - afnix:txt module global test unit                                       -
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

# @info   input string global test unit
# @author amaury darsch

# get the module
interp:library "afnix-sio"

# create an input string stream
const is (afnix:sio:InputString)

# create a reader by stream
const rd (Reader is)

# set a string and parse
is:set "trans i 0"
trans form (rd:parse)

# evaluate and check
assert 0   (eval form)
assert 0   i
assert nil (rd:parse)

# check for more parsing
is:set "unref i"
trans form (rd:parse)

# eval again and check
eval form
assert false (eval-p i)
