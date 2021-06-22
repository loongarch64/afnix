# ----------------------------------------------------------------------------
# - std-adp-ctxdb                                                            -
# - afnix:adp context database module                                        -
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
const afnix:adp:ctxdb (class)

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the context environment
# @param uri the context uri name

trans afnix:adp:ctxdb:preset nil {
  # preset the base object
  if (object-p this:super) (this:super:preset)
  # preini the environment
  this:preini
}

# preini the context environment

trans afnix:adp:ctxdb:preini nil {
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

trans afnix:adp:ctxdb:process-source-content (uri) {
  # create a new xml content
  const scnt (afnix:xpe:XmlContent uri)
  # process the source content
  this:xmlp:process scnt
}

# process a source file by uri name
# @param uri the uri name to process

trans afnix:adp:ctxdb:process (uri) {
  # process the source content
  const xcnt (this:process-source-content uri)
  # get the root node and dispatch
  const root (xcnt:get-root)
  # dispatch the root node
  this:expands-root root
  # write the content
  this:write
}
