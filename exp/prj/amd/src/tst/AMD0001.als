# ----------------------------------------------------------------------------
# - AMD0001.als                                                              -
# - afnix:amd test module                                                    -
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

# @info   amd utilities test module
# @author amaury darsch

# load the project
interp:load "exp-amd-prjld"

# check the uri name by name
trans name "http://www.afnix.org"
assert "http://www.afnix.org/" (afnix:amd:get-uri-name name)

# check the uri name by name
trans name "www.afnix.org"
assert "http://www.afnix.org/" (afnix:amd:get-uri-name name)

# unload the project
interp:load "exp-amd-punld"
