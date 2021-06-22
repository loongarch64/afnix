# ---------------------------------------------------------------------------
# - AXI0071.als                                                             -
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

# @info   lexer class test module
# @author amaury darsch

# create a lexer by boolean
trans l (Lexer)

# check representation
assert true (lexer-p l)
assert "Lexer" (l:repr)

# check boolean value
trans l (Lexer "true")
assert true (eval (l:get-object))
trans l (Lexer "false")
assert false (eval (l:get-object))

# check character value
trans l (Lexer "'c'")
assert 'c' (eval (l:get-object))

# check integer value
trans l (Lexer "2013")
assert 2013 (eval (l:get-object))

# check real value
trans l (Lexer "3.1415")
assert 3.1415 (eval (l:get-object))

# check string value
trans l (Lexer "\"afnix\"")
assert "afnix" (eval (l:get-object))

# check lexical value
trans l (Lexer "hello")
trans o (l:get-object)
assert true (lexical-p o)
