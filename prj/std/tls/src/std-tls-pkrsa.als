# ----------------------------------------------------------------------------
# - std-tls-pkrsa                                                            -
# - afnix:tls rsa key class module                                           -
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

# the rsa key class
const afnix:tls:pkrsa (class)
# the rsa key nameset
try (const AFNIX:TLS:PKRSA (nameset AFNIX:TLS))

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the option messages
const AFNIX:TLS:PKRSA:U-CLS-MSG "axi [i afnix-std-tls cmd-tls-pkrsa] [options]"
const AFNIX:TLS:PKRSA:H-LCO-MSG "    [h]              print this help message"
const AFNIX:TLS:PKRSA:V-LCO-MSG "    [v]              print system version"
const AFNIX:TLS:PKRSA:V-UCO-MSG "    [V]              enable verbose mode"
const AFNIX:TLS:PKRSA:D-LCO-MSG "    [d]              use decimal format"
const AFNIX:TLS:PKRSA:S-LCO-MSG "    [s size]         create a key by size"
const AFNIX:TLS:PKRSA:F-LCO-MSG "    [f path]         set the file path"

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the rsa key class

trans afnix:tls:pkrsa:preset (argv) {
  # preini the class
  this:preini argv
  # postdo the class
  this:postdo
}

# preini the tls class
# @param argv the argument vector

trans afnix:tls:pkrsa:preini (argv) {
  # create an option class and bind it
  const this:opts (afnix:sys:Options AFNIX:TLS:PKRSA:U-CLS-MSG)
  # register the options
  this:opts:add-string-option 'f' AFNIX:TLS:PKRSA:F-LCO-MSG
  this:opts:add-string-option 's' AFNIX:TLS:PKRSA:S-LCO-MSG
  this:opts:add-unique-option 'd' AFNIX:TLS:PKRSA:D-LCO-MSG
  this:opts:add-unique-option 'V' AFNIX:TLS:PKRSA:V-UCO-MSG
  this:opts:add-unique-option 'v' AFNIX:TLS:PKRSA:V-LCO-MSG
  this:opts:add-unique-option 'h' AFNIX:TLS:PKRSA:H-LCO-MSG
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

# postdo the res key class

trans afnix:tls:pkrsa:postdo nil {
  # check for the key size
  if (this:opts:get-unique-option 's') {
    const this:size (Integer (this:opts:get-string-option 's'))
  } (const this:size 0)
  # check for a path
  if (this:opts:get-unique-option 'f') {
    const this:path (this:opts:get-string-option 'f')
  } (const this:path nil)
  
  # check for flags
  const this:vopt (this:opts:get-unique-option 'V')
  const this:dopt (this:opts:get-unique-option 'd')
  # preset the key
  trans this:rkey nil
}

# execute the command

trans afnix:tls:pkrsa:run nil {
  # create a key by size
  if (> this:size 0) {
    # generate the key
    trans this:rkey (afnix:sec:Key afnix:sec:Key:KRSA size)
  }
  # check for export
  if (object-p this:path) {
    # create a pkcs object
    const pkcs (afnix:tls:Pkcs1 this:rkey)
    # write to a file
    pkcs:write this:path
  }
  # check for verbose mode
  if this:vopt {
    # report modulus
    const m (this:rkey:get-relatif-key afnix:sec:Key:RSA-MODULUS)
    println "modulus:"
    if this:dopt (println m) (println (m:to-hexa-string))
    # report public exponent
    const e (this:rkey:get-relatif-key afnix:sec:Key:RSA-PUBLIC-EXPONENT)
    println "public exponent:"
    if this:dopt (println e) (println (e:to-hexa-string))
    # report secret exponent
    const s (this:rkey:get-relatif-key afnix:sec:Key:RSA-SECRET-EXPONENT)
    println "secret exponent:"
    if this:dopt (println s) (println (s:to-hexa-string))
  }
}
