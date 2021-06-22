# ---------------------------------------------------------------------------
# - NWG0004.als                                                             -
# - afnix:nwg module test unit                                              -
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

# @info   mime test unit
# @author amaury darsch

# get the module
interp:library "afnix-nwg"

# check the mime extension predicate
assert true  (afnix:nwg:mime-extension-p "txt")
assert true  (afnix:nwg:mime-extension-p "axc")
assert false (afnix:nwg:mime-extension-p "aaa")

# check the mime value predicate
assert true  (afnix:nwg:mime-value-p "text/plain")
assert true  (afnix:nwg:mime-value-p "application/x-afnix-source")
assert true  (afnix:nwg:mime-value-p "application/x-afnix-math-source")
assert true  (afnix:nwg:mime-value-p "application/x-afnix-spreadsheet")
assert true  (afnix:nwg:mime-value-p "application/x-afnix-octet-stream")
assert true  (afnix:nwg:mime-value-p "application/x-afnix-serial-encoded")
assert true  (afnix:nwg:mime-value-p "application/x-afnix-source-compiled")
assert true  (afnix:nwg:mime-value-p "application/x-afnix-librarian")

# check the extension to mime
assert "text/plain"                          (afnix:nwg:extension-to-mime "txt")
assert "application/xhtml+xml"               (afnix:nwg:extension-to-mime "xht")
assert "application/x-afnix-source"          (afnix:nwg:extension-to-mime "als")
assert "application/x-afnix-math-source"     (afnix:nwg:extension-to-mime "ams")
assert "application/x-afnix-spreadsheet"     (afnix:nwg:extension-to-mime "asp")
assert "application/x-afnix-octet-stream"    (afnix:nwg:extension-to-mime "asl")
assert "application/x-afnix-serial-encoded"  (afnix:nwg:extension-to-mime "aem")
assert "application/x-afnix-source-compiled" (afnix:nwg:extension-to-mime "axc")
assert "application/x-afnix-librarian"       (afnix:nwg:extension-to-mime "axl")

# check xml mime type
assert false (afnix:nwg:mime-xml-p "text/plain")
assert true  (afnix:nwg:mime-xml-p "application/xml")
