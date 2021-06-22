# ----------------------------------------------------------------------------
# - EXP1001.als                                                              -
# - afnix example : chapter 10 : example 01                                  -
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

# create a new print table with 3 columns
const tbl (PrintTable 3)

# add the rows
tbl:add "Planet"  "Diameter" "Rotation time"
tbl:add "Mercury"   4840     "1407:36"
tbl:add "Venus"    12400     "5819:51"
tbl:add "Earth"    12756     "23:56"
tbl:add "Mars"      6800     "24:37"
tbl:add "Jupiter" 142800     "9:50"
tbl:add "Saturn"  120800     "10:14"
tbl:add "Uranus"   47600     "10:49"
tbl:add "Neptune"  44600     "15:40"
tbl:add "Pluto"     5850     "153:17"
tbl:add "Sedna"     1180     "10:00"

# set the table style
trans stl (tbl:get-style 0)
stl:set-text-size 10
tbl:set-style 0 stl

trans stl (tbl:get-style 1)
stl:set-text-size 10
tbl:set-style 1 stl

trans stl (tbl:get-style 2)
stl:set-text-alignment Style:ALIGN-RIGHT
tbl:set-style 2 stl

# print the table
tbl:format
