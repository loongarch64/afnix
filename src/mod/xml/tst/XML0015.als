# ---------------------------------------------------------------------------
# - XML0015.als                                                             -
# - afnix:xml module test unit                                              -
# ---------------------------------------------------------------------------
# - This program is free software;  you can redistribute it  and/or  modify -
# - it provided that this copyright notice is kept intact.                  -
# -                                                                         -
# - This program  is  distributed in  the hope  that it will be useful, but -
# - without  any  warranty;  without  even   the   implied    warranty   of -
# - merchantability or fitness for a particular purpose.  In no event shall -
# - the copyright holder be liable for any  direct, indirect, incidental or -
# - special damages arising in any way out of the use of this software.     -
# ---------------------------------------------------------------------------
# - copyright (c) 1999-2021 amaury darsch                                   -
# ---------------------------------------------------------------------------

# @info   section node test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# the section value
const xval "%axmxa-extra-module;"

# create a document type node
trans  node (afnix:xml:XmlSection xval)
assert true (afnix:xml:node-p node)
assert true (afnix:xml:section-p node)

# create a reader and check doctype node
const xmlr  (afnix:xml:XmlReader)

# parse a simple section definition
trans  node (xmlr:get-node "<![%axmxa-extra-module;[]]>") 
assert true (afnix:xml:section-p node)
assert xval (node:get-xval)

# parse an ignore like section definition
trans  node (xmlr:get-node "<![%axmxa-extra-module;[
             %axmxa-extra-mod;]]>") 
assert true (afnix:xml:section-p node)
assert xval (node:get-xval)

# parse a complex section definition
trans  node (xmlr:get-node "<![%axmxa-extra-module;[
             <!ENTITY % axmxa-extra-mod 
               PUBLIC \"-//AFNIX//DTD AXMXA EXTRA ELEMENTS//EN\"
                      \"http://www.afnix.org/dtd/axmxa-extra.mod\">
              %axmxa-extra-mod;]]>") 
assert true (afnix:xml:section-p node)
assert xval (node:get-xval)
