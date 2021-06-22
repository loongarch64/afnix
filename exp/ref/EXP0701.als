# ----------------------------------------------------------------------------
# - EXP0701.als                                                              -
# - afnix example : chapter 7 : example 01                                   -
# ----------------------------------------------------------------------------
# - This program is  free software;  you can  redistribute it and/or  modify -
# - it provided that this copyright notice is kept intact.                   -
# -                                                                          -
# - This  program  is  distributed in the hope  that it  will be useful, but -
# - without  any   warranty;  without  even   the   implied    warranty   of -
# - merchantability  or fitness for a particular purpose. In not event shall -
# - the copyright holder be  liable for  any direct, indirect, incidental or -
# - special damages arising in any way out of the use of this software.      -
# ----------------------------------------------------------------------------
# - copyright (c) 1999-2021 amaury darsch                                    -
# ----------------------------------------------------------------------------

# the lexeme separators
const  SEPS [<$b$n>+]
assert true (== SEPS " ")

# the afnix integer number
const  INTEGER [[$d+]|[[0x<$x_>+]|[0b<01_>+]]]
assert true (== INTEGER "0")
assert true (== INTEGER "2000")
assert true (== INTEGER "0x2000")
assert true (== INTEGER "0xff_00")
assert true (== INTEGER "0b1111")
assert true (== INTEGER "0b11_00")

# the afnix real number
const  REAL [[$d+.$d*]|[$d*.$d+]]
assert true (== REAL "0.")
assert true (== REAL ".0")
assert true (== REAL "0.0")

# the afnix string
const  STRING ["\""$N*"\""]
assert true (== STRING "\"hello world\"")

# the afnix regex
const  REGEX ["["$N*"]"]
assert true (== REGEX "[$d+]")

# the afnix lexical name
const  LEXICAL [$w+]
assert true (== LEXICAL "true")
assert true (== LEXICAL "token-p")

# the afnix qualified name
const  QUALIFIED [$w+[:$w+]+]
assert true (== QUALIFIED "afnix:txt")
assert true (== QUALIFIED "afnix:txt:Scanner")

# the afnix comment - use eol or eos at the end
const  COMMENT ["#"$E*$e]
assert true (== COMMENT "# afnix comment")

# the afnix special characters
const  SPECIAL [<"("")""{""}">]
assert true (== SPECIAL "(")
