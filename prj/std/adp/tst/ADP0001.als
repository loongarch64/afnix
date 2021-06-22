# ---------------------------------------------------------------------------
# - ADP0001.als                                                             -
# - afnix:adp test module                                                   -
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

# @info   adp utilities test module
# @author amaury darsch

# load the project
interp:load "std-adp-prjld"

# check the uri name by name
trans name "http://www.afnix.org"
assert "http://www.afnix.org" (afnix:adp:get-uri-name name)

# check the uri name by name
trans name "www.afnix.org"
assert "http://www.afnix.org" (afnix:adp:get-uri-name name)

# check for local file
trans name "Makefile"
trans pnam (afnix:adp:get-uri-name name)
assert "Makefile" (afnix:sio:get-base-name pnam)

# unload the project
interp:load "std-adp-punld"
