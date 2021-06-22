# ----------------------------------------------------------------------------
# - std-adp-xhtut                                                            -
# - afnix:adp xhtml utilities classes module                                 -
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
# - private section                                                          -
# ----------------------------------------------------------------------------

# the afnix logo data
const AFNIX:ADP:IMG-LOGO-WTH "120"
const AFNIX:ADP:IMG-LOGO-HGT "120"

# the title table style
const AFNIX:ADP:TBL-TTDL-CLS "logo"
const AFNIX:ADP:TBL-TTDR-CLS "title"

# the div div element style
const AFNIX:ADP:TBL-DELM-CLS "elem"
# the div para element style
const AFNIX:ADP:TBL-PARA-CLS "text"

# ----------------------------------------------------------------------------
# - public section                                                           -
# ----------------------------------------------------------------------------

# add a title to the body node
# @param body the body node to format
# @param text the body title text to add

const afnix:adp:add-body-title (body text) {
  # create a new logo
  const img (afnix:wax:XhtmlImg afnix:adp:system-ximg afnix:adp:system-xdsc)
  img:set-geometry AFNIX:ADP:IMG-LOGO-WTH AFNIX:ADP:IMG-LOGO-HGT
  # create a new table
  const tbl (afnix:wax:XhtmlTable)
  # add a new row
  const tr (tbl:new-row)
  # add the left data
  const tdl (tr:new-data AFNIX:ADP:TBL-TTDL-CLS)
  tdl:add-child img
  # add the right data
  const tdr (tr:new-data AFNIX:ADP:TBL-TTDR-CLS)
  tdr:parse text
  # add the title to the body
  body:add-child tbl
}

# ----------------------------------------------------------------------------
# - para section                                                             -
# ----------------------------------------------------------------------------

# the paragraph class
const afnix:adp:xpara (class)

# preset the para environment
# @param cls the class style

trans afnix:adp:xpara:preset (cls) {
  # create a paragraph tag
  const this:super (afnix:wax:XhtmlPara cls)
}

# @return the paragraph node

trans afnix:adp:xpara:get-node nil {
  eval this:super
}

# add a text to the paragraph
# @param text the text to add

trans afnix:adp:xpara:add-text (text) {
  this:parse text
}

# add an emphasis to the paragraph
# @param text the text to add

trans afnix:adp:xpara:add-emph (text) {
  # create an emphasize node
  const node (afnix:wax:XhtmlEmph)
  node:add-child (afnix:xml:XmlText text)
  # add the node as a child
  this:add-child node
}

# add a tt text to the paragraph
# @param text the text to add

trans afnix:adp:xpara:add-ttxt (text) {
  # create a text tag
  const node (afnix:wax:XHtmlTxtt)
  # parse the text
  node:add-child (afnix:xml:XmlText text)
  # add the node as a child
  this:add-child node
}

# add a text package to the paragraph
# @param text the text to add

trans afnix:adp:xpara:add-pack (text) {
  # create a italic tag
  const node (afnix:wax:XhtmlTxti "package")
  # parse the text
  node:add-child (afnix:xml:XmlText text)
  # add the node as a child
  this:add-child node
}

# add a text product to the paragraph
# @param text the text to add

trans afnix:adp:xpara:add-prod (text) {
  # create a italic tag
  const node (afnix:wax:XhtmlTxti "product")
  # parse the text
  node:add-child (afnix:xml:XmlText text)
  # add the node as a child
  this:add-child node
}

# add a text command to the paragraph
# @param text the text to add

trans afnix:adp:xpara:add-tcmd (text) {
  # create a text tag
  const node (afnix:wax:XhtmlTxtt "command")
  # parse the text
  node:add-child (afnix:xml:XmlText text)
  # add the node as a child
  this:add-child node
}

# add a text option to the paragraph
# @param text the text to add

trans afnix:adp:xpara:add-topt (text) {
  # create a text tag
  const node (afnix:wax:XhtmlTxtt "option")
  # parse the text
  node:add-child (afnix:xml:XmlText text)
  # add the node as a child
  this:add-child node
}

# add a text file to the paragraph
# @param text the text to add

trans afnix:adp:xpara:add-file (text) {
  # create a text tag
  const node (afnix:wax:XhtmlTxtt "file")
  # parse the text
  node:add-child (afnix:xml:XmlText text)
  # add the node as a child
  this:add-child node
}

# add a text path to the paragraph
# @param text the text to add

trans afnix:adp:xpara:add-path (text) {
  # create a text tag
  const node (afnix:wax:XhtmlTxtt "path")
  # parse the text
  node:add-child (afnix:xml:XmlText text)
  # add the node as a child
  this:add-child node
}

# add a text extension to the paragraph
# @param text the text to add

trans afnix:adp:xpara:add-extn (text) {
  # create a text tag
  const node (afnix:wax:XhtmlTxtt "extn")
  # parse the text
  node:add-child (afnix:xml:XmlText text)
  # add the node as a child
  this:add-child node
}

# add a text code to the paragraph
# @param text the text to add

trans afnix:adp:xpara:add-code (text) {
  # create a text tag
  const node (afnix:wax:XhtmlTxtt "code")
  # parse the text
  node:add-child (afnix:xml:XmlText text)
  # add the node as a child
  this:add-child node
}

# add a hyperlink to the paragraph
# @param href the hyperlink to add
# @param text the text to add

trans afnix:adp:xpara:add-href (href text) {
  # create a hyperlink node
  const node (afnix:wax:XhtmlRef href text)
  # add the target attribute
  node:set-attribute "target" "_blank"
  # add the node as a child
  this:add-child node
}

# add a mail link to the paragraph
# @param addr the mail address to add
# @param text the text to add

trans afnix:adp:xpara:add-mail (addr text) {
  # create a uri text
  const href (+ "mailto:" addr)
  # create a hyperlink node
  const node (afnix:wax:XhtmlRef href text)
  # add the node as a child
  this:add-child node
}

# ----------------------------------------------------------------------------
# - table section                                                            -
# ----------------------------------------------------------------------------

# the table row class
const afnix:adp:xtr   (class)
# the table class
const afnix:adp:xtabl (class)

# preset the table row environment
# @param ndoe the row node to bind

trans afnix:adp:xtr:preset (node) {
  const this:super node
}

# @return the table row node

trans afnix:adp:xtr:get-node nil {
  eval this:super
}

# add a new table head to the row
# @param text the text to add

trans afnix:adp:xtr:new-head (text) {
  const th (this:super:new-head)
  th:parse text
}

# add a new table data to the row
# @param text the text to add

trans afnix:adp:xtr:new-data (text) {
  const td (this:super:new-data)
  td:parse text
}

# preset the table environment
# @param cls the class style

trans afnix:adp:xtabl:preset (cls) {
  # create a table tag
  const this:super (afnix:wax:XhtmlTable cls cls cls)
}

# @return the table node

trans afnix:adp:xtabl:get-node nil {
  eval this:super
}

# set the table caption
# @param text the caption text to set

trans afnix:adp:xtabl:set-caption (text) {
  const this:caption (text:clone)
}

# @return a new table row

trans afnix:adp:xtabl:new-row nil {
  afnix:adp:xtr (this:super:new-row)
}

# ----------------------------------------------------------------------------
# - list section                                                             -
# ----------------------------------------------------------------------------

# the list class
const afnix:adp:xlist (class)

# preset the list environment
# @param cls the class style

trans afnix:adp:xlist:preset (cls) {
  const this:super (afnix:wax:XhtmlUl cls)
}

# @return the list node

trans afnix:adp:xlist:get-node nil {
  eval this:super
}

# add a new item to the list
# @param ref the item reference to add
# @param txt the item text to add

trans afnix:adp:xlist:new-item-ref (ref txt) {
  # create a new item
  const li (this:super:new-item)
  # add the reference
  li:add-child (const txti (afnix:wax:XhtmlTxti "item"))
  txti:parse ref
  # add a break
  li:add-child (afnix:wax:XhtmlBr)
  # parse the text
  li:parse txt
}

# add a new item text to the list
# @param text the item text to add

trans afnix:adp:xlist:new-item-text (text) {
  # create a new item
  const li (this:super:new-item)
  # add the text
  li:parse text
}

# ----------------------------------------------------------------------------
# - list pre section                                                         -
# ----------------------------------------------------------------------------

# the list class
const afnix:adp:xlpre (class)

# preset the list pre environment
# @param cls the class style

trans afnix:adp:xlpre:preset (cls) {
  const this:super (afnix:wax:XhtmlPre cls)
}

# @return the list pre node

trans afnix:adp:xlpre:get-node nil {
  eval this:super
}

# add a new text in the list
# @param text the text to add

trans afnix:adp:xlpre:add-text-elem (text) {
  # create a new name element
  const elem (afnix:wax:XhtmlTxti)
  elem:parse (+ text "\n")
  # add the element as a child
  this:add-child elem
}

# add a new line in the list
# @param text the text to add

trans afnix:adp:xlpre:add-newline nil {
  this:add-child (afnix:xml:XmlText "\n")
}

# ----------------------------------------------------------------------------
# - list div section                                                         -
# ----------------------------------------------------------------------------

# the list class
const afnix:adp:xldiv (class)

# preset the list div environment
# @param cls the class style
trans afnix:adp:xldiv:preset (cls) {
  const this:super (afnix:wax:XhtmlDiv cls)
}

# @return the list div node
trans afnix:adp:xldiv:get-node nil {
  eval this:super
}

# add a new name element by text
# @param text the text to add
trans afnix:adp:xldiv:add-name-elem (text) {
  # create a new name element
  const elem (afnix:wax:XhtmlTxti)
  elem:add-child (afnix:xml:XmlText text)
  # add the element as a child
  this:add-child elem
}

# add a new argument element by text
# @param text the text to add
trans afnix:adp:xldiv:add-args-elem (text) {
  # create a new element
  const elem (afnix:wax:XhtmlTxti)
  elem:parse (+ (+ " (" text) ")")
  # add the element as a child
  this:add-child elem
}

# add a new return element by text
# @param text the text to add
trans afnix:adp:xldiv:add-retn-elem (text) {
  # create a new element
  const elem (afnix:wax:XhtmlTxti)
  elem:parse (+ (+ " (&#x2192; " text) ")")
  # add the element as a child
  this:add-child elem
}

# add a new text element by text
# @param text the text to add
trans afnix:adp:xldiv:add-para-elem (text) {
  # create a new ruler
  const hr (afnix:wax:XhtmlHr)
  this:add-child hr
  # create a new para element
  const para (afnix:wax:XhtmlPara AFNIX:ADP:TBL-PARA-CLS)
  para:add-child (afnix:xml:XmlText text)
  # add the element as a child
  this:add-child para
}

# ----------------------------------------------------------------------------
# - table div section                                                   -
# ----------------------------------------------------------------------------

# the table div class
const afnix:adp:xtdiv (class)

# preset the table div environment
# @param cls the class style
trans afnix:adp:xtdiv:preset (cls) {
  const this:super (afnix:wax:XhtmlDiv cls)
}

# @return the table div node
trans afnix:adp:xtdiv:get-node nil {
  eval this:super
}

# add a new name element by text
# @param text the text to add
trans afnix:adp:xtdiv:add-name-elem (text) {
  # create a new element
  const elem (afnix:wax:XhtmlTxtb)
  elem:parse text
  # add the element as a child
  this:add-child elem
}

# add a new argument element by text
# @param text the text to add
trans afnix:adp:xtdiv:add-args-elem (text) {
  # create a new element
  const elem (afnix:wax:XhtmlTxti)
  elem:parse (+ " " text)
  # add the element as a child
  this:add-child elem
}

# add a new text element by text
# @param text the text to add
trans afnix:adp:xtdiv:add-para-elem (text) {
  # create a new element
  const elem (afnix:wax:XhtmlTxti)
  elem:set-style "float:right"
  elem:parse text
  # add the element as a child
  this:add-child elem
}

