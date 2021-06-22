# ----------------------------------------------------------------------------
# - std-adp-texwr                                                            -
# - afnix:adp latex writer module                                            -
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
const afnix:adp:texwr (class)
# make the context class as an inferent class
const afnix:adp:texwr:infer afnix:adp:ctxdb

# ----------------------------------------------------------------------------
# - private section                                                          -
# ----------------------------------------------------------------------------

# the revision information
const AFNIX:ADP:XML-TAG-RMAJ  (interp:major-version:clone)
const AFNIX:ADP:XML-TAG-RMIN  (interp:minor-version:clone)
const AFNIX:ADP:XML-TAG-RPAT  (interp:patch-version:clone)

# the documentation tags
const AFNIX:ADP:XML-TAG-DTDN  "axdoc"
const AFNIX:ADP:XML-TAG-INFO  "info"
const AFNIX:ADP:XML-TAG-BODY  "body"
const AFNIX:ADP:XML-TAG-FRNT  "front"
const AFNIX:ADP:XML-TAG-MAIN  "main"
const AFNIX:ADP:XML-TAG-BACK  "back"

const AFNIX:ADP:XML-TAG-HEAD  "header"
const AFNIX:ADP:XML-TAG-DESC  "desc"

const AFNIX:ADP:XML-TAG-PREF  "preface"
const AFNIX:ADP:XML-TAG-LICS  "license"
const AFNIX:ADP:XML-TAG-CHAP  "chapter"
const AFNIX:ADP:XML-TAG-APDX  "appendix"
const AFNIX:ADP:XML-TAG-SECT  "section"
const AFNIX:ADP:XML-TAG-SUBS  "subsect"

const AFNIX:ADP:XML-TAG-KEYW  "keyword"
const AFNIX:ADP:XML-TAG-OBJT  "object"
const AFNIX:ADP:XML-TAG-CTRL  "control"

const AFNIX:ADP:XML-TAG-PRED  "pred"
const AFNIX:ADP:XML-TAG-INTH  "inherit"
const AFNIX:ADP:XML-TAG-CTRS  "ctors"
const AFNIX:ADP:XML-TAG-OTRS  "otors"
const AFNIX:ADP:XML-TAG-MTHS  "methods"
const AFNIX:ADP:XML-TAG-FNTS  "functions"
const AFNIX:ADP:XML-TAG-CSTS  "constants"

const AFNIX:ADP:XML-TAG-CTOR  "ctor"
const AFNIX:ADP:XML-TAG-OPER  "oper"
const AFNIX:ADP:XML-TAG-METH  "meth"
const AFNIX:ADP:XML-TAG-FUNC  "func"
const AFNIX:ADP:XML-TAG-CNST  "const"

const AFNIX:ADP:XML-TAG-PARA  "p"
const AFNIX:ADP:XML-TAG-NAME  "name"
const AFNIX:ADP:XML-TAG-ARGS  "args"
const AFNIX:ADP:XML-TAG-RETN  "retn"
const AFNIX:ADP:XML-TAG-TITL  "title"
const AFNIX:ADP:XML-TAG-SYNT  "syntax"
const AFNIX:ADP:XML-TAG-EXPL  "example"

const AFNIX:ADP:XML-TAG-LIST  "list"
const AFNIX:ADP:XML-TAG-ITEM  "item"

const AFNIX:ADP:XML-TAG-TABL  "table"
const AFNIX:ADP:XML-TAG-TBLR  "tr"
const AFNIX:ADP:XML-TAG-TBLH  "th"
const AFNIX:ADP:XML-TAG-TBLD  "td"

const AFNIX:ADP:XML-TAG-CLNT  "client"
const AFNIX:ADP:XML-TAG-SNPS  "synopsis"
const AFNIX:ADP:XML-TAG-OPTS  "options"
const AFNIX:ADP:XML-TAG-OPTN  "optn"
const AFNIX:ADP:XML-TAG-RMRK  "remark"

# the header section attribute
const AFNIX:ADP:XML-HEAD-SNUM "section"
# the list ref attribute
const AFNIX:ADP:XML-LIST-AREF "ref"

# ----------------------------------------------------------------------------
# - initial section                                                          -
# ----------------------------------------------------------------------------

# preset the context environment

trans afnix:adp:texwr:preset nil {
  # bind the super class
  const this:super (afnix:adp:xmlnp)
  # preini the environment
  this:preini
  # postdo the environment
  this:postdo
}

# preini the context environment

trans afnix:adp:texwr:preini nil {
  # set the writer mode
  const this:mode AFNIX:ADP:WRITER-MODE:TEX
  # create the latex page
  const this:page (afnix:adp:texpg)
}

# postdo the context environment

trans afnix:adp:texwr:postdo nil {
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-DTDN this:expands-cnod
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-INFO this:process-ctag-info
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-BODY this:expands-cnod
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-FRNT this:expands-cnod
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-MAIN this:expands-cnod
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-BACK this:expands-cnod
  
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-PREF this:process-ctag-pref
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-LICS this:process-ctag-lics
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-CHAP this:expands-cnod
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-APDX this:expands-cnod
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-SECT this:expands-cnod
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-SUBS this:expands-cnod

  this:add-dispatch-tag AFNIX:ADP:XML-TAG-KEYW this:expands-cnod
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-OBJT this:expands-cnod
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-CTRL this:expands-cnod

  this:add-dispatch-tag AFNIX:ADP:XML-TAG-PRED this:process-ctag-pred
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-INTH this:process-ctag-inth
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-CSTS this:process-ctag-csts
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-CTRS this:process-ctag-ctrs
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-OTRS this:process-ctag-otrs
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-MTHS this:process-ctag-mths
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-FNTS this:process-ctag-fnts

  this:add-dispatch-tag AFNIX:ADP:XML-TAG-SNPS this:process-ctag-snps

  this:add-dispatch-tag AFNIX:ADP:XML-TAG-PARA this:process-ctag-para
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-TITL this:process-ctag-titl
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-NAME this:process-ctag-name
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-SYNT this:process-ctag-synt
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-EXPL this:process-ctag-expl
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-LIST this:process-ctag-list
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-TABL this:process-ctag-tabl

  this:add-dispatch-tag AFNIX:ADP:XML-TAG-CLNT this:expands-cnod
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-OPTS this:process-ctag-opts
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-OPTN this:process-ctag-optn
  this:add-dispatch-tag AFNIX:ADP:XML-TAG-RMRK this:expands-cnod
}

# ----------------------------------------------------------------------------
# - method section                                                           -
# ----------------------------------------------------------------------------

# write the result page

trans afnix:adp:texwr:write nil {
  # create an output file
  const os (afnix:sio:OutputFile afnix:adp:system-onam "UTF-8")
  # write the page
  this:page:write os
  # close the file
  os:close
}

# process the source info header
# @param node the node to process

trans afnix:adp:texwr:process-ctag-info-head (node) {
  # make sure we have the info node
  assert AFNIX:ADP:XML-TAG-INFO (node:get-name)
  # check if we have a header node
  if (not (node:child-p AFNIX:ADP:XML-TAG-HEAD)) (return)
  # get the header node
  const hnod (node:lookup-child AFNIX:ADP:XML-TAG-HEAD)
  const stxt (hnod:get-attribute-value AFNIX:ADP:XML-HEAD-SNUM)
  # get the name/desc nodes
  const nnod (hnod:lookup-child AFNIX:ADP:XML-TAG-NAME)
  const dnod (hnod:lookup-child AFNIX:ADP:XML-TAG-DESC)
  # get the header date
  const ntxt (nnod:to-text)
  const dtxt (dnod:to-text)
  # set the author/copy(right)(left) meta
  this:page:add-head ntxt stxt dtxt
}

# process the preface info header
# @param node the node to process

trans afnix:adp:texwr:process-ctag-pref (node) {
  # make sure we have the preface node
  assert AFNIX:ADP:XML-TAG-PREF (node:get-name)
  # add a preface to the page
  this:page:add-preface
  # process the node
  this:expands-cnod node
}

# process the license info header
# @param node the node to process

trans afnix:adp:texwr:process-ctag-lics (node) {
  # make sure we have the license node
  assert AFNIX:ADP:XML-TAG-LICS (node:get-name)
  # add a license to the page
  this:page:add-license
  # process the node
  this:expands-cnod node
}

# process the source info title
# @param node the node to process

trans afnix:adp:texwr:process-ctag-info-titl (node) {
  # make sure we have the info node
  assert AFNIX:ADP:XML-TAG-INFO (node:get-name)
  # get the title node and value
  const tnod (node:lookup-child AFNIX:ADP:XML-TAG-TITL)
  const tval (tnod:to-text)
  # set the page title
  this:page:add-title tval
}

# process the source name tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-name (node) {
  # make sure we have the name tag
  assert AFNIX:ADP:XML-TAG-NAME (node:get-name)
  # dispatch from the context
  const pnod (node:get-parent)
  const pnam (pnod:get-name)
  # process the keyword name
  if (== pnam AFNIX:ADP:XML-TAG-KEYW) {
    this:page:add-keyword (node:to-text)
    return
  }
  # process the object name
  if (== pnam AFNIX:ADP:XML-TAG-OBJT) {
    this:page:add-object (node:to-text)
    return
  }
  # process the control name
  if (== pnam AFNIX:ADP:XML-TAG-CTRL) {
    this:page:add-control (node:to-text)
    return
  }
  # process the client name
  if (== pnam AFNIX:ADP:XML-TAG-CLNT) {
    this:page:add-client (node:to-text)
    return
  }
  # unknown name context
  errorln "[texwr] no handler for name " (node:to-text)
}

# process the source info tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-info (node) {
  # process the header
  this:process-ctag-info-head node
  # process the title
  this:process-ctag-info-titl node
}

# process the source title tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-titl (node) {
  # make sure we have the title tag
  assert AFNIX:ADP:XML-TAG-TITL (node:get-name)
  # dispatch from the context
  const pnod (node:get-parent)
  const pnam (pnod:get-name)
  # process chapter title
  if (== pnam AFNIX:ADP:XML-TAG-CHAP) {
    this:page:add-chapter (node:to-text)
    return
  }
  # process section title
  if (== pnam AFNIX:ADP:XML-TAG-SECT) {
    this:page:add-section (node:to-text)
    return
  }
  # process subsection title
  if (== pnam AFNIX:ADP:XML-TAG-SUBS) {
    this:page:add-subsect (node:to-text)
    return
  }
  # process appendix title
  if (== pnam AFNIX:ADP:XML-TAG-APDX) {
    this:page:add-appendix (node:to-text)
    return
  }
  # process remark title
  if (== pnam AFNIX:ADP:XML-TAG-RMRK) {
    this:page:add-parsect (node:to-text)
    return
  }
  # unknown title context
  errorln "[texwr] no handler for title " (node:to-text)
}

# process the source example tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-expl (node) {
  # make sure we have the example tag
  assert AFNIX:ADP:XML-TAG-EXPL (node:get-name)
  # dispatch from the context
  const pnod (node:get-parent)
  const pnam (pnod:get-name)
  # process keyword example
  if (== pnam AFNIX:ADP:XML-TAG-KEYW) {
    this:page:add-keyw-expl (afnix:xml:pre-normal (node:to-text)) 
    return
  }
  # add the general example
  this:page:add-example (afnix:xml:pre-normal (node:to-text)) 
}

# process the source syntax tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-synt (node) {
  # make sure we have the syntax tag
  assert AFNIX:ADP:XML-TAG-SYNT (node:get-name)
  # add the syntax description
  this:page:add-ref-synt (afnix:xml:pre-normal (node:to-text)) 
}

# process the source pred tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-pred (node) {
  # make sure we have the pred tag
  assert AFNIX:ADP:XML-TAG-PRED (node:get-name)
  # add the pred description
  this:page:add-ref-pred (afnix:xml:pre-normal (node:to-text)) 
}

# process the source inherit tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-inth (node) {
  # make sure we have the inth tag
  assert AFNIX:ADP:XML-TAG-INTH (node:get-name)
  # add the inth description
  this:page:add-ref-inth (afnix:xml:pre-normal (node:to-text)) 
}

# process the source csts tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-csts (node) {
  # make sure we have the name tag
  assert AFNIX:ADP:XML-TAG-CSTS (node:get-name)
  # add a title reference
  this:page:add-ref-csts
  # start a new list
  this:page:beg-list
  # loop into the children
  const clen (node:child-length)
  loop (trans i 0) (< i clen) (i:++) {
    # get the node
    trans cnod (node:get-child i)
    trans cnam (cnod:get-name)
    switch cnam (
      (AFNIX:ADP:XML-TAG-CNST (this:process-ctag-cnst cnod))
      (else (errorln "[texwr] no handler for constants tag " cnam))
    )
  }
  # end the list
  this:page:end-list   
}

# process the source ctors tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-ctrs (node) {
  # make sure we have the name tag
  assert AFNIX:ADP:XML-TAG-CTRS (node:get-name)
  # add a title reference
  this:page:add-ref-ctrs
  # start a new list
  this:page:beg-list
  # loop into the children
  const clen (node:child-length)
  loop (trans i 0) (< i clen) (i:++) {
    # get the node
    trans cnod (node:get-child i)
    trans cnam (cnod:get-name)
    switch cnam (
      (AFNIX:ADP:XML-TAG-CTOR (this:process-ctag-ctor cnod))
      (else (errorln "[texwr] no handler for ctors tag " cnam))
    )
  }
  # end the list
  this:page:end-list
}

# process the source operators tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-otrs (node) {
  # make sure we have the name tag
  assert AFNIX:ADP:XML-TAG-OTRS (node:get-name)
  # add a title reference
  this:page:add-ref-otrs
  # start a new list
  this:page:beg-list
  # loop into the children
  const clen (node:child-length)
  loop (trans i 0) (< i clen) (i:++) {
    # get the node
    trans cnod (node:get-child i)
    trans cnam (cnod:get-name)
    switch cnam (
      (AFNIX:ADP:XML-TAG-OPER (this:process-ctag-oper cnod))
      (else (errorln "[texwr] no handler for otors tag " cnam))
    )
  }
  # end the list
  this:page:end-list
}

# process the source methods tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-mths (node) {
  # make sure we have the name tag
  assert AFNIX:ADP:XML-TAG-MTHS (node:get-name)
  # add a title reference
  this:page:add-ref-mths
  # start a new list
  this:page:beg-list
  # loop into the children
  const clen (node:child-length)
  loop (trans i 0) (< i clen) (i:++) {
    # get the node
    trans cnod (node:get-child i)
    trans cnam (cnod:get-name)
    switch cnam (
      (AFNIX:ADP:XML-TAG-METH (this:process-ctag-meth cnod))
      (else (errorln "[texwr] no handler for methods tag " cnam))
    )
  }
  # end the list
  this:page:end-list
}

# process the source functions tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-fnts (node) {
  # make sure we have the name tag
  assert AFNIX:ADP:XML-TAG-FNTS (node:get-name)
  # add a title reference
  this:page:add-ref-fnts
  # start a new list
  this:page:beg-list
  # loop into the children
  const clen (node:child-length)
  loop (trans i 0) (< i clen) (i:++) {
    # get the node
    trans cnod (node:get-child i)
    trans cnam (cnod:get-name)
    switch cnam (
      (AFNIX:ADP:XML-TAG-FUNC (this:process-ctag-func cnod))
      (else (errorln "[texwr] no handler for functions tag " cnam))
    )
  }
  # end the list
  this:page:end-list
}

# process the source cnst tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-cnst (node) {
  # make sure we have the cnst tag
  assert AFNIX:ADP:XML-TAG-CNST (node:get-name)
  # create a new call command
  this:process-ctag-call node
}

# process the source ctor tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-ctor (node) {
  # make sure we have the ctor tag
  assert AFNIX:ADP:XML-TAG-CTOR (node:get-name)
  # create a new call command
  this:process-ctag-call node
}

# process the source oper tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-oper (node) {
  # make sure we have the oper tag
  assert AFNIX:ADP:XML-TAG-OPER (node:get-name)
  # create a new call command
  this:process-ctag-call node
}

# process the source meth tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-meth (node) {
  # make sure we have the meth tag
  assert AFNIX:ADP:XML-TAG-METH (node:get-name)
  # create a new call command
  this:process-ctag-call node
}

# process the source func tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-func (node) {
  # make sure we have the ctor tag
  assert AFNIX:ADP:XML-TAG-FUNC (node:get-name)
  # create a new call command
  this:process-ctag-call node
}

# process the source synopsys tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-snps (node) {
  # make sure we have the snps tag
  assert AFNIX:ADP:XML-TAG-SNPS (node:get-name)
  # add the snps description
  this:page:add-ref-snps (afnix:xml:pre-normal (node:to-text)) 
}

# process the source opts tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-opts (node) {
  # make sure we have the opts tag
  assert AFNIX:ADP:XML-TAG-OPTS (node:get-name)
  # add a title reference
  this:page:add-ref-opts
  # loop into the children
  const clen (node:child-length)
  loop (trans i 0) (< i clen) (i:++) {
    # get the node
    trans cnod (node:get-child i)
    trans cnam (cnod:get-name)
    switch cnam (
      (AFNIX:ADP:XML-TAG-OPTN (this:process-ctag-optn cnod))
      (else (errorln "[texwr] no handler for options tag " cnam))
    )
  }
}

# process the source list tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-list (node) {
  # make sure we have the list tag
  assert AFNIX:ADP:XML-TAG-LIST (node:get-name)
  # start a new list
  this:page:beg-list
  # loop into the children
  const clen (node:child-length)
  loop (trans i 0) (< i clen) (i:++) {
    # get the node
    trans cnod (node:get-child i)
    trans cnam (cnod:get-name)
    switch cnam (
      (AFNIX:ADP:XML-TAG-ITEM {
          # get the text value
          trans txt (cnod:to-text)
          if (cnod:attribute-p AFNIX:ADP:XML-LIST-AREF) {
            # get the ref attribute
            trans ref (cnod:get-attribute-value AFNIX:ADP:XML-LIST-AREF)
            # add the list item
            this:page:add-list-item ref (afnix:xml:to-normal txt)
          } (this:page:add-list-elem txt)
        }
      )
      (else (errorln "[texwr] no handler for list tag " cnam))
    )
  }
  # end the list
  this:page:end-list
}

# process the source table tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-tabl (node) {
  # make sure we have the table tag
  assert AFNIX:ADP:XML-TAG-TABL (node:get-name)
  # get the table row tag vector
  const nvec (afnix:adp:xmlnp:get-child-vector node AFNIX:ADP:XML-TAG-TBLR)
  # get the maximum number of data
  const cmax (afnix:adp:xmlnp:max-child-length nvec)
  # create a new table
  const tabl (this:page:new-tabl cmax)
  # loop into the children
  for (tr) (nvec) {
    tabl:new-row
    # loop into the children
    trans rlen (tr:child-length)
    loop (trans i 0) (< i rlen) (i:++) {
      # get the node
      trans cnod (tr:get-child i)
      trans cnam (cnod:get-name)
      switch cnam (
        (AFNIX:ADP:XML-TAG-TBLH (tabl:add-head (cnod:to-text)))
        (AFNIX:ADP:XML-TAG-TBLD (tabl:add-data (cnod:to-text)))
        (else (errorln "[texwr] no handler for table row tag " cnam))
      )
    }
  }
}

# process a call node
# @param node the node to process

trans afnix:adp:texwr:process-ctag-call (node) {
  # create a new call command
  const cc (this:page:add-ref-call)
  # loop into the children
  const clen (node:child-length)
  loop (trans i 0) (< i clen) (i:++) {
    # get the node
    trans cnod (node:get-child i)
    trans cnam (cnod:get-name)
    switch cnam (
      (AFNIX:ADP:XML-TAG-NAME {
          # get the text value
          trans txt (cnod:to-text)
          # set call name
          cc:set-name txt
        }
      )
      (AFNIX:ADP:XML-TAG-RETN {
          # get the text value
          trans txt (cnod:to-text)
          # set call arguments
          cc:set-retn txt
        }
      )
      (AFNIX:ADP:XML-TAG-ARGS {
          # get the text value
          trans txt (cnod:to-text)
          # set call arguments
          cc:set-args txt
        }
      )
      (AFNIX:ADP:XML-TAG-PARA {
          # create a paragraph
          const para (afnix:adp:texpc)
          # expand the paragraph
          this:expand-ctag-para cnod para
          # get the text value
          trans txt (afnix:xml:to-normal (para:to-text))
          # set command data
          cc:set-data txt
        }
      )
      (else (errorln "[texwr] no handler for tag " cnam))
    )
  }
}

# process an option node
# @param node the node to process

trans afnix:adp:texwr:process-ctag-optn (node) {
  # make sure we have the optn tag
  assert AFNIX:ADP:XML-TAG-OPTN (node:get-name)
  # create a new option command
  const co (this:page:add-ref-optn)
  # loop into the children
  const clen (node:child-length)
  loop (trans i 0) (< i clen) (i:++) {
    # get the node
    trans cnod (node:get-child i)
    trans cnam (cnod:get-name)
    switch cnam (
      (AFNIX:ADP:XML-TAG-NAME {
          # get the text value
          trans txt (cnod:to-text)
          # set call name
          co:set-name txt
        }
      )
      (AFNIX:ADP:XML-TAG-ARGS {
          # get the text value
          trans txt (cnod:to-text)
          # set call arguments
          co:set-args txt
        }
      )
      (AFNIX:ADP:XML-TAG-PARA {
          # create a paragraph
          const para (afnix:adp:texpc)
          # expand the paragraph
          this:expand-ctag-para cnod para
          # get the text value
          trans txt (afnix:xml:to-normal (para:to-text))
          # set command data
          co:set-data txt
        }
      )
      (else (errorln "[texwr] no handler for tag " cnam))
    )
  }
}

# process the source paragraph tag
# @param node the node to process

trans afnix:adp:texwr:process-ctag-para (node) {
  # make sure we have the paragraph tag
  assert AFNIX:ADP:XML-TAG-PARA (node:get-name)
  # create a new paragraph
  const para (this:page:new-para)
  # expand the paragraph node
  this:expand-ctag-para node para
}

# expand a source paragraph tag
# @param node the node to process
# @param para the paragraph to fill

trans afnix:adp:texwr:expand-ctag-para (node para) {
  # make sure we have the paragraph tag
  assert AFNIX:ADP:XML-TAG-PARA (node:get-name)
  # loop into the children
  const clen (node:child-length)
  loop (trans i 0) (< i clen) (i:++) {
    # get the node
    trans cnod (node:get-child i)
    # check for text
    if (afnix:xml:text-p cnod) (para:add-text (cnod:to-normal))
    if (afnix:xml:tag-p  cnod) {
      trans cnam (cnod:get-name)
      switch cnam (
        ("ie"      (para:add-indx (afnix:xml:to-normal (cnod:to-text))))
        ("em"      (para:add-txit (afnix:xml:to-normal (cnod:to-text))))
        ("tt"      (para:add-txtt (afnix:xml:to-normal (cnod:to-text))))
        ("file"    (para:add-txtt (afnix:xml:to-normal (cnod:to-text))))
        ("path"    (para:add-txtt (afnix:xml:to-normal (cnod:to-text))))
        ("extn"    (para:add-txtt (afnix:xml:to-normal (cnod:to-text))))
        ("code"    (para:add-txit (afnix:xml:to-normal (cnod:to-text))))
        ("major"   (para:add-text AFNIX:ADP:XML-TAG-RMAJ))
        ("minor"   (para:add-text AFNIX:ADP:XML-TAG-RMIN))
        ("patch"   (para:add-text AFNIX:ADP:XML-TAG-RPAT))
        ("afnix"   (para:add-txbf "AFNIX"))
        ("option"  (para:add-txbt (afnix:xml:to-normal (cnod:to-text))))
        ("product" (para:add-txit (afnix:xml:to-normal (cnod:to-text))))
        ("package" (para:add-txit (afnix:xml:to-normal (cnod:to-text))))
        ("command" (para:add-txtt (afnix:xml:to-normal (cnod:to-text))))
        ("link"  {
            trans text (cnod:to-text)
            para:add-txtt (afnix:xml:to-normal text)
          }
        )
        ("mail"  {
            trans addr (cnod:get-attribute-value "address") 
            trans text (cnod:to-text)
            para:add-txtt "("
            para:add-txtt (afnix:xml:to-normal addr)
            para:add-txtt ") "
            para:add-txtt (afnix:xml:to-normal text)
          }
        )
        (else    (errorln "[texwr] no handler for para tag " cnam))
      )
    }
  }
}
