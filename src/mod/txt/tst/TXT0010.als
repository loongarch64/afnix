# ---------------------------------------------------------------------------
# - TXT0010.als                                                             -
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

# @info   text utilities test
# @author amaury darsch

# get the module
interp:library "afnix-txt"

# create a test string
trans  src "hello"
trans  dst "hello"
assert dst (afnix:txt:indent src "{" "}" " ")

trans  src "hello {\nworld\n}"
trans  dst "hello {\n world\n}"
assert dst (afnix:txt:indent src "{" "}" " ")

trans  src "hello {\n{world}\n}"
trans  dst "hello {\n {world}\n}"
assert dst (afnix:txt:indent src "{" "}" " ")
