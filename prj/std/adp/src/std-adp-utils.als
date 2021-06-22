# ----------------------------------------------------------------------------
# - std-adp-utils                                                            -
# - afnix:adp utility functions module                                       -
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
# - type section                                                             -
# ----------------------------------------------------------------------------

# map an argument to a literal
# @param value the value to map
const afnix:adp:to-literal (value) (
  if (literal-p value) (value:to-literal) "nil"
)

# ----------------------------------------------------------------------------
# - copyright section                                                        -
# ----------------------------------------------------------------------------

# @return the copyright message
const afnix:adp:get-copyright-message nil {
  # format the copyright message
  + (+ AFNIX:ADP:TITLE " - ") AFNIX:ADP:HOLDER
}

# @return the full system version
const afnix:adp:get-revision-message nil {
  # get the revision info
  const major (afnix:adp:to-literal AFNIX:ADP:MAJOR)
  const minor (afnix:adp:to-literal AFNIX:ADP:MINOR)
  const patch (afnix:adp:to-literal AFNIX:ADP:PATCH)
  # format the revision message
  const mesg  (+ (+ (+ (+ (+ "revision " major) '.') minor) '.') patch)
  # add system info
  mesg:+= (+ ", afnix " interp:version) 
  mesg:+= (+ ", "       interp:os-name)
  # here it is
  eval mesg
}

# ----------------------------------------------------------------------------
# - uri section                                                              -
# ----------------------------------------------------------------------------

# normalize a uri name into a system name
# @param name the name to normalize
const afnix:adp:get-uri-name (name) {
  afnix:nwg:system-uri-name name
}

# ----------------------------------------------------------------------------
# - format section                                                           -
# ----------------------------------------------------------------------------

# format a title by section numbers
# @param name the name to normalize
const afnix:adp:format-full-title (name args) {
  trans result ""
  for (v) (args) {
    if (result:nil-p) (result:+= (v:to-string)) {
      result:+= "."
      result:+= (v:to-string)
    }
  }
  if (result:nil-p) (result:+= name) {
    result:+= " - "
    result:+= name
  }
  eval result
}
