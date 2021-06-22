# ----------------------------------------------------------------------------
# - std-axp-ctxdb                                                            -
# - afnix:axp context database module                                        -
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

# define the operating context
const afnix:axp:ctxdb (class)

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the context environment
# @param uri the context uri name

trans afnix:axp:ctxdb:preset nil {
  # preset the base object
  if (object-p this:super) (this:super:preset)
  # preini the environment
  this:preini
}

# preini the context environment

trans afnix:axp:ctxdb:preini nil {
  # create the xml processor
  const this:xmlp (afnix:xpe:XmlProcessor)
  # bind the xml include feature
  this:xmlp:add-feature (afnix:xpe:XmlInclude)
}

# ----------------------------------------------------------------------------
# - method section                                                           -
# ----------------------------------------------------------------------------

# process a source content by uri name
# @param uri the uri name to process

trans afnix:axp:ctxdb:process-source-content (uri) {
  # create a new xml content
  const scnt (afnix:xpe:XmlContent uri)
  # process the source content
  this:xmlp:process scnt
}

# write a root node

trans afnix:axp:ctxdb:write-root-node (root) {
  # create an output file
  const os (afnix:sio:OutputFile afnix:axp:system-onam "UTF-8")
  # write the page
  root:write os
  # close the file
  os:close
}

# process a source file by uri name
# @param uri the uri name to process

trans afnix:axp:ctxdb:process (uri) {
  # process the source content
  const xcnt (this:process-source-content uri)
  # get the root node and write
  const root (xcnt:get-root)
  # write the content
  this:write-root-node root
}
