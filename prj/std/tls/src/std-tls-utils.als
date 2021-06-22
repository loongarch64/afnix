# ----------------------------------------------------------------------------
# - std-tls-utils                                                            -
# - afnix:std:tls utility functions unit                                     -
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
const AFNIX:TLS:UTILS (nameset AFNIX:TLS)

# ----------------------------------------------------------------------------
# - type section                                                             -
# ----------------------------------------------------------------------------

# map an object representation
# @param value the value to map

const afnix:tls:to-repr (value) (
  if (object-p value) (value:repr) "nil"
)

# check that a string is valid
# @param value the value to check

const afnix:tls:object-string-p (value) {
  # check for nil
  if (nil-p value) (return false)
  # check for a string
  if (not (string-p value)) (return false)
  # check string content
  not (value:nil-p)
}

# map a literal to a string
# @param value the literal to map

const afnix:tls:to-string (value) (
  if (literal-p value) (value:to-string) "nil"
)

# ----------------------------------------------------------------------------
# - copyright section                                                        -
# ----------------------------------------------------------------------------

# format a revision string
# @param major the major number
# @param minor the minor number
# @param patch the patch number

const afnix:tls:get-revision-number (major minor patch) {
  # format the number
  const mj (afnix:tls:to-string major)
  const mn (afnix:tls:to-string minor)
  const pt (afnix:tls:to-string patch)  
  # build revision
  + (+ (+ (+ mj '.') mn) '.') pt
}

# @return a formated revision string

const afnix:tls:get-revision-string nil {
  afnix:tls:get-revision-number AFNIX:TLS:MAJOR AFNIX:TLS:MINOR AFNIX:TLS:PATCH
}

# @return the copyright message

const afnix:tls:get-copyright-message nil {
  # format the copyright message
  + (+ AFNIX:TLS:TITLE ", ") AFNIX:TLS:CLEFT
}

# @return the full system version

const afnix:tls:get-revision-message nil {
  # get the revision info
  const mrev (afnix:tls:get-revision-string)
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

const afnix:tls:write-error-plist (plst) {
  # convert the plist to a print table
  const ptbl (plst:to-print-table true)
  # write to the interpreter error stream
  ptbl:format (interp:get-error-stream)
}

# ----------------------------------------------------------------------------
# - i/o section                                                              -
# ----------------------------------------------------------------------------

# get a credential from the terminal
const afnix:tls:get-credential nil {
  # duplicate an interpreter for reading
  const di (interp:dup)
  # read the credential
  di:read-credential "credential: "
}

