# ---------------------------------------------------------------------------
# - SEC0015.als                                                             -
# - afnix:sec module test unit                                              -
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

# @info   pbkdf2 test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# create a default key derivation
const pkdf (afnix:sec:Pbkdf2)
assert true (afnix:sec:pbkdf2-p pkdf)
# check representation
assert "Pbkdf2" (pkdf:repr)

# request a 32 bytes key
pkdf:set-key-size 32
# derive by string
trans kbuf (pkdf:derive "afnix")
trans kref "C1A76783CFBC93A2E3ABE36E4570EAA873C72742AC66243A81864A8AA2C8D8F6"
assert kref (kbuf:format)

