# ---------------------------------------------------------------------------
# - SIO0005.als                                                             -
# - afnix:sio module test unit                                              -
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

# @info   afnix:sio pathname test unit
# @author amaury darsch

# get the module
interp:library "afnix-sio"

# create a default path name
trans  path (afnix:sio:Pathname)
assert true (afnix:sio:pathname-p path)

# check the file name
path:set-file-name "world"
assert "world"(path:get-file-name)

# check the directory name
path:set-directory-name "hello"
assert "hello" (path:get-directory-name)

# check the path
assert 1       (path:length)
assert ""      (path:get-root)
assert "hello" (path:get-path 0)

# check the relative path
path:set-directory-name (afnix:sio:relative-path "usr" "bin")
assert 2       (path:length)
assert ""      (path:get-root)
assert "usr"   (path:get-path 0)
assert "bin"   (path:get-path 1)

# check the absolute path
path:set-directory-name (afnix:sio:absolute-path "usr" "bin")
assert 2       (path:length)

# try to rebuild the path
const p0  (path:get-path 0)
const p1  (path:get-path 1)
const pf  (afnix:sio:relative-path p0 p1)
assert pf (path:get-directory-name)

# reset and check file name
path:reset
assert 0 (path:length)

# set a relative path
path:set-file-name (afnix:sio:relative-path "usr" "axi")
assert 1     (path:length)
assert "usr" (path:get-directory-name)
assert "axi" (path:get-file-name)

# add new path component
path:add-path "bin"
assert 2      (path:length)
