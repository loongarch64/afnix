# ---------------------------------------------------------------------------
# - WAX0005.als                                                             -
# - afnix:wax service test unit                                             -
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

# @info   xhtml table test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-wax"

# create table header (th) element
const  th   (afnix:wax:XhtmlTh)
assert true (afnix:xml:node-p th)
assert true (afnix:wax:xhtml-th-p th)

# create table header (td) element
const  td   (afnix:wax:XhtmlTd)
assert true (afnix:xml:node-p td)
assert true (afnix:wax:xhtml-td-p td)

# create table row (tr) element
const  tr   (afnix:wax:XhtmlTr)
assert true (afnix:xml:node-p tr)
assert true (afnix:wax:xhtml-tr-p tr)

# create a col element
const  col  (afnix:wax:XhtmlCol)
assert true (afnix:xml:node-p col)
assert true (afnix:wax:xhtml-col-p col)

# create a column group (cgr) element
const  cgr  (afnix:wax:XhtmlCgr)
assert true (afnix:xml:node-p cgr)
assert true (afnix:wax:xhtml-cgr-p cgr)

# create a table element
const  tbl  (afnix:wax:XhtmlTable)
assert true (afnix:xml:node-p tbl)
assert true (afnix:wax:xhtml-table-p tbl)
