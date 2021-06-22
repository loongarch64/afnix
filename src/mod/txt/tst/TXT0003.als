# ---------------------------------------------------------------------------
# - TXT0003.als                                                             -
# - afnix:txt module test unit                                              -
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

# @info   regex element test unit
# @author amaury darsch

# get the module
interp:library "afnix-txt"

# create a default regex element
const  r1   (afnix:txt:Pattern)
assert true (afnix:txt:pattern-p r1)

# assign a regex to this one
r1:set-regex [hello]
assert true    (r1:check "hello")
assert "hello" (r1:match "hello")

# create another regex element by string
const r2       (afnix:txt:Pattern "$d+")
assert true    (r2:check "123")
assert "123"   (r2:match "123")

# set the regex element name
r2:set-name "INTEGER"
assert "INTEGER" (r2:get-name)

# create a regex element by name and string value
const r3 (afnix:txt:Pattern "INTEGER" "$d+")
assert true      (r3:check "123")
assert "123"     (r3:match "123")
assert "INTEGER" (r3:get-name)

# create a regex element by name and regex
const r4 (afnix:txt:Pattern "INTEGER" [$d+])
assert true      (r4:check "123")
assert "INTEGER" (r4:get-name)

# create a balanced pattern
const r5           (afnix:txt:Pattern "STRING" "\"" "\"")
assert true        (r5:check "\"hello\"")
assert "\"hello\"" (r5:match "\"hello\"")

# create a balanced pattern with an escape sequence
const r6           (afnix:txt:Pattern "STRING" "\"" '\\')
assert true        (r6:check "\"hello\"")
assert "\"hello\"" (r6:match "\"hello\"")

# create a recursive balanced pattern
const r7         (afnix:txt:Pattern "STRING" "<" ">" true)
assert true      (r7:check "<hello <world> afnix>")
assert "<hello>" (r7:match "<hello>")
