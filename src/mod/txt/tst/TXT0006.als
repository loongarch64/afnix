# ---------------------------------------------------------------------------
# - TXT0006.als                                                             -
# - afnix:txt module test unit                                           -
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

# @info   transliteration test unit
# @author amaury darsch

# get the module
interp:library "afnix-txt"

# create a literate element
const  tl   (afnix:txt:Literate)
assert true (afnix:txt:literate-p tl)

# check for default translation
assert "hello" (tl:translate "hello")

# set a strange mapping
tl:set-map-character 'h' 'w'
tl:set-map-character 'e' 'o'
tl:set-map-character 'l' 'r'
tl:set-map-character 'o' 'd'

# check for translation
assert "worrd" (tl:translate "hello")

# check the mapping
assert (tl:get-map-character 'h') 'w'
assert (tl:get-map-character 'e') 'o'
assert (tl:get-map-character 'l') 'r'
assert (tl:get-map-character 'o') 'd'


# reset the mapping and check again
tl:reset
assert (tl:get-map-character 'h') 'h'
assert (tl:get-map-character 'e') 'e'
assert (tl:get-map-character 'l') 'l'
assert (tl:get-map-character 'o') 'o'

# set the escape character
tl:set-esc-character '\\'
assert (tl:get-esc-character) '\\'

# set the escape map for tabulation
tl:set-emc-character 't' '\t'
assert (tl:get-emc-character 't') '\t'

# check for transliteration
assert (tl:translate "hello \\t world") "hello \t world"
