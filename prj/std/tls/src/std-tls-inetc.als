# ----------------------------------------------------------------------------
# - std-tls-inetc                                                            -
# - afnix:tls inet client class module                                       -
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

# the inet server class
const afnix:tls:inetc (class)
# the inet server nameset
try (const AFNIX:TLS:INETC (nameset AFNIX:TLS))

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the option messages
const AFNIX:TLS:INETC:U-CLS-MSG "axi [i afnix-std-tls cmd-tls-inetc] [options]"
const AFNIX:TLS:INETC:H-LCO-MSG "    [h]              print this help message"
const AFNIX:TLS:INETC:V-LCO-MSG "    [v]              print system version"
const AFNIX:TLS:INETC:T-LCO-MSG "    [t]              connect with tcp socket"
const AFNIX:TLS:INETC:P-LCO-MSG "    [p]              show the tls parameters"
const AFNIX:TLS:INETC:D-LCO-MSG "    [d]              set the debug flag"
const AFNIX:TLS:INETC:H-UCO-MSG "    [H]              set the host address"
const AFNIX:TLS:INETC:P-UCO-MSG "    [P]              set the host port"

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the inet server class

trans afnix:tls:inetc:preset (argv) {
  # preini the server class
  this:preini argv
  # postdo the server class
  this:postdo
}

# preini the inet server class
# @param argv the argument vector

trans afnix:tls:inetc:preini (argv) {
  # create an option class and bind it
  const this:opts (afnix:sys:Options AFNIX:TLS:INETC:U-CLS-MSG)
  # register the options
  this:opts:add-string-option 'P' AFNIX:TLS:INETC:P-UCO-MSG
  this:opts:add-string-option 'H' AFNIX:TLS:INETC:H-UCO-MSG
  this:opts:add-unique-option 'd' AFNIX:TLS:INETC:D-LCO-MSG
  this:opts:add-unique-option 'p' AFNIX:TLS:INETC:P-LCO-MSG
  this:opts:add-unique-option 't' AFNIX:TLS:INETC:T-LCO-MSG
  this:opts:add-unique-option 'v' AFNIX:TLS:INETC:V-LCO-MSG
  this:opts:add-unique-option 'h' AFNIX:TLS:INETC:H-LCO-MSG
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

# postdo the inet server class

trans afnix:tls:inetc:postdo nil {
  # create a tcp server socket
  if (this:opts:get-unique-option 'H') {
    const this:host (this:opts:get-string-option 'H')
  } (const this:host AFNIX:TLS:SERVER-HOST)
  if (this:opts:get-unique-option 'P') {
    const this:port (Integer (this:opts:get-string-option 'H'))
  } (const this:port AFNIX:TLS:SERVER-PORT)
  # get options
  const this:dbug (this:opts:get-unique-option 'd')
  const this:topt (this:opts:get-unique-option 't')
  # create the tls parameters
  const this:prms (afnix:tls:TlsParams this:host this:port)
  const this:popt (this:opts:get-unique-option 'p')
}

# execute the command

trans afnix:tls:inetc:run nil {
  # show the tls parameters
  if this:popt (this:show-tls-params)
  # connect the tls socket and get the connected socket
  const cs (if this:topt (this:accept-tcp-socket) (this:accept-tls-socket))
  if (nil-p cs) (return)
  # report in debug mode
  if this:dbug (afnix:tls:write-error-plist (cs:get-info))
  # collect the input/output stream
  const is (cs:get-input-stream)
  if (nil-p is) {
    errorln "[inetc] nil input stream in connect"
    afnix:sys:exit 1
  }
  const os (cs:get-output-stream)
  if (nil-p os) {
    errorln "[inetc] nil output stream in connect"
    afnix:sys:exit 1
  }
  # launch the read/write thread
  launch (this:read-loop is)
  launch (this:write-loop os)
}

# ----------------------------------------------------------------------------
# - report section                                                          -
# ----------------------------------------------------------------------------

trans afnix:tls:inetc:show-tls-params nil {
  # get the cipher suite table
  const ptbl (this:prms:get-info)
  # format the table
  afnix:tls:write-error-plist ptbl
}

# ----------------------------------------------------------------------------
# - process section                                                          -
# ----------------------------------------------------------------------------

# connect the socket with a tcp socket

trans afnix:tls:inetc:accept-tcp-socket nil {
  # create a tcp client
  const s (afnix:net:TcpClient (this:prms:to-client-sock-params))
  # create a tls connect object
  const co (afnix:tls:TlsConnect false this:prms)
  # get the connected socket state
  trans ssta (co:connect s s)
  # create a tls socket
  afnix:tls:TlsSocket s ssta
}

# connect the socket with a tls socket

trans afnix:tls:inetc:accept-tls-socket nil {
  # create a tls client
  afnix:tls:TlsClient this:prms
}

# run the tls read loop

trans afnix:tls:inetc:read-loop (is) {
  # loop in the input stream
  try {
    # loop until eos
    while (is:valid-p) {
      trans line (is:readln)
      println line
    }
  } {
    errorln "[inetc] " what:about
    afnix:sys:exit 1
  }
}

# run the tls write loop

trans afnix:tls:inetc:write-loop (os) {
  # create an output terminal
  const ts (afnix:sio:Terminal)
  ts:set-no-prompt true
  # loop until eof
  try {
    trans eos false
    while (not eos) {
      trans line (ts:read-line)
      trans eos  (line:eos-p)
      if (not eos) (os:write line)
    }
  } {
    errorln "[inetc] " what:about
    afnix:sys:exit 1
  }
}
