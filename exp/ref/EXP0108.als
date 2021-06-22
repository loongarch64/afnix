# ----------------------------------------------------------------------------
# - EXP0108.als                                                              -
# - afnix example : chapter 1 : example 08                                   -
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

# a simple function bind return the max of two numbers as an integer
const to-max (x y) (
  if (> x y) (Integer x) (Integer y))

# a simple color class
const Color (class)

# create a new color with 3 components
trans Color:preset (red green blue) {
  const this:red   (Integer red)
  const this:green (Integer green)
  const this:blue  (Integer blue)
}

# compute a darker color
const Color:RED-FACTOR   0.75
const Color:GREEN-FACTOR 0.75
const Color:BLUE-FACTOR  0.75
trans Color:get-darker nil {
  trans lr (to-max (* (Real this:red)   Color:RED-FACTOR)   0)
  trans lg (to-max (* (Real this:green) Color:GREEN-FACTOR) 0)
  trans lb (to-max (* (Real this:blue)  Color:BLUE-FACTOR)  0)
  Color lr lg lb
}

# print some color information
trans Color:info nil {
  println "red   : " this:red
  println "green : " this:green
  println "blue  : " this:blue
}

# bind some default colors
const Color:RED   (Color 255   0   0)
const Color:GREEN (Color   0 255   0)
const Color:BLUE  (Color   0   0 255)

# compute a red darker color and print the color values
const dark-red (Color:RED:get-darker)
dark-red:info
