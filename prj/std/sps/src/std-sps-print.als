# ----------------------------------------------------------------------------
# - std-sps-print                                                            -
# - afnix:sps cipher print class module                                      -
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

# the cipher print class
const afnix:sps:print (class)
# the print nameset
try (const AFNIX:SPS:PRINT (nameset AFNIX:SPS))

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the option messages
const AFNIX:SPS:PRINT:U-CLS-MSG "axi [i afnix-std-sps cmd-sps-print] [options]"
const AFNIX:SPS:PRINT:H-LCO-MSG "    [h]              print this help message"
const AFNIX:SPS:PRINT:V-LCO-MSG "    [v]              print system version"
const AFNIX:SPS:PRINT:B-LCO-MSG "    [b]              set break sequence"
const AFNIX:SPS:PRINT:F-UCO-MSG "    [F]              set sps format"
const AFNIX:SPS:PRINT:L-UCO-MSG "    [L]              add locale definition"

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the sps print class

trans afnix:sps:print:preset (argv) {
  # preini the print class
  this:preini argv
  # postdo the print class
  this:postdo
}

# preini the sps print class
# @param argv the argument vector

trans afnix:sps:print:preini (argv) {
  # create an option class and bind it
  const this:opts (afnix:sys:Options AFNIX:SPS:PRINT:U-CLS-MSG)
  # register the options
  this:opts:add-vector-option 'L' AFNIX:SPS:PRINT:L-UCO-MSG
  this:opts:add-string-option 'F' AFNIX:SPS:PRINT:F-UCO-MSG
  this:opts:add-string-option 'b' AFNIX:SPS:PRINT:B-LCO-MSG
  this:opts:add-unique-option 'v' AFNIX:SPS:PRINT:V-LCO-MSG
  this:opts:add-unique-option 'h' AFNIX:SPS:PRINT:H-LCO-MSG
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
    println (afnix:sps:get-copyright-message)
    println (afnix:sps:get-revision-message)
    afnix:sys:exit 0
  }
  # check for a break sequence
  if (this:opts:get-unique-option 'b') {
    const this:sbrk (this:opts:get-string-option 'b')
  } (const this:sbrk "")
  # check for a break sequence
  if (this:opts:get-unique-option 'F') {
    const this:xfmt (this:opts:get-string-option 'F')
  } (const this:xfmt "")
  # check for a locale
  if (this:opts:get-unique-option 'L') {
    const this:locv (this:opts:get-vector-option 'L')
  } (const this:locv nil)
  # get vector argument
  const this:varg (this:opts:get-vector-arguments)
}

# postdo the sps print class

trans afnix:sps:print:postdo nil {
  # create a sheet transfer
  trans this:st (afnix:sps:SheetTransfer this:xfmt this:sbrk)
  # add the locale definition
  for (ldef) (this:locv) (this:st:add-locale ldef)
}

# execute the command

trans afnix:sps:print:run nil {
  # import the file into the sheet
  for (f) (this:varg) (this:st:import f)
  # get the sheet
  const sht (st:get-sheet)
  # convert it to a table and format
  const tbl (sht:convert)
  tbl:format
}
