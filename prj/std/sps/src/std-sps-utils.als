# ----------------------------------------------------------------------------
# - std-sps-utils                                                            -
# - afnix:std:sps utility functions unit                                     -
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

# ----------------------------------------------------------------------------
# - global section                                                           -
# ----------------------------------------------------------------------------

# the utility nameset
const AFNIX:SPS:UTILS (nameset AFNIX:SPS)

# ----------------------------------------------------------------------------
# - type section                                                             -
# ----------------------------------------------------------------------------

# map an object representation
# @param value the value to map

const afnix:sps:to-repr (value) (
  if (object-p value) (value:repr) "nil"
)

# check that a string is valid
# @param value the value to check

const afnix:sps:object-string-p (value) {
  # check for nil
  if (nil-p value) (return false)
  # check for a string
  if (not (string-p value)) (return false)
  # check string content
  not (value:nil-p)
}

# map a literal to a string
# @param value the literal to map

const afnix:sps:to-string (value) (
  if (literal-p value) (value:to-string) "nil"
)

# ----------------------------------------------------------------------------
# - copyright section                                                        -
# ----------------------------------------------------------------------------

# format a revision string
# @param major the major number
# @param minor the minor number
# @param patch the patch number

const afnix:sps:get-revision-number (major minor patch) {
  # format the number
  const mj (afnix:sps:to-string major)
  const mn (afnix:sps:to-string minor)
  const pt (afnix:sps:to-string patch)  
  # build revision
  + (+ (+ (+ mj '.') mn) '.') pt
}

# @return a formated revision string

const afnix:sps:get-revision-string nil {
  afnix:sps:get-revision-number AFNIX:SPS:MAJOR AFNIX:SPS:MINOR AFNIX:SPS:PATCH
}

# @return the copyright message

const afnix:sps:get-copyright-message nil {
  # format the copyright message
  + (+ AFNIX:SPS:TITLE ", ") AFNIX:SPS:CLEFT
}

# @return the full system version

const afnix:sps:get-revision-message nil {
  # get the revision info
  const mrev (afnix:sps:get-revision-string)
  # format the revision message
  const mesg (+ "revision " mrev)
  # add system info
  mesg:+= (+ ", afnix " interp:version) 
  mesg:+= (+ ", "       interp:os-name)
  # here it is
  eval mesg
}

# ----------------------------------------------------------------------------
# - plist section                                                            -
# ----------------------------------------------------------------------------

# dump a property list to the error stream for debug
# @param plst the property list to dump

const afnix:sps:write-error-plist (plst) {
  # convert the plist to a print table
  const ptbl (plst:to-print-table true)
  # write to the interpreter error stream
  ptbl:format (interp:get-error-stream)
}
