# ----------------------------------------------------------------------------
# - std-tls-pbkdf                                                            -
# - afnix:tls pbkdf key class module                                         -
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
const afnix:tls:pbkdf (class)
# the rsa key nameset
try (const AFNIX:TLS:PBKDF (nameset AFNIX:TLS))

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the option messages
const AFNIX:TLS:PBKDF:U-CLS-MSG "axi [i afnix-std-tls cmd-tls-pbkdf] [options]"
const AFNIX:TLS:PBKDF:H-LCO-MSG "    [h]              print this help message"
const AFNIX:TLS:PBKDF:V-LCO-MSG "    [v]              print system version"
const AFNIX:TLS:PBKDF:V-UCO-MSG "    [V]              enable verbose mode"
const AFNIX:TLS:PBKDF:C-LCO-MSG "    [c cred]         set the key credential"
const AFNIX:TLS:PBKDF:I-LCO-MSG "    [i inum]         set the iteration number"
const AFNIX:TLS:PBKDF:S-LCO-MSG "    [s salt]         set the salt"
const AFNIX:TLS:PBKDF:G-LCO-MSG "    [g ks:is:ss]     set the key geometry"
const AFNIX:TLS:PBKDF:K-UCO-MSG "    [K]              print the key"
const AFNIX:TLS:PBKDF:I-UCO-MSG "    [I]              print the iv vector"
const AFNIX:TLS:PBKDF:S-UCO-MSG "    [S]              print the salt"

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the pkkdf class

trans afnix:tls:pbkdf:preset (argv) {
  # preini the class
  this:preini argv
  # postdo the class
  this:postdo
}

# preini the tls class
# @param argv the argument vector

trans afnix:tls:pbkdf:preini (argv) {
  # create an option class and bind it
  const this:opts (afnix:sys:Options AFNIX:TLS:PBKDF:U-CLS-MSG)
  # register the options
  this:opts:add-unique-option 'S' AFNIX:TLS:PBKDF:S-UCO-MSG
  this:opts:add-unique-option 'I' AFNIX:TLS:PBKDF:I-UCO-MSG
  this:opts:add-unique-option 'K' AFNIX:TLS:PBKDF:K-UCO-MSG
  this:opts:add-string-option 'g' AFNIX:TLS:PBKDF:G-LCO-MSG
  this:opts:add-string-option 's' AFNIX:TLS:PBKDF:S-LCO-MSG
  this:opts:add-string-option 'i' AFNIX:TLS:PBKDF:I-LCO-MSG
  this:opts:add-string-option 'c' AFNIX:TLS:PBKDF:C-LCO-MSG
  this:opts:add-unique-option 'V' AFNIX:TLS:PBKDF:V-UCO-MSG
  this:opts:add-unique-option 'v' AFNIX:TLS:PBKDF:V-LCO-MSG
  this:opts:add-unique-option 'h' AFNIX:TLS:PBKDF:H-LCO-MSG
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

trans afnix:tls:pbkdf:postdo nil {
  # check for credential
  if (this:opts:get-unique-option 'c') {
    trans this:cred (this:opts:get-string-option 'c')
  } (trans this:cred (afnix:tls:get-credential))
  # check for iteration number
  if (this:opts:get-unique-option 'i') {
    trans this:inum (Integer (this:opts:get-string-option 'i'))
  } (trans this:inum 1)
  # check for the key size
  if (this:opts:get-unique-option 'g') {
    const sval (this:opts:get-string-option 'g')
    const svec (sval:split ":")
    trans this:ksiz (if (> (svec:length) 0) (Integer (svec:get 0)) 0)
    trans this:isiz (if (> (svec:length) 1) (Integer (svec:get 1)) 0)
    trans this:ssiz (if (> (svec:length) 2) (Integer (svec:get 2)) 0)
  }
  # check for a salt data
  if (and (> this:ssiz 0) (this:opts:get-unique-option 's')) {
    errorln "[pbkdf] incompatible salt size with salt value"
    afnix:sys:exit 1
  }
  const this:salt (this:opts:get-string-option 's')
  # check for flags
  const this:kopt (this:opts:get-unique-option 'K')
  const this:iopt (this:opts:get-unique-option 'I')
  const this:sopt (this:opts:get-unique-option 'S')
}

# execute the command

trans afnix:tls:pbkdf:run nil {
  # compute the key size
  const size (+ this:ksiz this:isiz)
  if (<= size 0) (return)
  # create a key by size
  const pkdf (afnix:sec:Pbkdf2 size inum)
  # derive the salt by size or data
  if (> this:ssiz 0) (pkdf:new-salt this:ssiz)
  if (not (this:salt:nil-p)) (pkdf:set-salt this:salt)
  # derive the key
  const kbuf (pkdf:derive this:cred)
  # format the result
  if (not (or this:kopt (or this:iopt this:sopt))) (println (kbuf:format))
  if (and this:kopt (> this:ksiz 0)) {
    trans tbuf (kbuf:extract 0 this:ksiz)
    println (tbuf:format)
  }
  if (and this:iopt (> this:isiz 0)) {
    trans tbuf (kbuf:extract this:ksiz this:isiz)
    println (tbuf:format)
  }
  if this:sopt {
    trans sbuf (pkdf:get-salt)
    println (sbuf:format)
  }
}
