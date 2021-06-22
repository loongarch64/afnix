# ---------------------------------------------------------------------------
# - XTXT001.als                                                             -
# - afnix:txt module exemple - afnix language scanner                       -
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

#get the modules
interp:library "afnix-txt"
interp:library "afnix-sio"

# create the scanner regex element
const REAL      (afnix:txt:Pattern "REAL"      [[$d+.$d*]|[$d*.$d+]])
const REGEX     (afnix:txt:Pattern "REGEX"     "[" "]" true)
const STRING    (afnix:txt:Pattern "STRING"    "\"" '\\')
const CHARACTER (afnix:txt:Pattern "CHARACTER" "'" '\\')
const COMMENT   (afnix:txt:Pattern "COMMENT"   ["#"$E*$e])
const SPECIAL   (afnix:txt:Pattern "SPECIAL"   [<"("")""{""}">])
const INTEGER   (afnix:txt:Pattern "INTEGER"   [[$d+]|[[0x<$x_>+]|[0b<01_>+]]])
const LEXICAL   (afnix:txt:Pattern "LEXICAL"   [$w+])
const QUALIFIED (afnix:txt:Pattern "QUALIFIED" [$w+[:$w+]+])
const SEPARATOR (afnix:txt:Pattern "SEPARATOR" [<$b$n>+])

# create a default scanner and add the regex elements
const scanner (afnix:txt:Scanner)
scanner:add SEPARATOR COMMENT SPECIAL INTEGER REAL REGEX STRING CHARACTER
scanner:add LEXICAL QUALIFIED

# create an input stream
const is (afnix:sio:InputFile "XTXT001.als")
# loop until we have an eos or a nilp lexeme
while (trans valid (is:valid-p)) {
  trans lexm (scanner:scan is)
  if (object-p lexm) (println (lexm:get-value))
  valid:= (and (is:valid-p) (not (nil-p lexm)))
}
