# ----------------------------------------------------------------------------
# - exp-amd-htmmc                                                            -
# - afnix:amd http media controller module                                   -
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
# - public section                                                           -
# ----------------------------------------------------------------------------

# the media controller clas
const afnix:amd:htmmc (class)
# the media nameset
const AFNIX:AMD:HTMMC (nameset AFNIX:AMD)

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the media controler type name
const AFNIX:AMD:HTMMC:HTTP-TYPE-NAME "http"
# the user agent header
const AFNIX:AMD:HTMMC:HTTP-USER-NAME "User-Agent"
const AFNIX:AMD:HTMMC:HTTP-USER-HEAD AFNIX:AMD:TITLE

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the controller environment
# @param uri the uri to process

trans afnix:amd:htmmc:preset (uri) {
  # make sure we have the right scheme
  assert AFNIX:AMD:HTMMC:HTTP-TYPE-NAME (uri:get-scheme)
  # save the uri
  const this:super uri
}

# ----------------------------------------------------------------------------
# - main section                                                             -
# ----------------------------------------------------------------------------

# execute the media dump by creating a http request to the authority

trans afnix:amd:htmmc:dump nil {
  # create a http request
  const hrq (afnix:nwg:HttpRequest this:super)
  # set the request header
  hrq:header-set AFNIX:AMD:HTMMC:HTTP-USER-NAME AFNIX:AMD:HTMMC:HTTP-USER-HEAD
  # get the socket host and port
  const host (this:get-host)
  const port (this:get-port)
  # open a tcp socket
  const s (afnix:net:TcpClient host port)
  # write the request
  hrq:write s
  # create a http response
  const hrs (afnix:nwg:HttpResponse)
  # parse the response
  hrs:parse s
  # check the response and dump
  if (hrs:status-ok-p) {
    # create a http stream
    const is (afnix:nwg:HttpStream hrs s)
    # create an output file
    const os (afnix:sio:OutputFile (afnix:amd:get-output-name uri))
    # read the response
    while (is:valid-p) (os:write (is:read))
    # close the streams
    os:close
  }
}
