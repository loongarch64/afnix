# ----------------------------------------------------------------------------
# - std-adp-texpg                                                            -
# - afnix:adp latex page module                                              -
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
const afnix:adp:texpg (class)
# define the writer nameset
const AFNIX:ADP:TEXPG (nameset AFNIX:ADP)

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the page commands
const AFNIX:ADP:TEXPG:NRM-CHAP-CMD "chapter"
const AFNIX:ADP:TEXPG:NRM-SECT-CMD "section"
const AFNIX:ADP:TEXPG:NRM-SUBS-CMD "subsection"
const AFNIX:ADP:TEXPG:NRM-PARS-CMD "parsection"
const AFNIX:ADP:TEXPG:NRM-NEWP-CMD "newpage"
const AFNIX:ADP:TEXPG:ENV-ITMZ-CMD "itemize"
const AFNIX:ADP:TEXPG:NRM-ITEM-CMD "item"

# the page contents
const AFNIX:ADP:TEXPG:NRM-PREF-CNT "PREFACE"
const AFNIX:ADP:TEXPG:NRM-LICS-CNT "LICENSE"

const AFNIX:ADP:TEXPG:NRM-SYNT-CNT "Syntax"
const AFNIX:ADP:TEXPG:NRM-EXPL-CNT "Example"
const AFNIX:ADP:TEXPG:NRM-MTHS-CNT "Methods"
const AFNIX:ADP:TEXPG:NRM-CSTS-CNT "Constants"
const AFNIX:ADP:TEXPG:NRM-PRED-CNT "Predicate"
const AFNIX:ADP:TEXPG:NRM-OTRS-CNT "Operators"
const AFNIX:ADP:TEXPG:NRM-FNTS-CNT "Functions"
const AFNIX:ADP:TEXPG:NRM-INTH-CNT "Inheritance"
const AFNIX:ADP:TEXPG:NRM-CTRS-CNT "Constructors"

const AFNIX:ADP:TEXPG:NRM-SNPS-CNT "Synopsis"
const AFNIX:ADP:TEXPG:NRM-OPTS-CNT "Options"

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the writer environment
trans afnix:adp:texpg:preset nil {
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
trans afnix:adp:texpg:write (os) {
  # write the header
  os:writeln "% This documentation, which is part of the afnix writing"
  os:writeln "% system distribution, is free and can be distributed as"
  os:writeln "% long as this copyright notice is left intact."
  os:writeln "%"
  os:writeln "% This documentation is distributed in the hope  that it"
  os:writeln "% will be useful, but without any warranty; without even"
  os:writeln "% the implied warranty  of  merchantability and  fitness"
  os:writeln "% for  a  particular  purpose.  In  no  event, shall the"
  os:writeln "% copyright holder be  liable  for  any direct, incident"
  os:writeln "% incidental or special damages arising in any way out"
  os:writeln "% of the use of this documentation."
  os:writeln "%"
  os:writeln "% Copyright (c) 1999-2021 Amaury Darsch"
  # write the commands
  for (c) (this:cvec) (c:write os)
}

# add the page header by name section and text
# @param name the header name
# @param snum the section number
# @param desc the header description
trans afnix:adp:texpg:add-head (name snum desc) {
  const this:hnam (name:clone)
  const this:hnum (snum:clone)
  const this:hdes (desc:clone)
}

# add the page title
# @param title the title to set
trans afnix:adp:texpg:add-title (title) {
  const this:htit (title:clone)
}

# add a preface to the page
trans afnix:adp:texpg:add-preface nil {
  this:add-chapter AFNIX:ADP:TEXPG:NRM-PREF-CNT
}

# add a license to the page
trans afnix:adp:texpg:add-license nil {
  this:add-chapter AFNIX:ADP:TEXPG:NRM-LICS-CNT
}

# add a chapter to the page
# @param title the chapter title
trans afnix:adp:texpg:add-chapter (title) {
  # create a new chapter command
  const tsc (afnix:adp:texcc AFNIX:ADP:TEXPG:NRM-CHAP-CMD title)
  # add the command
  this:cvec:add tsc
}

# add an appendix to the page
# @param title the appendix title
trans afnix:adp:texpg:add-appendix (title) {
  # create a new chapter command
  const tsc (afnix:adp:texcc AFNIX:ADP:TEXPG:NRM-CHAP-CMD title)
  # add the command
  this:cvec:add tsc
}

# add a section to the page
# @param title the section title
trans afnix:adp:texpg:add-section (title) {
  # create a new section command
  const tsc (afnix:adp:texcc AFNIX:ADP:TEXPG:NRM-SECT-CMD title)
  # add the command
  this:cvec:add tsc
}

# add a subsection to the page
# @param title the subsection title
trans afnix:adp:texpg:add-subsect (title) {
  # create a new subsection command
  const tsc (afnix:adp:texcc AFNIX:ADP:TEXPG:NRM-SUBS-CMD title)
  # add the command
  this:cvec:add tsc
}

# add a parsection to the page
# @param title the parsection title
trans afnix:adp:texpg:add-parsect (title) {
  # create a new parsection command
  const tsc (afnix:adp:texcc AFNIX:ADP:TEXPG:NRM-PARS-CMD title)
  # add the command
  this:cvec:add tsc
}

# add a new page command
trans afnix:adp:texpg:add-newpage nil {
  # add a newpage command
  const nwpc (afnix:adp:texsc AFNIX:ADP:TEXPG:NRM-NEWP-CMD)
  this:cvec:add nwpc
}

# add a new keyword by name
# @param keyw the keyword name to add
trans afnix:adp:texpg:add-keyword (keyw) {
  # add a new page
  this:add-newpage
  # add a new section
  this:add-section (+ "Keyword " keyw)
}

# add a new object by name
# @param onam the object name to add
trans afnix:adp:texpg:add-object (onam) {
  # add a new page
  this:add-newpage
  # add a new section
  this:add-section (+ "Object " onam)
}

# add a reference control
# @param name the control name
trans afnix:adp:texpg:add-control (name) {
  # create a default control command
  const ct (afnix:adp:texct name)
  # add the command
  this:cvec:add ct
}

# add a new client by name
# @param cltn the client name to add
trans afnix:adp:texpg:add-client (cltn) {
  # add a new chapter
  this:add-chapter cltn
}

# add a new paragraph
trans afnix:adp:texpg:new-para nil {
  # create a new text paragarph
  const para (afnix:adp:texpc)
  # add the command
  this:cvec:add para
  # here we are
  eval para
}

# add an example by text
# @param text the example text
trans afnix:adp:texpg:add-example (text) {
  # prepare text data
  const data (afnix:txt:indent (text:strip " \t\r\n") "{(" "})" "  ")
  # create the example command
  const rbox (afnix:adp:texrb data)
  # add the command
  this:cvec:add rbox
}

# add a keyword example by text
# @param text the example text
trans afnix:adp:texpg:add-keyw-expl (text) {
  # add a title section
  this:add-subsect AFNIX:ADP:TEXPG:NRM-EXPL-CNT
  # add the example
  this:add-example text
}

# add a syntax by text
# @param text the example text
trans afnix:adp:texpg:add-ref-synt (text) {
  # add a title section
  this:add-subsect AFNIX:ADP:TEXPG:NRM-SYNT-CNT
  # add the example
  this:add-example text
}

# add a predicate by text
# @param text the example text
trans afnix:adp:texpg:add-ref-pred (text) {
  # add a title section
  this:add-subsect AFNIX:ADP:TEXPG:NRM-PRED-CNT
  # add the text as a list
  this:beg-list
  this:add-list-elem text
  this:end-list
}

# add an inheritance by text
# @param text the example text
trans afnix:adp:texpg:add-ref-inth (text) {
  # add a title section
  this:add-subsect AFNIX:ADP:TEXPG:NRM-INTH-CNT
  this:beg-list
  this:add-list-elem text
  this:end-list
}

# add a csts reference
trans afnix:adp:texpg:add-ref-csts nil {
  # add a title section
  this:add-subsect AFNIX:ADP:TEXPG:NRM-CSTS-CNT
}

# add a ctrs reference
trans afnix:adp:texpg:add-ref-ctrs nil {
  # add a title section
  this:add-subsect AFNIX:ADP:TEXPG:NRM-CTRS-CNT
}

# add a reference operators title
trans afnix:adp:texpg:add-ref-otrs nil {
  # add a title section
  this:add-subsect AFNIX:ADP:TEXPG:NRM-OTRS-CNT
}

# add a methods reference
trans afnix:adp:texpg:add-ref-mths nil {
  # add a title section
  this:add-subsect AFNIX:ADP:TEXPG:NRM-MTHS-CNT
}

# add a function reference
trans afnix:adp:texpg:add-ref-fnts nil {
  # add a title section
  this:add-subsect AFNIX:ADP:TEXPG:NRM-FNTS-CNT
}

# add a reference function call
trans afnix:adp:texpg:add-ref-call nil {
  # create a default function call command
  const fc (afnix:adp:texfc)
  # add the command
  this:cvec:add fc
  # here is the command
  eval fc
}

# add a synopsys reference
# @param text the reference text
trans afnix:adp:texpg:add-ref-snps (text) {
  # add a title section
  this:add-subsect AFNIX:ADP:TEXPG:NRM-SNPS-CNT
  # add the example
  this:add-example text
}

# add a opts reference
trans afnix:adp:texpg:add-ref-opts nil {
  # add a title section
  this:add-subsect AFNIX:ADP:TEXPG:NRM-OPTS-CNT
}

# add a reference option

trans afnix:adp:texpg:add-ref-optn nil {
  # create a default option command
  const co (afnix:adp:texco)
  # add the command
  this:cvec:add co
  # here is the command
  eval co
}

# begin a new list by starting a relative indentation
trans afnix:adp:texpg:beg-list nil {
  # start an itemize command
  this:cvec:add (afnix:adp:texcc "begin" AFNIX:ADP:TEXPG:ENV-ITMZ-CMD)
}

# end a list by removing the relative indentation
trans afnix:adp:texpg:end-list nil {
  # end an itemize command
  this:cvec:add (afnix:adp:texcc "end" AFNIX:ADP:TEXPG:ENV-ITMZ-CMD)
}

# add a list item by reference and text
# @param iref the item reference 
# @param item the item text
trans afnix:adp:texpg:add-list-item (iref text) {
  # create item command
  const itmc (afnix:adp:texsc AFNIX:ADP:TEXPG:NRM-ITEM-CMD)
  this:cvec:add itmc
  # create the data text
  const data (+ (+ "\\textbf{" iref) "}\\\\\n")
  data:+= (text:strip " \t\r\n")
  # set the command data
  itmc:set-data data
}

# add a list item by text
# @param iref the item reference 
# @param item the item text
trans afnix:adp:texpg:add-list-elem (text) {
  # create item command
  const itmc (afnix:adp:texsc AFNIX:ADP:TEXPG:NRM-ITEM-CMD)
  this:cvec:add itmc
  # set the command data
  itmc:set-data (text:strip " \t\r\n")
}

# add a new table
# @param clen the number of columns
trans afnix:adp:texpg:new-tabl (clen) {
  # create a new table command
  const tabl (afnix:adp:textb clen)
  # add the command
  this:cvec:add tabl
  # here we are
  eval tabl
}
