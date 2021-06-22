# ---------------------------------------------------------------------------
# - AXI0045.als                                                              -
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

# @info   lexical and qualified methods test
# @author amaury darsch

# check the lexical
trans lex      0
const sym-lex  (protect lex)
assert true    (lexical-p sym-lex)
assert "lex"   (sym-lex:to-string)
assert "lex"   (sym-lex:to-literal)

# check a qualified name
const  sym-qal (protect q:a:l)
assert true    (qualified-p sym-qal)
assert "q:a:l" (sym-qal:to-string)
assert "q:a:l" (sym-qal:to-literal)

# check for a symbol
const  sym     (sym-lex:map)
assert true    (symbol-p sym)
assert false   (sym:get-const)
assert "lex"   (sym:to-string)
assert "lex"   (sym:to-literal)
assert 0       (sym:get-object)

# symbol modifiers
sym:set-object true
assert true    (sym:get-object)
assert true    lex
