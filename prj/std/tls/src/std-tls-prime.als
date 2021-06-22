# ----------------------------------------------------------------------------
# - std-tls-prime                                                            -
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
const afnix:tls:prime (class)
# the random byte nameset
try (const AFNIX:TLS:PRIME (nameset AFNIX:TLS))

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the option messages
const AFNIX:TLS:PRIME:U-CLS-MSG "axi [i afnix-std-tls cmd-tls-prime] [options]"
const AFNIX:TLS:PRIME:H-LCO-MSG "    [h]              print this help message"
const AFNIX:TLS:PRIME:V-LCO-MSG "    [v]              print system version"
const AFNIX:TLS:PRIME:C-LCO-MSG "    [c]              check prime number"
const AFNIX:TLS:PRIME:D-LCO-MSG "    [d]              use decimal format"
const AFNIX:TLS:PRIME:R-LCO-MSG "    [r]              report number size"
const AFNIX:TLS:PRIME:S-LCO-MSG "    [s size]         prime number size"
const AFNIX:TLS:PRIME:S-UCO-MSG "    [S]              enable safe prime mode"

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the random byte class

trans afnix:tls:prime:preset (argv) {
  # preini the class
  this:preini argv
  # postdo the class
  this:postdo
}

# preini the tls class
# @param argv the argument vector

trans afnix:tls:prime:preini (argv) {
  # create an option class and bind it
  const this:opts (afnix:sys:Options AFNIX:TLS:PRIME:U-CLS-MSG)
  # register the options
  this:opts:add-unique-option 'S' AFNIX:TLS:PRIME:S-UCO-MSG
  this:opts:add-string-option 's' AFNIX:TLS:PRIME:S-LCO-MSG
  this:opts:add-unique-option 'r' AFNIX:TLS:PRIME:R-LCO-MSG
  this:opts:add-unique-option 'd' AFNIX:TLS:PRIME:D-LCO-MSG
  this:opts:add-unique-option 'c' AFNIX:TLS:PRIME:C-LCO-MSG
  this:opts:add-unique-option 'v' AFNIX:TLS:PRIME:V-LCO-MSG
  this:opts:add-unique-option 'h' AFNIX:TLS:PRIME:H-LCO-MSG
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

# postdo the prime class

trans afnix:tls:prime:postdo nil {
  # check for the prime number size
  if (this:opts:get-unique-option 's') {
    const this:size (Integer (this:opts:get-string-option 's'))
  } (const this:size 0)
  # check for flags
  const this:copt (this:opts:get-unique-option 'c')
  const this:dopt (this:opts:get-unique-option 'd')
  const this:ropt (this:opts:get-unique-option 'r')
  const this:sopt (this:opts:get-unique-option 'S')
  # preset prime number
  trans this:pnum nil
  # get the vector arguments
  trans this:varg (this:opts:get-vector-arguments)
}

# execute the command

trans afnix:tls:prime:run nil {
  # check for a prime number size
  if (> this:size 0) {
    trans this:pnum (if this:sopt
      (afnix:mth:get-random-safe  this:size)
      (afnix:mth:get-random-prime this:size))
    if this:dopt (println this:pnum) (println (this:pnum:to-hexa-string))
  }
  # check prime number
  if (and this:copt (object-p this:pnum)) {
    println "status: " (if this:sopt
      (afnix:mth:prime-safe-p this:pnum)
      (afnix:mth:prime-probable-p this:pnum))
  }
  # loop in arguments
  for (x) (this:varg) {
    # convert number
    trans pnum (Relatif x)
    # check for prime
    if this:copt {
      if this:dopt (println pnum) (println (pnum:to-hexa-string))
      println "status : " (if this:sopt
        (afnix:mth:prime-safe-p pnum)
        (afnix:mth:prime-probable-p pnum))
    }
    # report size
    if this:ropt { 
      println "size   : " (pnum:get-msb)
    }     
  }
}
