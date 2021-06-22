# ----------------------------------------------------------------------------
# - exp-amd-optdb                                                            -
# - afnix:amd option database module                                         -
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
# - global section                                                          -
# ----------------------------------------------------------------------------

# the option database class
const afnix:amd:optdb (class)

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the option messages
const AFNIX:AMD:U-CLS-MSG "axi [i afnix-amd] amd-start [options] uri"
const AFNIX:AMD:H-LCO-MSG "    [h]           print this help message"
const AFNIX:AMD:V-LCO-MSG "    [v]           print system version"
const AFNIX:AMD:O-LCO-MSG "    [o]           set output file name"

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# initialize the option descriptors
trans afnix:amd:optdb:preset (argv) {
  # create an option class and bind it
  trans this:super (afnix:sys:Options AFNIX:AMD:U-CLS-MSG)

  # register the options
  this:add-string-option 'o' AFNIX:AMD:O-LCO-MSG
  this:add-unique-option 'v' AFNIX:AMD:V-LCO-MSG
  this:add-unique-option 'h' AFNIX:AMD:H-LCO-MSG

  # parse the options
  try (this:parse argv) {
    this:usage (interp:get-error-stream)
    afnix:sys:exit 1
  }
  # check for the help option
  if (this:get-unique-option 'h') {
    this:usage (interp:get-output-stream)
    afnix:sys:exit 0
  }
  # check for the version option
  if (this:get-unique-option 'v') {
    println (afnix:amd:get-copyright-message)
    println (afnix:amd:get-revision-message)
    afnix:sys:exit 0
  }
  # check for the output file name
  if (this:get-unique-option 'o') {
    trans afnix:amd:system-oflg true
    trans afnix:amd:system-onam (this:get-string-option 'o')
  }
  # get the requested uri argument
  const varg (this:get-vector-arguments)
  if (!= (varg:length) 1) {
    this:usage (interp:get-error-stream)
    afnix:sys:exit 1
  }
  const this:uri (afnix:amd:get-uri-name (varg:get 0))
}

# ----------------------------------------------------------------------------
# - methods section                                                          -
# ----------------------------------------------------------------------------

# @return the requested uri

trans afnix:amd:optdb:get-uri nil {
  afnix:nwg:Uri this:uri
}
