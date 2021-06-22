# ---------------------------------------------------------------------------
# - XSYS001.als                                                             -
# - afnix example : system module example                                  -
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

# print the date and time information
# @author amaury darsch

# get the module
interp:library "afnix-sys"

# create a date instance
const date (afnix:sys:Time)

# print the information - local time
println "seconds since the epoch : " (date:get-time)
println "number of seconds       : " (date:seconds)
println "number of minutes       : " (date:minutes)
println "number of hours         : " (date:hours)
println "day in month            : " (date:day-of-month)
println "day in week             : " (date:day-of-week)
println "day in year             : " (date:day-of-year)
println "gregorian year          : " (date:year)
