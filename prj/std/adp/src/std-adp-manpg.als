# ----------------------------------------------------------------------------
# - std-adp-manpg                                                            -
# - afnix:adp nroff manual page module                                       -
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

# define the writer class
const afnix:adp:manpg (class)
# define the writer nameset
const AFNIX:ADP:MANPG (nameset AFNIX:ADP)

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the page commands
const AFNIX:ADP:MANPG:MANCC-HEAD-CMD ".TH"
const AFNIX:ADP:MANPG:MANCC-HSEC-CMD ".SH"
const AFNIX:ADP:MANPG:MANCC-BOLD-CMD ".B"
const AFNIX:ADP:MANPG:MANCC-ITAL-CMD ".I"
const AFNIX:ADP:MANPG:MANCC-PARA-CMD ".PP"
const AFNIX:ADP:MANPG:MANCC-SRID-CMD ".RS"
const AFNIX:ADP:MANPG:MANCC-ERID-CMD ".RE"
const AFNIX:ADP:MANPG:MANCC-NEWL-CMD ".br"
const AFNIX:ADP:MANPG:MANCC-LSPC-CMD ".sp"
const AFNIX:ADP:MANPG:MANCC-SALF-CMD ".nf"
const AFNIX:ADP:MANPG:MANCC-EALF-CMD ".fi"

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the writer environment

trans afnix:adp:manpg:preset nil {
  # set the header name
  trans this:hnam (String)
  # create the command vector
  const this:cvec (Vector)
}

# ----------------------------------------------------------------------------
# - method section                                                           -
# ----------------------------------------------------------------------------

# write the page content to a stream
# @param os the output stream to write
trans afnix:adp:manpg:write (os) {
  # write the header
  os:writeln "'\\\" t"
  os:writeln ".\\\" This documentation, which is part of the afnix writing"
  os:writeln ".\\\" system distribution, is free and can be distributed as"
  os:writeln ".\\\" long as this copyright notice is left intact."
  os:writeln ".\\\""
  os:writeln ".\\\" This documentation is distributed in the hope  that it"
  os:writeln ".\\\" will be useful, but without any warranty; without even"
  os:writeln ".\\\" the implied warranty  of  merchantability and  fitness"
  os:writeln ".\\\" for  a  particular  purpose.  In  no  event, shall the"
  os:writeln ".\\\" copyright holder be  liable  for  any direct, incident"
  os:writeln ".\\\" incidental or special damages arising in any way out"
  os:writeln ".\\\" of the use of this documentation."
  os:writeln ".\\\""
  os:writeln ".\\\" Copyright (c) 1999-2021 Amaury Darsch"
  os:write   ".\\\""
  # write the commands
  for (c) (this:cvec) (c:write os)
}

# add the page header by name section and text
# @param name the header name
# @param snum the section number
# @param desc the header description
trans afnix:adp:manpg:add-head (name snum desc) {
  # save the header name
  trans this:hnam (name:clone)
  # format the command text
  const text (name:clone)
  text:+= ' '
  text:+= snum
  text:+= ' '
  text:+= (afnix:adp:get-date)
  text:+= ' '
  text:+= afnix:adp:system-xsrc
  text:+= ' '
  text:+= (desc:to-literal)
  # create a new head command
  const mcc (afnix:adp:mancc AFNIX:ADP:MANPG:MANCC-HEAD-CMD text)
  # add the command
  this:cvec:add mcc
}

# add the page title
# @param title the title to set

trans afnix:adp:manpg:add-title (title) {
  # format the title text
  const text (this:hnam:to-lower)
  if (not (text:nil-p)) (text:+= " - ")
  text:+= (title:to-lower)
  # create a new section head command
  const mcc (afnix:adp:mancc AFNIX:ADP:MANPG:MANCC-HSEC-CMD "NAME")
  # add the command data
  mcc:set-data text
  # add the command
  this:cvec:add mcc
}

# add a preface to the page
trans afnix:adp:manpg:add-preface nil {
  # format the command name
  trans name "PREFACE"
  # create a new section head command
  const mcc (afnix:adp:mancc AFNIX:ADP:MANPG:MANCC-HSEC-CMD name)
  # add the command
  this:cvec:add mcc
}

# add a license to the page
trans afnix:adp:manpg:add-license nil {
  # format the command name
  trans name "LICENSE"
  # create a new section head command
  const mcc (afnix:adp:mancc AFNIX:ADP:MANPG:MANCC-HSEC-CMD name)
  # add the command
  this:cvec:add mcc
}

# add a chapter to the page
# @param title the chapter title
trans afnix:adp:manpg:add-chapter (title) {
  # format the command name
  trans name (title:to-upper)
  # create a new section head command
  const mcc (afnix:adp:mancc AFNIX:ADP:MANPG:MANCC-HSEC-CMD name)
  # add the command
  this:cvec:add mcc
}

# add a section to the page
# @param title the section title

trans afnix:adp:manpg:add-section (title) {
  # create a new section head command
  const mcc (afnix:adp:mancc AFNIX:ADP:MANPG:MANCC-BOLD-CMD title)
  mcc:set-head AFNIX:ADP:MANPG:MANCC-PARA-CMD
  mcc:set-data AFNIX:ADP:MANPG:MANCC-NEWL-CMD
  # add the command
  this:cvec:add mcc
}

# add a subsection to the page
# @param title the subsection title
trans afnix:adp:manpg:add-subsect (title) {
  # create a new section head command
  const mcc (afnix:adp:mancc AFNIX:ADP:MANPG:MANCC-ITAL-CMD title)
  mcc:set-head AFNIX:ADP:MANPG:MANCC-PARA-CMD
  mcc:set-data AFNIX:ADP:MANPG:MANCC-NEWL-CMD
  # add the command
  this:cvec:add mcc
}

# add a keyword to the page
# @param name the keyword name

trans afnix:adp:manpg:add-keyword (name) {
  # create a new head command
  const mcc (afnix:adp:mancc AFNIX:ADP:MANPG:MANCC-BOLD-CMD name)
  mcc:set-head AFNIX:ADP:MANPG:MANCC-PARA-CMD
  mcc:set-data AFNIX:ADP:MANPG:MANCC-NEWL-CMD
  # add the command
  this:cvec:add mcc
}

# add an object to the page
# @param name the object name
trans afnix:adp:manpg:add-object (name) {
  # create a new head command
  const mcc (afnix:adp:mancc AFNIX:ADP:MANPG:MANCC-BOLD-CMD name)
  mcc:set-head AFNIX:ADP:MANPG:MANCC-PARA-CMD
  mcc:set-data AFNIX:ADP:MANPG:MANCC-NEWL-CMD
  # add the command
  this:cvec:add mcc
}

# add a control to the page
# @param name the control name
trans afnix:adp:manpg:add-control (name) {
  # create a new head command
  const mcc (afnix:adp:mancc AFNIX:ADP:MANPG:MANCC-BOLD-CMD name)
  mcc:set-head AFNIX:ADP:MANPG:MANCC-PARA-CMD
  mcc:set-data AFNIX:ADP:MANPG:MANCC-NEWL-CMD
  # add the command
  this:cvec:add mcc
}

# add an example by text
# @param text the example text
trans afnix:adp:manpg:add-example (text) {
  # prepare text data
  const data (afnix:txt:indent (text:strip " \t\r\n") "{(" "})" "  ")
  # create a space command
  this:cvec:add (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-LSPC-CMD)
  # create the example command
  const expl (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-SALF-CMD)
  expl:set-data data
  # add the command
  this:cvec:add expl
  # add the formating end command
  this:cvec:add (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-EALF-CMD)
  this:cvec:add (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-LSPC-CMD)
}

# add an appendix to the page
# @param title the appendix title
trans afnix:adp:manpg:add-appendix (title) {
  # format the command name
  trans name (title:to-upper)
  # create a new section head command
  const mcc (afnix:adp:mancc AFNIX:ADP:MANPG:MANCC-HSEC-CMD name)
  # add the command
  this:cvec:add mcc
}

# add a reference title
# @param title the title to add
trans afnix:adp:manpg:add-ref-titl (title) { 
  # create a new title command
  const mcc (afnix:adp:mancc AFNIX:ADP:MANPG:MANCC-ITAL-CMD title)
  mcc:set-head AFNIX:ADP:MANPG:MANCC-PARA-CMD
  mcc:set-data AFNIX:ADP:MANPG:MANCC-NEWL-CMD
  # add the command
  this:cvec:add mcc
}

# add a reference subtitle
# @param title the subtitle to add
trans afnix:adp:manpg:add-ref-subt (title) {
  # add a reference title
  this:add-ref-titl title
}

# add a reference text
# @param text the reference text
trans afnix:adp:manpg:add-ref-text (text) {
  # add a spacing first
  this:cvec:add (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-LSPC-CMD)
  # create the command with relative indentation
  const rtxt (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-SRID-CMD)
  rtxt:set-data (text:strip " \t\r\n")
  # add the command
  this:cvec:add rtxt
  # finish indentation
  this:cvec:add (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-ERID-CMD)
}

# add a reference function call
trans afnix:adp:manpg:add-ref-call nil {
  # create a default function command
  const fc (afnix:adp:manfc)
  # add the command
  this:cvec:add fc
  # here is the command
  eval fc
}

# add a reference text
# @param text the reference text
trans afnix:adp:manpg:add-ref-text-sav (text) {
  this:cvec:add (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-LSPC-CMD)
  # create the  command
  const rtxt (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-SALF-CMD)
  rtxt:set-data (text:strip " \t\r\n")
  # add the command
  this:cvec:add rtxt
  # add the formating end command
  this:cvec:add (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-EALF-CMD)
  this:cvec:add (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-LSPC-CMD)
}

# add a reference syntax
# @param text the syntax text
trans afnix:adp:manpg:add-ref-synt (text) {
  # add a reference title
  this:add-ref-titl "Syntax"
  # add a reference text
  this:add-ref-text text
}

# add a reference example
# @param text the example text
trans afnix:adp:manpg:add-ref-expl (text) {
  # prepare text data
  const data (afnix:txt:indent (text:strip " \t\r\n") "{(" "})" "  ")
  # add a reference title
  this:add-ref-titl "Example"
  # add a reference text
  this:add-ref-text data
}

# add a reference predicate
# @param text the example text
trans afnix:adp:manpg:add-ref-pred (text) {
  # add a reference title
  this:add-ref-titl "Predicate"
  # add a reference text
  this:add-ref-text text
}

# add a reference inherit
# @param text the example text
trans afnix:adp:manpg:add-ref-inth (text) {
  # add a reference title
  this:add-ref-titl "Inheritance"
  # add a reference text
  this:add-ref-text text
}

# add a reference constants title
# @param text the example text
trans afnix:adp:manpg:add-ref-csts nil {
  # add a reference title
  this:add-ref-titl "Constants"
}

# add a reference constructors title
# @param text the example text
trans afnix:adp:manpg:add-ref-ctrs nil {
  # add a reference title
  this:add-ref-titl "Constructors"
}

# add a reference operators title
# @param text the example text
trans afnix:adp:manpg:add-ref-otrs nil {
  # add a reference title
  this:add-ref-titl "Operators"
}

# add a reference methods title
# @param text the example text
trans afnix:adp:manpg:add-ref-mths nil {
  # add a reference title
  this:add-ref-titl "Methods"
}

# add a reference functions title
# @param text the example text
trans afnix:adp:manpg:add-ref-fnts nil {
  # add a reference title
  this:add-ref-titl "Functions"
}

# add a reference synopsis
# @param text the example text
trans afnix:adp:manpg:add-ref-snps (text) {
  # add a reference title
  this:add-chapter "Synopsis"
  # add a reference text
  const para (afnix:adp:manpg:new-para)
  para:add-text text
}

# add a reference options
trans afnix:adp:manpg:add-ref-opts nil {
  # add a reference title
  this:add-chapter "Options"
}

# add a reference option
trans afnix:adp:manpg:add-ref-optn nil {
  # create a default option command
  const oc (afnix:adp:manoc)
  # add the command
  this:cvec:add oc
  # here is the command
  eval oc
}

# begin a new list by starting a relative indentation
trans afnix:adp:manpg:beg-list nil {
  # start a relative indent
  this:cvec:add (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-SRID-CMD)
}

# end a list by removing the relative indentation

trans afnix:adp:manpg:end-list nil {
  # end a relative indent
  this:cvec:add (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-ERID-CMD)
  this:cvec:add (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-LSPC-CMD)
}

# add a list item by reference and text
# @param iref the item reference 
# @param item the item text
trans afnix:adp:manpg:add-list-item (iref text) {
  # create a space command
  this:cvec:add (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-LSPC-CMD)
  # create item command
  const cref (afnix:adp:mancc AFNIX:ADP:MANPG:MANCC-BOLD-CMD iref)
  this:cvec:add cref
  # create the item text command
  const ctxt (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-NEWL-CMD)
  ctxt:set-data (text:strip " \t\r\n")
  # add the command
  this:cvec:add ctxt
}

# add a list item by text
# @param iref the item reference 
# @param item the item text
trans afnix:adp:manpg:add-list-elem (text) {
  # create the item text command
  const ctxt (afnix:adp:mansc AFNIX:ADP:MANPG:MANCC-NEWL-CMD)
  ctxt:set-data (text:strip " \t\r\n")
  # add the command
  this:cvec:add ctxt
}

# add a new paragraph
trans afnix:adp:manpg:new-para nil {
  # create a new text paragarph
  const para (afnix:adp:manpc)
  # add the command
  this:cvec:add para
  # here we are
  eval para
}

# add a new table
# @param clen the number of columns
trans afnix:adp:manpg:new-tabl (clen) {
  # create a new table command
  const tabl (afnix:adp:mantb clen)
  # add the command
  this:cvec:add tabl
  # here we are
  eval tabl
}
