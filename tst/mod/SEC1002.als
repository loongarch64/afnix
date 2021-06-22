# ---------------------------------------------------------------------------
# - SEC1002.als                                                             -
# - afnix:sec module global test unit                                       -
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

# @info   rsa global test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"
interp:library "afnix-sio"

# create a rsa key
const key (afnix:sec:Key afnix:sec:Key:KRSA 256)

# create a rsa cipher
const   rsa  (afnix:sec:Rsa key)
assert "RSA" (rsa:get-name)

# create an input string buffer
trans ts "afnix security engine"
trans is (afnix:sio:InputString ts)

# create an input/output stream
trans io (afnix:sio:InputOutput)

# stream the buffer
trans  sl (rsa:stream io is)
assert sl (ts:length)

# reverse the cipher
rsa:set-reverse true

# create an output string
trans os (afnix:sio:OutputString)

# stream the buffer
trans  sl (rsa:stream os io)
assert ts (os:to-string)
