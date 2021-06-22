# ----------------------------------------------------------------------------
# - std-axp-optdb                                                            -
# - afnix:axp option database module                                         -
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

# the option database class
const afnix:axp:optdb (class)

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the option messages
const AFNIX:AXP:U-CLS-MSG "axi [i afnix-std-axp -m --] [options] uri"
const AFNIX:AXP:H-LCO-MSG "    [h]              print this help message"
const AFNIX:AXP:V-LCO-MSG "    [v]              print system version"
const AFNIX:AXP:P-LCO-MSG "    [p]              parse only the source content"
const AFNIX:AXP:X-LCO-MSG "    [x]              set output file extension"
const AFNIX:AXP:O-LCO-MSG "    [o]              set output file name"

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# initialize the option descriptors
trans afnix:axp:optdb:preset (argv) {
  # create an option class and bind it
  trans this:super (afnix:sys:Options AFNIX:AXP:U-CLS-MSG)

  # register the options
  this:add-string-option 'o' AFNIX:AXP:O-LCO-MSG
  this:add-string-option 'x' AFNIX:AXP:X-LCO-MSG
  this:add-unique-option 'p' AFNIX:AXP:P-LCO-MSG
  this:add-unique-option 'v' AFNIX:AXP:V-LCO-MSG
  this:add-unique-option 'h' AFNIX:AXP:H-LCO-MSG

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
    println (afnix:axp:get-copyright-message)
    println (afnix:axp:get-revision-message)
    afnix:sys:exit 0
  }
  # get the requested uri argument
  const varg (this:get-vector-arguments)
  if (!= (varg:length) 1) {
    this:usage (interp:get-error-stream)
    afnix:sys:exit 1
  }
  const this:uri (varg:get 0)

  # check for extension file name
  if (this:get-unique-option 'x') {
    # get the extension option
    trans afnix:axp:system-xnam (this:get-string-option 'x')
    # remove input extension
    trans afnix:axp:system-onam (afnix:sio:remove-extension this:uri)
    # add output extension
    afnix:axp:system-onam:+= '.'
    afnix:axp:system-onam:+= afnix:axp:system-xnam
  }

  # check for the output file name
  if (this:get-unique-option 'o') {
    trans afnix:axp:system-oflg true
    trans afnix:axp:system-onam (this:get-string-option 'o')
  }
}

# ----------------------------------------------------------------------------
# - methods section                                                          -
# ----------------------------------------------------------------------------

# @return the requested uri name

trans afnix:axp:optdb:get-uri-name nil {
  afnix:axp:get-uri-name this:uri
}
