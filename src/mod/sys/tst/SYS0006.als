# ---------------------------------------------------------------------------
# - SYS0006.als                                                             -
# - afnix:sys module test unit                                              -
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

# @info   options test unit
# @author amaury darsch

# get the module
interp:library "afnix-sys"

# create an option object
const  opts (afnix:sys:Options "hello")
assert true (afnix:sys:options-p opts)

# check user message
assert "hello"(opts:get-user-message)

# check a unique option
opts:add-unique-option 'h' "print this help message"
assert false (opts:get-unique-option 'h')

# parse the option
opts:parse (Vector (afnix:sys:get-option 'h'))
assert true (opts:get-unique-option 'h')

# add other option
opts:reset
opts:add-string-option 'o' "set output file"
opts:add-vector-option 'i' "add a path"
opts:add-list-option   'f' "assert" "enable assertion checking"

# parse more options
opts:parse (Vector (afnix:sys:get-option 'h') 
                   (afnix:sys:get-option 'o') "name" 
                   (afnix:sys:get-option 'i') "path"
                   (afnix:sys:get-option 'f') "assert" "hello" "world")

# check option settings
assert true (opts:get-unique-option 'h')
assert true (opts:get-unique-option 'o')
assert true (opts:get-unique-option 'f' "assert")

# get the string option
assert "name" (opts:get-string-option 'o')

# get the vector option
const   vopt  (opts:get-vector-option 'i')
assert     1  (vopt:length)
assert "path" (vopt:get 0)

# get the vector arguments
const varg     (opts:get-vector-arguments)
assert   2     (varg:length)
assert "hello" (varg:get 0)
assert "world" (varg:get 1)
