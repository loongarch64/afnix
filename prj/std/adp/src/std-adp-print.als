# ----------------------------------------------------------------------------
# - std-adp-optdb                                                            -
# - afnix:adp option database module                                         -
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
const afnix:adp:print (class)
# the print nameset
try (const AFNIX:ADP:PRINT (nameset AFNIX:ADP))

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the option messages
const AFNIX:ADP:PRINT:U-CLS-MSG "axi [i afnix-std-adp] [options] uri"
const AFNIX:ADP:PRINT:H-LCO-MSG "    [h]      print this help message"
const AFNIX:ADP:PRINT:V-LCO-MSG "    [v]      print system version"
const AFNIX:ADP:PRINT:P-LCO-MSG "    [p]      parse only source content"
const AFNIX:ADP:PRINT:X-LCO-MSG "    [x]      enable xhtml mode"
const AFNIX:ADP:PRINT:M-LCO-MSG "    [m]      enable nroff manual mode"
const AFNIX:ADP:PRINT:L-LCO-MSG "    [l]      enable latex manual mode"
const AFNIX:ADP:PRINT:O-LCO-MSG "    [o]      set output file name"
const AFNIX:ADP:PRINT:D-UCO-MSG "    [D name] set the writer description"
const AFNIX:ADP:PRINT:I-UCO-MSG "    [I name] set the writer image"
const AFNIX:ADP:PRINT:N-UCO-MSG "    [N name] set the writer name"
const AFNIX:ADP:PRINT:S-UCO-MSG "    [S name] set the style name"
const AFNIX:ADP:PRINT:F-UCO-MSG "    [F name] set the boolean flag"

# the option flag
const AFNIX:ADP:PRINT:O-SYS-NOH "no-page-header"
const AFNIX:ADP:PRINT:O-SYS-NOD "no-body-div"

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the printer class
# @param argv the argument vector
trans afnix:adp:print:preset (argv) {
  # preini the print class
  this:preini argv
  # postdo the print class
  this:postdo
}

# preini the printer class
# @param argv the argument vector
trans afnix:adp:print:preini (argv) {
  # create an option class and bind it
  trans this:super (afnix:sys:Options AFNIX:ADP:PRINT:U-CLS-MSG)

  # register the options
  this:add-string-option 'F' AFNIX:ADP:PRINT:F-UCO-MSG
  this:add-string-option 'S' AFNIX:ADP:PRINT:S-UCO-MSG
  this:add-string-option 'N' AFNIX:ADP:PRINT:N-UCO-MSG
  this:add-string-option 'I' AFNIX:ADP:PRINT:I-UCO-MSG
  this:add-string-option 'D' AFNIX:ADP:PRINT:D-UCO-MSG
  this:add-string-option 'o' AFNIX:ADP:PRINT:O-LCO-MSG
  this:add-unique-option 'l' AFNIX:ADP:PRINT:L-LCO-MSG
  this:add-unique-option 'm' AFNIX:ADP:PRINT:M-LCO-MSG
  this:add-unique-option 'x' AFNIX:ADP:PRINT:X-LCO-MSG
  this:add-unique-option 'p' AFNIX:ADP:PRINT:P-LCO-MSG
  this:add-unique-option 'v' AFNIX:ADP:PRINT:V-LCO-MSG
  this:add-unique-option 'h' AFNIX:ADP:PRINT:H-LCO-MSG

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
    println (afnix:adp:get-copyright-message)
    println (afnix:adp:get-revision-message)
    afnix:sys:exit 0
  }
  # check for the output file name
  if (this:get-unique-option 'o') {
    trans afnix:adp:system-oflg true
    trans afnix:adp:system-onam (this:get-string-option 'o')
  }
  # check for the writer descrciption
  if (this:get-unique-option 'D') {
    trans afnix:adp:system-xdsc (this:get-string-option 'D')
  }
  # check for the writer logo
  if (this:get-unique-option 'I') {
    trans afnix:adp:system-ximg (this:get-string-option 'I')
  }
  # check for the writer name
  if (this:get-unique-option 'N') {
    trans afnix:adp:system-xsrc (this:get-string-option 'N')
  }
  # check for the writer style
  if (this:get-unique-option 'S') {
    trans afnix:adp:system-xcss (this:get-string-option 'S')
  }
  # get the requested uri argument
  const varg (this:get-vector-arguments)
  if (!= (varg:length) 1) {
    this:usage (interp:get-error-stream)
    afnix:sys:exit 1
  }
  const this:suri (varg:get 0)
}

# postdo the print command
trans afnix:adp:print:postdo nil {
  # check for present mode
  const this:pmod (this:get-unique-option 'p')
  # check for xhtml mode
  const this:xmod (this:get-unique-option 'x')
  # check for nroff mode
  const this:mmod (this:get-unique-option 'm')
  # check for latex mode
  const this:lmod (this:get-unique-option 'l')
  # check for boolean flag
  if (this:get-unique-option 'F') {
    # get the string option
    const xflg (this:get-string-option 'F')
    # dispatch flag
    if (== xflg AFNIX:ADP:PRINT:O-SYS-NOH) (trans afnix:adp:system-nohd true)
    if (== xflg AFNIX:ADP:PRINT:O-SYS-NOD) (trans afnix:adp:system-nobd true)
  }
}

# ----------------------------------------------------------------------------
# - command section                                                          -
# ----------------------------------------------------------------------------

# execute the command

trans afnix:adp:print:run nil {
  # check for parse mode
  if this:pmod {
    try {
      # create a new context
      const ctx (afnix:adp:ctxdb)
      # get the uri name
      const uri (afnix:adp:get-uri-name this:suri)
      # process source content by uri
      ctx:process-source-content uri
    } {
      errorln "error: " what:about
      afnix:sys:exit 1
    }
  }
  # check for xhtml mode
  if this:xmod {
    try {
      # load the xhtml module writer
      interp:load "std-adp-xhtmw"
      # create a new context
      const ctx (afnix:adp:xhtwr)
      # get the uri name
      const uri (afnix:adp:get-uri-name this:suri)
      # process the uri
      ctx:process uri
    } {
      errorln "error: " what:about
      afnix:sys:exit 1
    }
  }
  # check for nroff manual mode
  if mmod {
    try {
      # load the nroff module writer
      interp:load "std-adp-manmw"
      # create a new context
      const ctx (afnix:adp:manwr)
      # get the uri name
      const uri (afnix:adp:get-uri-name this:suri)
      # process the uri
      ctx:process uri
    } {
      errorln "error: " what:about
      afnix:sys:exit 1
    }
  }
  # check for latex mode
  if lmod {
    try {
      # load the tex module writer
      interp:load "std-adp-texmw"
      # create a new context
      const ctx (afnix:adp:texwr)
      # get the uri name
      const uri (afnix:adp:get-uri-name this:suri)
      # process the uri
      ctx:process uri
    } {
      errorln "error: " what:about
      afnix:sys:exit 1
    }
  }
}
