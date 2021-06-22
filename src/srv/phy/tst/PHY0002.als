# ---------------------------------------------------------------------------
# - PHY0002.als                                                             -
# - afnix:phy service test unit                                             -
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

# @info   unit test unit
# @author amaury darsch

# get the module
interp:library "afnix-phy"

# check the nil unit
trans unit (afnix:phy:Unit)
assert true (afnix:phy:unit-p unit)
assert "Unit" (unit:repr)
assert ""     (unit:to-string)

# check base unit
trans unit (afnix:phy:Unit "s" "second")
assert "s" (unit:to-string)
assert "s" (unit:get-name)
assert "second" (unit:get-info)
assert 1.0 (unit:to-factor)

trans unit (afnix:phy:Unit "m" "meter")
assert "m" (unit:to-string)
assert "m" (unit:get-name)
assert "meter" (unit:get-info)
assert 1.0 (unit:to-factor)

trans unit (afnix:phy:Unit "kg" "kilogram")
assert "kg" (unit:to-string)
assert "kg" (unit:get-name)
assert "kilogram" (unit:get-info)
assert 1.0 (unit:to-factor)

trans unit (afnix:phy:Unit "A" "ampere")
assert "A" (unit:to-string)
assert "A" (unit:get-name)
assert "ampere" (unit:get-info)
assert 1.0 (unit:to-factor)

trans unit (afnix:phy:Unit "K" "kelvin")
assert "K" (unit:to-string)
assert "K" (unit:get-name)
assert "kelvin" (unit:get-info)
assert 1.0 (unit:to-factor)

trans unit (afnix:phy:Unit "mol" "mole")
assert "mol" (unit:to-string)
assert "mol" (unit:get-name)
assert "mole" (unit:get-info)
assert 1.0 (unit:to-factor)

trans unit (afnix:phy:Unit "cd" "candela")
assert "cd" (unit:to-string)
assert "cd" (unit:get-name)
assert "candela" (unit:get-info)
assert 1.0 (unit:to-factor)

# check derived unit
trans unit (afnix:phy:to-unit "rad")
assert "rad"              (unit:get-name)
assert "radian"           (unit:get-info)
assert "m.m^-1"           (unit:to-string)
assert 1.0 (unit:to-factor)

trans unit (afnix:phy:to-unit "sr")
assert "sr"               (unit:get-name)
assert "steradian"        (unit:get-info)
assert "m^2.m^-2"         (unit:to-string)
assert 1.0 (unit:to-factor)

trans unit (afnix:phy:to-unit "Hz")
assert "Hz"               (unit:get-name)
assert "hertz"            (unit:get-info)
assert "s^-1"             (unit:to-string)
assert 1.0 (unit:to-factor)

trans unit (afnix:phy:to-unit "N")
assert "N"                (unit:get-name)
assert "newton"           (unit:get-info)
assert "kg.m.s^-2"        (unit:to-string)
assert 1.0 (unit:to-factor)

trans unit (afnix:phy:to-unit "Pa")
assert "Pa"               (unit:get-name)
assert "pascal"           (unit:get-info)
assert "kg.m^-1.s^-2"     (unit:to-string)
assert 1.0 (unit:to-factor)

trans unit (afnix:phy:to-unit "J")
assert "J"                (unit:get-name)
assert "joule"            (unit:get-info)
assert "kg.m^2.s^-2"      (unit:to-string)
assert 1.0 (unit:to-factor)

trans unit (afnix:phy:to-unit "W")
assert "W"                (unit:get-name)
assert "watt"             (unit:get-info)
assert "kg.m^2.s^-3"      (unit:to-string)
assert 1.0 (unit:to-factor)

trans unit (afnix:phy:to-unit "C")
assert "C"                (unit:get-name)
assert "coulomb"          (unit:get-info)
assert "s.A"              (unit:to-string)
assert 1.0 (unit:to-factor)

trans unit (afnix:phy:to-unit "V")
assert "V"                (unit:get-name)
assert "volt"             (unit:get-info)
assert "kg.m^2.s^-3.A^-1" (unit:to-string)
assert 1.0 (unit:to-factor)

# check unit with factor
trans unit (afnix:phy:Unit "km/s" "speed" "km.s^-1")
assert "km/s"     (unit:get-name)
assert "speed"    (unit:get-info)
assert "km.s^-1"  (unit:to-string)
assert 1000.0     (unit:to-factor)

trans unit (afnix:phy:to-unit "km/h")
assert "km/h"  (unit:get-name)
assert (/ 1000.0 3600.0)  (unit:get-scaling-factor)
assert 1000.0 (unit:mul 3600.0)
trans sval    (unit:to-string)
trans unit    (afnix:phy:Unit (unit:get-name) (unit:get-info) sval)
assert sval   (unit:to-string)

trans unit  (afnix:phy:to-unit "au")
assert "au" (unit:get-name)
trans sval  (unit:to-string)
trans unit  (afnix:phy:Unit (unit:get-name) (unit:get-info) sval)
assert sval (unit:to-string)
