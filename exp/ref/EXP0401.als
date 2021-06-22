# ----------------------------------------------------------------------------
# - EXP0401.als                                                              -
# - afnix example : chapter 4 : example 01                                   -
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

# complex number as an example of class description

# class declaration
const Complex (class)
trans Complex:preset (re im) {
  trans this:re (Real re)
  trans this:im (Real im)
}

# class mutators
trans Complex:set-re (x) (trans this:re re)
trans Complex:set-im (x) (trans this:im im)

# class accessors
trans Complex:get-re nil (Real this:re)
trans Complex:get-im nil (Real this:im)
trans Complex:module nil {
  trans result (Real (+ (* this:re this:re) (* this:im this:im)))
  result:sqrt
}
trans Complex:format nil {
  trans result (String this:re)
  result:+= "+i" 
  result:+= (String this:im)
}

# complex predicate
const complex-p (c) (
  if (instance-p c) (== Complex c:meta) false)

# operators
trans Complex:== (c) (
  if (complex-p c) (and (this:re:== c:re) (this:im:== c:im)) (
    if (number-p c) (and (this:re:== c) (this:im:zero-p)) false))

trans Complex:= (c) {
  if (complex-p c) {
    this:re:= (Real c:re)
    this:im:= (Real c:im)
    return this
  }
  this:re:= (Real c)
  this:im:= 0.0
  return this
}

trans Complex:+ (c) {
  trans result (Complex this:re this:im)
  if (complex-p c) {
    result:re:+= c:re
    result:im:+= c:im
    return result
  }
  result:re:+= (Real c)
  eval result
}

# create some complex number
const c1 (Complex 0.0 0.0)
const c2 (c1:+ 1)
println "complex [0.0:0.0] = " (c1:format)
println "complex [1.0:0.0] = " (c2:format)
