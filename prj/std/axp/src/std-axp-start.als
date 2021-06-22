# ----------------------------------------------------------------------------
# - std-axp-start                                                            -
# - afnix:axp starter module                                                 -
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
# - project load section                                                     -
# ----------------------------------------------------------------------------

interp:load "std-axp-prjld"
interp:load "std-axp-postl"

# ----------------------------------------------------------------------------
# - main section                                                             -
# ----------------------------------------------------------------------------

# check for parse mode

if (afnix:axp:options:get-unique-option 'p') {
  try {
    # create a new context
    const ctx (afnix:axp:ctxdb)
    # get the uri name from the arguments
    const uri (afnix:axp:options:get-uri-name)
    # process source content by uri
    ctx:process-source-content uri
  } {
    errorln "error: " what:about
    afnix:sys:exit 1
  }
}

# process the content
try {
  # create a new context
  const ctx (afnix:axp:ctxdb)
  # get the uri name from the arguments
  const uri (afnix:axp:options:get-uri-name)
  # process source content by uri
  ctx:process uri
} {
  errorln "error: " what:about
  afnix:sys:exit 1
}

# unload the project
interp:load "std-axp-punld"
