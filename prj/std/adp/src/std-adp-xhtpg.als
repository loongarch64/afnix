# ----------------------------------------------------------------------------
# - std-adp-xhtpg                                                            -
# - afnix:adp xhtml page module                                              -
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
const afnix:adp:xhtpg (class)
# define the writer nameset
const AFNIX:ADP:XHTPG (nameset AFNIX:ADP)

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the tag style classes
const AFNIX:ADP:XHTPG:XHTML-CHAP-CLS "chapter"
const AFNIX:ADP:XHTPG:XHTML-APDX-CLS "appendix"
const AFNIX:ADP:XHTPG:XHTML-SECT-CLS "section"
const AFNIX:ADP:XHTPG:XHTML-SUBS-CLS "subsect"
const AFNIX:ADP:XHTPG:XHTML-BBOX-CLS "bbox"
const AFNIX:ADP:XHTPG:XHTML-KEYW-CLS "elem"
const AFNIX:ADP:XHTPG:XHTML-OBJT-CLS "elem"
const AFNIX:ADP:XHTPG:XHTML-CTRL-CLS "elem"
const AFNIX:ADP:XHTPG:XHTML-EXPL-CLS "frame"
const AFNIX:ADP:XHTPG:XHTML-SYNT-CLS "frame"
const AFNIX:ADP:XHTPG:XHTML-PRED-CLS "frame"
const AFNIX:ADP:XHTPG:XHTML-INTH-CLS "frame"
const AFNIX:ADP:XHTPG:XHTML-CNST-CLS "frame"
const AFNIX:ADP:XHTPG:XHTML-CTOR-CLS "frame"
const AFNIX:ADP:XHTPG:XHTML-OPER-CLS "frame"
const AFNIX:ADP:XHTPG:XHTML-METH-CLS "frame"
const AFNIX:ADP:XHTPG:XHTML-FUNC-CLS "frame"
const AFNIX:ADP:XHTPG:XHTML-TREF-CLS "elem"
const AFNIX:ADP:XHTPG:XHTML-PARA-CLS "text"
const AFNIX:ADP:XHTPG:XHTML-TABL-CLS "text"
const AFNIX:ADP:XHTPG:XHTML-LIST-CLS "text"
const AFNIX:ADP:XHTPG:XHTML-SUBT-CLS "subt"

const AFNIX:ADP:XHTPG:XHTML-SNPS-CLS "frame"
const AFNIX:ADP:XHTPG:XHTML-OPTN-CLS "frame"

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the writer environment
trans afnix:adp:xhtpg:preset nil {
  # create a default page
  const this:super (afnix:wax:XhtmlRoot "no title")
  # set the body box
  trans this:bbox nil
  # the chapter counter
  trans this:ccnt 0
  # the section counter
  trans this:scnt 0
  # the subsection counter
  trans this:ucnt 0
  # preini the environment
  this:preini
}

# preini the writer environment
trans afnix:adp:xhtpg:preini nil {
  # set the charset
  this:set-charset
  # set the default style
  this:set-style afnix:adp:system-xcss
}

# ----------------------------------------------------------------------------
# - method section                                                           -
# ----------------------------------------------------------------------------

# add the head charset
trans afnix:adp:xhtpg:set-charset nil {
  # get the head node
  const head (this:get-head)
  # add the charset meta node
  head:add-charset
  # set the body box
  this:set-body-box
}

# set the page style by value
# @param value the page style to set
trans afnix:adp:xhtpg:set-style (value) {
  # get the head node
  const head (this:get-head)
  # set the page style
  head:add-style value
}

# set the body box
trans afnix:adp:xhtpg:set-body-box nil {
  # check for a body box
  if afnix:adp:system-nobd (return)
  # get the body node
  const body (this:get-body)
  # create a div box
  trans this:bbox (afnix:wax:XhtmlDiv AFNIX:ADP:XHTPG:XHTML-BBOX-CLS)
  # add the box to the body
  body:add-child this:bbox
}

# @return the body box
trans afnix:adp:xhtpg:get-body-box nil {
  if (nil-p this:bbox) (this:get-body) (eval this:bbox)
}

# set the page title by value
# @param title the title to set
trans afnix:adp:xhtpg:set-title (title) {
  # get the head node
  const head (this:get-head)
  # set the new title
  head:set-title title
  # get the body
  const body (this:get-body-box)
  # add the body title
  if (not afnix:adp:system-nohd) (afnix:adp:add-body-title body title)
}

# add meta information to the page head
# @param name the meta name
# @param info the meta info
trans afnix:adp:xhtpg:add-meta (name info) {
  # get the head node
  const head (this:get-head)
  # set the meta info
  head:add-meta name info
}

# add a preface to the body
trans afnix:adp:xhtpg:add-preface nil {
  # get the body node
  const body (this:get-body-box)
  # create div object by class
  const div (afnix:wax:XhtmlDiv AFNIX:ADP:XHTPG:XHTML-CHAP-CLS)
  # parse the title
  div:parse "Preface"
  # add a ruler
  body:add-child (afnix:wax:XhtmlHr)
  # add the chapter div
  body:add-child div
  # add a ruler
  body:add-child (afnix:wax:XhtmlHr)
}

# add a license to the body
trans afnix:adp:xhtpg:add-license nil {
  # get the body node
  const body (this:get-body-box)
  # create div object by class
  const div (afnix:wax:XhtmlDiv AFNIX:ADP:XHTPG:XHTML-CHAP-CLS)
  # parse the title
  div:parse "License"
  # add a ruler
  body:add-child (afnix:wax:XhtmlHr)
  # add the chapter div
  body:add-child div
  # add a ruler
  body:add-child (afnix:wax:XhtmlHr)
}

# add a chapter to the body
# @param title the chapter title
trans afnix:adp:xhtpg:add-chapter (title) {
  # get the body node
  const body (this:get-body-box)
  # create div object by class
  const div (afnix:wax:XhtmlDiv AFNIX:ADP:XHTPG:XHTML-CHAP-CLS)
  # parse the title
  div:parse (afnix:adp:format-full-title title (this:ccnt:++))
  trans this:scnt 0
  trans this:ucnt 0
  # add a ruler
  body:add-child (afnix:wax:XhtmlHr)
  # add the chapter div
  body:add-child div
  # add a ruler
  body:add-child (afnix:wax:XhtmlHr)
}

# add a section to the body
# @param title the section title
trans afnix:adp:xhtpg:add-section (title) {
  # get the body node
  const body (this:get-body-box)
  # create div object by class
  const div (afnix:wax:XhtmlDiv AFNIX:ADP:XHTPG:XHTML-SECT-CLS)
  # parse the title
  div:parse (afnix:adp:format-full-title title this:ccnt (this:scnt:++))
  trans this:ucnt 0
  # add the section div
  body:add-child div
}

# add a subsection to the body
# @param title the subsection title
trans afnix:adp:xhtpg:add-subsect (title) {
  # get the body node
  const body (this:get-body-box)
  # create subsection object by class
  const sub (afnix:wax:XhtmlPara AFNIX:ADP:XHTPG:XHTML-SUBS-CLS)
  # parse the title
  sub:parse (
    afnix:adp:format-full-title title this:ccnt this:scnt (this:ucnt:++))
  # add the subsection
  body:add-child sub
}

# add a keyword to the body
# @param name the keyword name to add
trans afnix:adp:xhtpg:add-keyword (name) {
  # get the body node
  const body (this:get-body-box)
  # create a reserved text
  const text (+ name " [reserved]")
  # create div object by class
  const div (afnix:wax:XhtmlDiv AFNIX:ADP:XHTPG:XHTML-KEYW-CLS)
  # parse the text
  div:parse text
  # add the keyword div
  body:add-child div
}

# add an object to the body
# @param name the object name to add
trans afnix:adp:xhtpg:add-object (name) {
  # get the body node
  const body (this:get-body-box)
  # add a simple para object
  body:add-child (afnix:wax:XhtmlPara true)
  # create div object by class
  const div (afnix:wax:XhtmlDiv AFNIX:ADP:XHTPG:XHTML-OBJT-CLS)
  # parse the name
  div:parse name
  # add the keyword div
  body:add-child div
}

# add a control to the body
# @param name the control name to add
trans afnix:adp:xhtpg:add-control (name) {
  # get the body node
  const body (this:get-body-box)
  # add a simple para object
  body:add-child (afnix:wax:XhtmlPara true)
  # create div object by class
  const div (afnix:wax:XhtmlDiv AFNIX:ADP:XHTPG:XHTML-CTRL-CLS)
  # parse the name
  div:parse name
  # add the keyword div
  body:add-child div
}

# add an appendix to the body
# @param title the chapter title
trans afnix:adp:xhtpg:add-appendix (title) {
  # get the body node
  const body (this:get-body-box)
  # create div object by class
  const div (afnix:wax:XhtmlDiv AFNIX:ADP:XHTPG:XHTML-APDX-CLS)
  # parse the title
  div:parse title
  # add a ruler
  body:add-child (afnix:wax:XhtmlHr)
  # add the chapter div
  body:add-child div
  # add a ruler
  body:add-child (afnix:wax:XhtmlHr)
}

# add a client to the body
# @param name the client name to add
trans afnix:adp:xhtpg:add-client (name) {
  # build the client text
  const text (+ name " [client]")
  # add a chapter header
  this:add-chapter text
}

# add a target to the body
# @param trgt the target name to add
trans afnix:adp:xhtpg:add-target (trgt) {
  # create an empty div node
  const div (afnix:wax:XhtmlDiv true)
  div:set-attribute "id" trgt
  # get the body node
  const body (this:get-body-box)
  body:add-child div
}

# add a new example
# @param text the example text
trans afnix:adp:xhtpg:add-example (text) {
  # prepare the text
  const data (+ (afnix:txt:indent text "{(" "})" "  ") "\n")
  # get the body node
  const body (this:get-body-box)
  # create example pre object
  const pre (afnix:wax:XhtmlPre AFNIX:ADP:XHTPG:XHTML-EXPL-CLS)
  # add the text
  pre:add-child (afnix:xml:XmlData data)
  # add the example
  body:add-child pre
}

# add a reference title
# @param text the title text to add
trans afnix:adp:xhtpg:add-ref-titl (text) {
  # get the body node
  const body (this:get-body-box)
  # create an identifier
  const ptl (afnix:wax:XhtmlPara AFNIX:ADP:XHTPG:XHTML-TREF-CLS)
  ptl:parse text
  body:add-child ptl
}

# add a reference subtitle
# @param text the subtitle text to add
trans afnix:adp:xhtpg:add-ref-subt (text) {
  # get the body node
  const body (this:get-body-box)
  # create a heading tag
  const hdt (afnix:wax:XhtmlH1 AFNIX:ADP:XHTPG:XHTML-SUBT-CLS)
  hdt:parse text
  body:add-child hdt
}

# add a reference example
# @param text the example text
trans afnix:adp:xhtpg:add-ref-expl (text) {
  # prepare the text
  const data (+ (afnix:txt:indent text "{(" "})" "  ") "\n")
  # add a reference title
  this:add-ref-titl "Example"
  # get the body node
  const body (this:get-body-box)
  # create a frame pre object
  const pre (afnix:wax:XhtmlPre AFNIX:ADP:XHTPG:XHTML-EXPL-CLS)
  # add the text
  pre:add-child (afnix:xml:XmlData data)
  # add the box
  body:add-child pre
}

# add a reference syntax
# @param text the syntax text
trans afnix:adp:xhtpg:add-ref-synt (text) {
  # add a reference title
  this:add-ref-titl "Syntax"
  # get the body node
  const body (this:get-body-box)
  # create a frame pre object
  const pre (afnix:wax:XhtmlPre AFNIX:ADP:XHTPG:XHTML-SYNT-CLS)
  # add the text
  pre:add-child (afnix:xml:XmlData (+ text "\n"))
  # add the box
  body:add-child pre
}

# add a reference predicate
# @param text the predicate text
trans afnix:adp:xhtpg:add-ref-pred (text) {
  # add a reference title
  this:add-ref-titl "Predicate"
  # get the body node
  const body (this:get-body-box)
  # create a frame pre object
  const pre (afnix:wax:XhtmlPre AFNIX:ADP:XHTPG:XHTML-PRED-CLS)
  # create a new name element
  const elem (afnix:wax:XhtmlTxti)
  elem:parse (+ text "\n\n")
  # add the element
  pre:add-child elem
  # add the box
  body:add-child pre
}

# add a reference inherit
trans afnix:adp:xhtpg:add-ref-inth nil {
  # add a reference title
  this:add-ref-titl "Inheritance"
  # get the body node
  const body (this:get-body-box)
  # create a new list pre
  const lpre (afnix:adp:xlpre AFNIX:ADP:XHTPG:XHTML-INTH-CLS)
  # add the node to the body
  body:add-child (lpre:get-node)
  # here we are
  eval lpre
}

# add a reference constants
trans afnix:adp:xhtpg:add-ref-csts nil {
  # add a reference title
  this:add-ref-titl "Constants"
}

# add a reference const
trans afnix:adp:xhtpg:add-ref-cnst nil {
  # get the body node
  const body (this:get-body-box)
  # create a new list div
  const ldiv (afnix:adp:xldiv AFNIX:ADP:XHTPG:XHTML-CNST-CLS)
  # add the node to the body
  body:add-child (ldiv:get-node)
  # add an empty paragraph
  body:add-child (afnix:wax:XhtmlPara true)
  # here we are
  eval ldiv
}

# add a reference constructors
trans afnix:adp:xhtpg:add-ref-ctrs nil {
  # add a reference title
  this:add-ref-titl "Constructors"
}

# add a reference ctor
trans afnix:adp:xhtpg:add-ref-ctor nil {
  # get the body node
  const body (this:get-body-box)
  # create a new list div
  const ldiv (afnix:adp:xldiv AFNIX:ADP:XHTPG:XHTML-CTOR-CLS)
  # add the node to the body
  body:add-child (ldiv:get-node)
  # add an empty paragraph
  body:add-child (afnix:wax:XhtmlPara true)
  # here we are
  eval ldiv
}

# add a reference operators
trans afnix:adp:xhtpg:add-ref-otrs nil {
  # add a reference title
  this:add-ref-titl "Operators"
}

# add a reference operator
trans afnix:adp:xhtpg:add-ref-oper nil {
  # get the body node
  const body (this:get-body-box)
  # create a new list div
  const ldiv (afnix:adp:xldiv AFNIX:ADP:XHTPG:XHTML-OPER-CLS)
  # add the node to the body
  body:add-child (ldiv:get-node)
  # add an empty paragraph
  body:add-child (afnix:wax:XhtmlPara true)
  # here we are
  eval ldiv
}

# add a reference methods
trans afnix:adp:xhtpg:add-ref-mths nil {
  # add a reference title
  this:add-ref-titl "Methods"
}

# add a reference meth
trans afnix:adp:xhtpg:add-ref-meth nil {
  # get the body node
  const body (this:get-body-box)
  # create a new list div
  const ldiv (afnix:adp:xldiv AFNIX:ADP:XHTPG:XHTML-METH-CLS)
  # add the node to the body
  body:add-child (ldiv:get-node)
  # add an empty paragraph
  body:add-child (afnix:wax:XhtmlPara true)
  # here we are
  eval ldiv
}

# add a reference functions
trans afnix:adp:xhtpg:add-ref-fnts nil {
  # add a reference title
  this:add-ref-titl "Functions"
}

# add a reference func
trans afnix:adp:xhtpg:add-ref-func nil {
  # get the body node
  const body (this:get-body-box)
  # create a new list div
  const ldiv (afnix:adp:xldiv AFNIX:ADP:XHTPG:XHTML-FUNC-CLS)
  # add the node to the body
  body:add-child (ldiv:get-node)
  # add an empty paragraph
  body:add-child (afnix:wax:XhtmlPara true)
  # here we are
  eval ldiv
}

# add a command synopsis
# @paran text the command text to add
trans afnix:adp:xhtpg:add-ref-snps (text) {
  # add a reference title
  this:add-ref-titl "Synopsis"
  # get the body node
  const body (this:get-body-box)
  # create a synopsis pre object
  const pre (afnix:wax:XhtmlPre AFNIX:ADP:XHTPG:XHTML-SNPS-CLS)
  # add the text
  pre:add-child (afnix:xml:XmlData (+ text "\n\n"))
  # add the synopsis
  body:add-child pre
}

# add a reference option
trans afnix:adp:xhtpg:add-ref-opts nil {
  # add a reference title
  this:add-ref-titl "Options"
}

# add a reference optn
trans afnix:adp:xhtpg:add-ref-optn nil {
  # get the body node
  const body (this:get-body-box)
  # create a new table div
  const tdiv (afnix:adp:xtdiv AFNIX:ADP:XHTPG:XHTML-OPTN-CLS)
  # add the node to the body
  body:add-child (tdiv:get-node)
  # add an empty paragraph
  body:add-child (afnix:wax:XhtmlPara true)
  # here we are
  eval tdiv
}

# add a new paragraph
trans afnix:adp:xhtpg:new-para nil {
  # create a new text paragarph
  const para (afnix:adp:xpara AFNIX:ADP:XHTPG:XHTML-PARA-CLS)
  # get the body node
  const body (this:get-body-box)
  # add the node to the body
  body:add-child (para:get-node)
  # here we are
  eval para
}

# add a new table
trans afnix:adp:xhtpg:new-tabl nil {
  # create a new table
  const tabl (afnix:adp:xtabl AFNIX:ADP:XHTPG:XHTML-TABL-CLS)
  # get the body node
  const body (this:get-body-box)
  # add the node to the body
  body:add-child (tabl:get-node)
  body:add-child (afnix:wax:XhtmlPara true)
  # here we are
  eval tabl
}

# add a new list
trans afnix:adp:xhtpg:new-list nil {
  # create a new list
  const list (afnix:adp:xlist AFNIX:ADP:XHTPG:XHTML-LIST-CLS)
  # get the body node
  const body (this:get-body-box)
  # add the node to the body
  body:add-child (list:get-node)
  # here we are
  eval list
}
