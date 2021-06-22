# ---------------------------------------------------------------------------
# - TXT0004.als                                                             -
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

# @info   lexeme test unit
# @author amaury darsch

# get the module
interp:library "afnix-txt"

# create a default regex element
const  lexm (afnix:txt:Lexeme)
assert true (afnix:txt:lexeme-p  lexm)
assert true (afnix:txt:literal-p lexm)

# check for the value
lexm:set-value "hello"
assert "hello" (lexm:get-value)
assert "hello" (lexm:to-string)
assert "hello" (lexm:to-literal)

# check for the tag
lexm:set-tag 2000
assert 2000 (lexm:get-tag)

# check for the source
lexm:set-source "world"
assert "world" (lexm:get-source)

# check for the source index
lexm:set-index 123
assert 123 (lexm:get-index)
