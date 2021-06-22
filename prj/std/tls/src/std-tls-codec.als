# ----------------------------------------------------------------------------
# - std-tls-codec                                                            -
# - afnix:tls cipher encode/decode class module                              -
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

# the cipher class
const afnix:tls:codec (class)
# the cipher nameset
try (const AFNIX:TLS:CODEC (nameset AFNIX:TLS))

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the option messages
const AFNIX:TLS:CODEC:U-CLS-MSG "axi [i afnix-std-tls cmd-tls-codec] [options]"
const AFNIX:TLS:CODEC:H-LCO-MSG "    [h]              print this help message"
const AFNIX:TLS:CODEC:V-LCO-MSG "    [v]              print system version"
const AFNIX:TLS:CODEC:I-LCO-MSG "    [i name]         set input file"
const AFNIX:TLS:CODEC:O-LCO-MSG "    [o name]         set output file"
const AFNIX:TLS:CODEC:R-LCO-MSG "    [r]              enable reverse cipher"
const AFNIX:TLS:CODEC:C-UCO-MSG "    [C type]         set cipher type"

# create a cipher by name
# @param cnam the cipher name
const afnix:tls:to-cipher (cnam) {
  # check for a pem cipher
  if (== cnam "pem") (return (afnix:tls:Pem))
  # invalid cipher
  throw "codec-error" (+ "invalid cipher name " cnam)
}

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the cipher class
trans afnix:tls:codec:preset (argv) {
  # preini the class
  this:preini argv
  # postdo the class
  this:postdo
}

# preini the cipher class
# @param argv the argument vector
trans afnix:tls:codec:preini (argv) {
  # create an option class and bind it
  const this:opts (afnix:sys:Options AFNIX:TLS:CODEC:U-CLS-MSG)
  # register the options
  this:opts:add-string-option 'C' AFNIX:TLS:CODEC:C-UCO-MSG
  this:opts:add-unique-option 'r' AFNIX:TLS:CODEC:R-LCO-MSG
  this:opts:add-string-option 'o' AFNIX:TLS:CODEC:O-LCO-MSG
  this:opts:add-string-option 'i' AFNIX:TLS:CODEC:I-LCO-MSG
  this:opts:add-unique-option 'v' AFNIX:TLS:CODEC:V-LCO-MSG
  this:opts:add-unique-option 'h' AFNIX:TLS:CODEC:H-LCO-MSG
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

# postdo the cipher class
trans afnix:tls:codec:postdo nil {
  # check for the input file name
  if (this:opts:get-unique-option 'i') {
    const this:inam (this:opts:get-string-option 'i')
  } (const this:inam nil)
  # check for the output file name
  if (this:opts:get-unique-option 'o') {
    trans this:onam (this:opts:get-string-option 'o')
  } (trans this:onam nil)
  # check for the cipher name
  if (this:opts:get-unique-option 'C') {
    trans this:cnam (this:opts:get-string-option 'C')
  } (trans this:cnam nil)
  # check for reverse mode
  const this:rmod (this:opts:get-unique-option 'r')
}


# execute the command

trans afnix:tls:codec:run nil {
  # check for valid input name
  if (nil-p this:inam) {
    errorln "[cipher] missing input name in command line"
    afnix:sys:exit 1
  }
  # check for valid output name
  if (nil-p this:onam) {
    errorln "[cipher] missing output name in command line"
    afnix:sys:exit 1
  }
  # check for valid cipher name
  if (nil-p this:cnam) {
    errorln "[cipher] missing cipher name in command line"
    afnix:sys:exit 1
  }
  # create a target cipher
  const cifr (afnix:tls:to-cipher this:cnam)
  cifr:set-reverse this:rmod
  # create an input stream
  const is (afnix:sio:InputFile this:inam)
  # create an output stream
  const os (afnix:sio:OutputFile this:onam)
  # stream the file
  trans clen (cifr:stream os is)
  # all done
  is:close
  os:close
}
