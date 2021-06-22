# ----------------------------------------------------------------------------
# - exp-amd-start                                                            -
# - afnix:amd starter module                                                 -
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
# - project load section                                                     -
# ----------------------------------------------------------------------------

interp:load "exp-amd-prjld"
interp:load "exp-amd-postl"

# ----------------------------------------------------------------------------
# - main section                                                             -
# ----------------------------------------------------------------------------

try {
  # create a new uri from the option argument
  const uri (afnix:amd:options:get-uri)
  # get the media controler from the scheme
  const vmc (afnix:amd:newmc uri)
  # dump the content
  vmc:dump
} {
  errorln "error: " what:reason
  afnix:sys:exit 1
}

# ----------------------------------------------------------------------------
# - project unload section                                                   -
# ----------------------------------------------------------------------------

interp:load "exp-amd-punld"
