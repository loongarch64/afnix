# ---------------------------------------------------------------------------
# - SYS0007.als                                                             -
# - afnix:sys module test unit                                              -
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

# @info   date class test unit
# @author amaury darsch

# get the module
interp:library "afnix-sys"

# simple date class
const  date (afnix:sys:Date 0)
assert true (afnix:sys:date-p date)

# check access methods
assert 0 (date:year     true)
assert 1 (date:month    true)
assert 1 (date:day      true)
assert 1 (date:year-day true)
assert 0 (date:get-time true)
assert 0 (date:hours    true)
assert 0 (date:minutes  true)
assert 0 (date:seconds  true)

# check the mapping
assert "Sat" (date:map-day   true)
assert "Jan" (date:map-month true)

# check the format
assert "0000-01-01"                 (date:to-date true)
assert "00:00:00"                   (date:to-time true)
assert "0000-01-01T00:00:00Z"       (date:to-iso  true)
assert "Sat Jan 1 00:00:00 0"       (date:format  true)
assert "Sat, 01 Jan 0 00:00:00 GMT" (date:to-web)

# check the epoch (1970-1-1 is a thursday)
date:set-date 1970 1 1
assert 1970 (date:year     true)
assert 1    (date:month    true)
assert 1    (date:day      true)
assert 1    (date:year-day true)
# check the mapping
assert "Thu" (date:map-day   true)
assert "Jan" (date:map-month true)

# check the format
assert "1970-01-01"                    (date:to-date true)
assert "00:00:00"                      (date:to-time true)
assert "1970-01-01T00:00:00Z"          (date:to-iso  true)
assert "Thu Jan 1 00:00:00 1970"       (date:format  true)
assert "Thu, 01 Jan 1970 00:00:00 GMT" (date:to-web)

# add some year and month and check again
date:add-years 30
assert "2000-01-01"              (date:to-date true)
date:add-months 13
assert "2001-02-01"              (date:to-date true)

# check the web date
date:set-web "Sun, 31 Dec 2000 23:59:00 GMT"
assert 2000 (date:year    true)
assert 12   (date:month   true)
assert 31   (date:day     true)
assert 23   (date:hours   true)
assert 59   (date:minutes true)
assert 00   (date:seconds true)

# check the day before the atc epoch
date:set-date -1 12 31
assert -1  (date:year     true)
assert 12  (date:month    true)
assert 31  (date:day      true)
assert 365 (date:year-day true)

# check the gregorian proleptic epoch
date:set-date -4714 11 24
assert -4714 (date:year     true)
assert 11    (date:month    true)
assert 24    (date:day      true)

