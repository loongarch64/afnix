# ----------------------------------------------------------------------------
# - std-tls-brand                                                            -
# - afnix:tls random byte class module                                       -
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

# the random byte class
const afnix:tls:brand (class)
# the random byte nameset
try (const AFNIX:TLS:BRAND (nameset AFNIX:TLS))

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the option messages
const AFNIX:TLS:BRAND:U-CLS-MSG "axi [i afnix-std-tls cmd-tls-brand] [options]"
const AFNIX:TLS:BRAND:H-LCO-MSG "    [h]              print this help message"
const AFNIX:TLS:BRAND:V-LCO-MSG "    [v]              print system version"
const AFNIX:TLS:BRAND:U-LCO-MSG "    [u]              random uuid"
const AFNIX:TLS:BRAND:N-LCO-MSG "    [n size]         the number of byte"

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the random byte class

trans afnix:tls:brand:preset (argv) {
  # preini the class
  this:preini argv
}

# preini the tls class
# @param argv the argument vector

trans afnix:tls:brand:preini (argv) {
  # create an option class and bind it
  const this:opts (afnix:sys:Options AFNIX:TLS:BRAND:U-CLS-MSG)
  # register the options
  this:opts:add-unique-option 'u' AFNIX:TLS:BRAND:U-LCO-MSG
  this:opts:add-string-option 'n' AFNIX:TLS:BRAND:N-LCO-MSG
  this:opts:add-unique-option 'v' AFNIX:TLS:BRAND:V-LCO-MSG
  this:opts:add-unique-option 'h' AFNIX:TLS:BRAND:H-LCO-MSG
  # parse the options
  try (this:opts:parse argv) {
    this:opts:usage (interp:get-error-stream)
    afnix:sys:exit 1
  }
  # check for the help option
  if (this:opts:get-unique-option 'h') {
    this:opts:usage (interp:get-output-stream)
    afnix:sys:exit 0
  }
  # check for the version option
  if (this:opts:get-unique-option 'v') {
    println (afnix:tls:get-copyright-message)
    println (afnix:tls:get-revision-message)
    afnix:sys:exit 0
  }
  # check for the number of bytes
  if (this:opts:get-unique-option 'n') {
    const this:bnum (Integer (this:opts:get-string-option 'n'))
  } (const this:bnum 0)
  # check for flags
  const this:uopt (this:opts:get-unique-option 'u')
}


# execute the command

trans afnix:tls:brand:run nil {
  # generate a random uuid
  if this:uopt {
    const uuid (afnix:nwg:Uuid)
    uuid:random
    println (uuid:to-string)
  }
  # generate random bytes
  if (> this:bnum 0) {
    loop (trans k 0) (< k this:bnum) (k:++) {
      trans rb (afnix:mth:get-random-byte)
      print (rb:to-hexa-string)
    }
    println
  }
}
