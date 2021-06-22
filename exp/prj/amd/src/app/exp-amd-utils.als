# ----------------------------------------------------------------------------
# - exp-amd-utils                                                            -
# - afnxi:amd utility functions module                                       -
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
# - type section                                                             -
# ----------------------------------------------------------------------------

# map an argument to a literal
# @param value the value to map

const afnix:amd:to-literal (value) (
  if (literal-p value) (value:to-literal) "nil"
)

# ----------------------------------------------------------------------------
# - copyright section                                                        -
# ----------------------------------------------------------------------------

# @return the copyright message

const afnix:amd:get-copyright-message nil {
  # format the copyright message
  + (+ AFNIX:AMD:TITLE " - ") AFNIX:AMD:HOLDER
}

# @return the full system version

const afnix:amd:get-revision-message nil {
  # get the revision info
  const major (afnix:amd:to-literal AFNIX:AMD:MAJOR)
  const minor (afnix:amd:to-literal AFNIX:AMD:MINOR)
  const patch (afnix:amd:to-literal AFNIX:AMD:PATCH)
  # format the revision message
  const mesg  (+ (+ (+ (+ (+ "revision " major) '.') minor) '.') patch)
  # add system info
  mesg:+= (+ ", afnix " interp:version) 
  mesg:+= (+ ", "       interp:os-name)
  # here it is
  eval mesg
}

# ----------------------------------------------------------------------------
# - uri section                                                              -
# ----------------------------------------------------------------------------

# normalize a uri name with a http scheme
# @param name the name to normalize

const afnix:amd:get-uri-name (name) {
  # cerate a uri with a normalized name
  const uri (afnix:nwg:Uri (afnix:nwg:normalize-uri-name name))
  # get the hierarchical name
  uri:get-hname
}

# compute the output name by uri and requested name
# @param uri the uri to process

const afnix:amd:get-output-name (uri) {
  # check if the file name has been set
  if afnix:amd:system-oflg (return afnix:amd:system-onam)
  # make sure we have a uri
  assert true (afnix:nwg:uri-p uri)
  # get the uri path target
  const ptnm (uri:get-path-target)
  # make sure we have something
  if (ptnm:nil-p) afnix:amd:system-onam ptnm
}

# create a new media controler by uri
# @param uri the uri to parse

const afnix:amd:newmc (uri) {
  # make sure we have a uri
  assert true (afnix:nwg:uri-p uri)
  # create a media controller from the scheme
  switch (uri:get-scheme) (
    ("http" (afnix:amd:htmmc uri))
    (else {
        const msg "invalid media scheme "
        throw "uri-error" (+ msg (uri:get-name))
      }
    )
  )
}
