# ---------------------------------------------------------------------------
# - SEC1000.als                                                             -
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

# @info   aes global test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"
interp:library "afnix-sio"

# create a symmetric key
const key (afnix:sec:Key afnix:sec:Key:KSYM)

# create an aes cipher
const aes    (afnix:sec:Aes key)
assert "AES" (aes:get-name)

# create an input string buffer
trans ts "afnix security engine"
trans is (afnix:sio:InputString ts)

# create an input/output stream
trans io (afnix:sio:InputOutput)

# stream the buffer
while (is:valid-p) (aes:stream io is)

# reverse the cipher
aes:set-reverse true

# create an output string
trans os (afnix:sio:OutputString)

# stream the buffer
while (io:valid-p) (aes:stream os io)
assert ts (os:to-string)
