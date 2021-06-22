# ----------------------------------------------------------------------------
# - std-adp-manut                                                            -
# - afnix:adp nroff manual utilities classes module                          -
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
# - utility section                                                          -
# ----------------------------------------------------------------------------

# @return today's date
const afnix:adp:get-date nil {
  const date (afnix:sys:Date)
  date:to-date
}

# ----------------------------------------------------------------------------
# - command section                                                          -
# ----------------------------------------------------------------------------

# the manual simple command class
const afnix:adp:mansc (class)

# preset the simple command
# @param name the command name

trans afnix:adp:mansc:preset (name) {
  const this:cnam (name:clone)
  trans this:cdat (String)
}

# set the command data
# @param text the command data text

trans afnix:adp:mansc:set-data (text) {
  trans this:cdat (text:clone)
}

# write the simple command to an output stream
# @param os the output stream to write

trans afnix:adp:mansc:write (os) {
  # write the command if any
  if (not (this:cnam:nil-p)) {
    os:newline
    os:write this:cnam
    os:write ' '
  }
  # write the command data
  if (not (this:cdat:nil-p)) {
    os:newline
    os:write this:cdat
  }
}

# the manual command class
const afnix:adp:mancc (class)

# preset the command manual
# @param name the command name
# @param text the command text

trans afnix:adp:mancc:preset (name text) {
  const this:cnam (name:clone)
  trans this:ctxt (text:clone)
  trans this:head (String)
  trans this:cdat (String)
}

# set the command head
# @param text the command data text

trans afnix:adp:mancc:set-head (text) {
  trans this:head (text:clone)
}

# set the command data
# @param text the command data text

trans afnix:adp:mancc:set-data (text) {
  trans this:cdat (text:clone)
}

# write the command to an output stream
# @param os the output stream to write

trans afnix:adp:mancc:write (os) {
  # write the head if any
  if (not (this:head:nil-p)) {
    os:newline
    os:write this:head
  }
  # write the command if any
  if (not (this:cnam:nil-p)) {
    os:newline
    os:write this:cnam
    os:write ' '
  }
  # write the command text
  if (not (this:ctxt:nil-p)) {
    os:write this:ctxt
  }
  # write the command data
  if (not (this:cdat:nil-p)) {
    os:newline
    os:write this:cdat
  }
}

# ----------------------------------------------------------------------------
# - para section                                                             -
# ----------------------------------------------------------------------------

# the para command class
const afnix:adp:manpc (class)

# preset the command manual

trans afnix:adp:manpc:preset nil {
  trans this:cdat (String)
}

# write the command to an output stream
# @param os the output stream to write

trans afnix:adp:manpc:write (os) {
  # write the command data
  if (not (this:cdat:nil-p)) {
    os:newline
    os:write this:cdat
  }
}

# add some text to the paragraph
# @param text the text to add

trans afnix:adp:manpc:add-text (text) {
  if (this:cdat:nil-p) (this:cdat:+= (text:strip-left " \t\r\n")) {
    this:cdat:+= text
  }
}

# ----------------------------------------------------------------------------
# - table section                                                            -
# ----------------------------------------------------------------------------

# the table command class
const afnix:adp:mantb (class)

# preset the table command
# @param clen the number of columns

trans afnix:adp:mantb:preset (clen) {
  const this:clen (clen:clone)
  trans this:cdat (String)
}

# write the command to an output stream
# @param os the output stream to write

trans afnix:adp:mantb:write (os) {
  # do nothing if nil
  if (this:cdat:nil-p) (return)
  # emit the table start
  os:newline
  os:writeln ".TS"
  # format the columns
  trans fmt (String)
  loop (trans i 0) (< i this:clen) (i:++) {
    if (not (fmt:nil-p)) (fmt:+= ' ')
    fmt:+= 'l'
  }
  fmt:+= '.'
  os:writeln fmt
  # emit the table data
  os:writeln this:cdat
  # emmit the table end
  os:writeln ".TE"
  os:write ".sp"
}

# add a new row in the table command

trans afnix:adp:mantb:new-row nil {
  if (not (this:cdat:nil-p)) (this:cdat:+= "\n")
}

# add a new head text in the table command
# @param text the text to add

trans afnix:adp:mantb:add-head (text) {
  # add a tab separator
  if (and (not (this:cdat:nil-p)) (!= (this:cdat:last) '\n')) {
    this:cdat:+= '\t'
  }
  # add the string
  this:cdat:+= (text:strip " \t\r\n")
}

# add a new data text in the table command
# @param text the text to add

trans afnix:adp:mantb:add-data (text) {
  # add a tab separator
  if (!= (this:cdat:last)'\n') (this:cdat:+= '\t')
  # add the string
  this:cdat:+= (text:strip " \t\r\n")
}

# ----------------------------------------------------------------------------
# - call section                                                             -
# ----------------------------------------------------------------------------

# the call command class
const afnix:adp:manfc (class)

# preset the call command

trans afnix:adp:manfc:preset nil {
  trans this:cnam (String)
  trans this:cret (String)
  trans this:carg (String)
  trans this:cdat (String)
}

# set the command name
# @param name the command name to set

trans afnix:adp:manfc:set-name (name) {
  trans this:cnam (name:clone)
}

# set the command return text
# @param text the return text to set

trans afnix:adp:manfc:set-retn (text) {
  trans this:cret (text:clone)
}

# set the command arguments
# @param text the arguments text to set

trans afnix:adp:manfc:set-args (text) {
  trans this:carg (text:clone)
}

# set the command data
# @param text the command data text

trans afnix:adp:manfc:set-data (text) {
  trans this:cdat (text:clone)
}

# write the command to an output stream
# @param os the output stream to write

trans afnix:adp:manfc:write (os) {
  # start by indenting
  os:newline
  os:writeln ".sp"
  os:writeln ".RS"
  # write the command name
  os:write ".B " this:cnam
  # write the return text if any
  if (not (this:cret:nil-p)) {
    os:write " -> "
    os:write this:cret
  }
  # write the arguments if any
  if (not (this:carg:nil-p)) {
    os:write " ("
    os:write this:carg
    os:write ')'
  }
  # write the command data
  if (not (this:cdat:nil-p)) {
    os:newline
    os:writeln ".br"
    os:write (this:cdat:strip " \t\r\n")
  }
  # stop indentation
  os:newline
  os:write ".RE"
}

# ----------------------------------------------------------------------------
# - option section                                                           -
# ----------------------------------------------------------------------------

# the option command class
const afnix:adp:manoc (class)

# preset the option command

trans afnix:adp:manoc:preset nil {
  trans this:cnam (String)
  trans this:carg (String)
  trans this:cdat (String)
}

# set the command name
# @param name the command name to set

trans afnix:adp:manoc:set-name (name) {
  trans this:cnam (name:clone)
}

# set the command arguments
# @param text the arguments text to set

trans afnix:adp:manoc:set-args (text) {
  trans this:carg (text:clone)
}

# set the command data
# @param text the command data text

trans afnix:adp:manoc:set-data (text) {
  trans this:cdat (text:clone)
}

# write the command to an output stream
# @param os the output stream to write

trans afnix:adp:manoc:write (os) {
  # start by a spacing
  os:newline
  os:writeln ".sp"
  # write the command name
  os:write ".B " (+ (+ "[" this:cnam) "]")
  # write the arguments if any
  if (not (this:carg:nil-p)) {
    os:write ' '
    os:write this:carg
  }
  # write the command data
  if (not (this:cdat:nil-p)) {
    os:newline
    os:writeln ".br"
    os:write (this:cdat:strip " \t\r\n")
  }
}
