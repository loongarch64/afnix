# ---------------------------------------------------------------------------
# - AXI0022.als                                                              -
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

# @info   character builtin method test module
# @author amaury darsch

const a   'a'
const h   'h'
const z   '0'
const b   ' '
const eol (Character '\n')
const eos (Character 4)
const nlc (Character 0)

assert 'h'  h
assert true (< 'a' 'z')
assert true (< 'A' 'Z')
assert true (< '0' '9')

assert true (b:blank-p)
assert true (z:digit-p)
assert true (h:alpha-p)
assert true (nlc:nil-p)
assert true (eol:eol-p)
assert true (eos:eos-p)

assert 0    (nlc:to-integer)

# byte object
const bo (Byte)
assert true   (byte-p bo)
assert "Byte" (bo:repr)

assert "0x00" (bo:to-hexa)
assert "00"   (bo:to-hexa-string)
