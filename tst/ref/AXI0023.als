# ---------------------------------------------------------------------------
# - AXI0023.als                                                              -
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

# @info   string builtin method test module
# @author amaury darsch

const s1  "hello"
const s2  (String "WORLD")
const s3  (String 1)
const s4  (String 'h')
const s5  (String 'U+3A3')

assert s1 "hello"
assert s2 "WORLD"
assert s3 "1"
assert s4 "h"

assert "HELLO" (s1:to-upper)    
assert "world" (s2:to-lower)
assert "helloWORLD" (+ s1 s2)

assert "00000hello" (s1:fill-left  '0' 10)
assert "hello-----" (s1:fill-right '-' 10)
assert "hel" (s1:sub-left  3)
assert "lo"  (s1:sub-right 3)

const  s6 " hello "
assert "hello " (s6:strip-left)
assert " hello" (s6:strip-right)
assert "hello"  (s6:strip)

const s7 "HeLlO"
assert true     (s1:strcic s7)
