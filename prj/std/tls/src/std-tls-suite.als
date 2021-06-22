# ----------------------------------------------------------------------------
# - std-tls-suite                                                            -
# - afnix:tls cipher suite class module                                      -
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

# the cipher suite class
const afnix:tls:suite (class)
# the suite nameset
try (const AFNIX:TLS:SUITE (nameset AFNIX:TLS))

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the option messages
const AFNIX:TLS:SUITE:U-CLS-MSG "axi [i afnix-std-tls cmd-tls-suite] [options]"
const AFNIX:TLS:SUITE:H-LCO-MSG "    [h]              print this help message"
const AFNIX:TLS:SUITE:V-LCO-MSG "    [v]              print system version"

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the tls suite class

trans afnix:tls:suite:preset (argv) {
  # preini the suite class
  this:preini argv
  # postdo the suite class
  this:postdo
}

# preini the tls suite class
# @param argv the argument vector

trans afnix:tls:suite:preini (argv) {
  # create an option class and bind it
  const this:opts (afnix:sys:Options AFNIX:TLS:SUITE:U-CLS-MSG)
  # register the options
  this:opts:add-unique-option 'v' AFNIX:TLS:SUITE:V-LCO-MSG
  this:opts:add-unique-option 'h' AFNIX:TLS:SUITE:H-LCO-MSG
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
}

# postdo the tls suite class

trans afnix:tls:suite:postdo nil {
  # create a tls suite
  const this:suit (afnix:tls:TlsSuite)
}

# execute the command

trans afnix:tls:suite:run nil {
  # get the cipher suite table
  const ptbl (this:suit:get-cipher-table)
  # format the table
  ptbl:format (interp:get-output-stream)
}
