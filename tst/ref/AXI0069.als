# ---------------------------------------------------------------------------
# - AXI0069.als                                                              -
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

# @info   string vector test module
# @author amaury darsch

# create a default string vector
trans sv (Strvec)

assert true     (strvec-p  sv)
assert "Strvec" (sv:repr)
assert 0        (sv:length)

# add and test
sv:add "hello"
sv:add "world"
assert 2 (sv:length)

# check content
assert "hello" (sv:get 0)
assert "world" (sv:get 1)
assert 0       (sv:find "hello")
assert -1      (sv:find "terre")
assert 1       (sv:lookup "world")

# check concat
assert "helloworld"  (sv:concat)
assert "hello world" (sv:concat ' ')

# check length
assert 5 (sv:min-length)
assert 5 (sv:max-length)

# add pop and check
sv:add "terra firma"
assert 11 (sv:max-length)
assert "terra firma" (sv:pop-last)
assert "hello" (sv:pop)
assert "world" (sv:pop)
assert 0 (sv:length)
