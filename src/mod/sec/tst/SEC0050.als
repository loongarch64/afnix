# ---------------------------------------------------------------------------
# - SEC0050.als                                                             -
# - afnix:sec module test unit                                              -
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

# @info   keyring test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# create a default keyring
const krng (afnix:sec:Keyring)

# check predicates
assert true (afnix:sec:keyring-p krng)
assert "Keyring" (krng:repr)
assert false (krng:isopen-p)

# check name and info
krng:set-name "KEYRING-TEST"
assert "KEYRING-TEST" (krng:get-name)
krng:set-info "KEYRING TEST"
assert "KEYRING TEST" (krng:get-info)

# add a rsa key
const krsa (afnix:sec:Key afnix:sec:Key:KRSA 256)
krsa:set-name "rsa-key"
assert "rsa-key" (krsa:get-name)
krng:add krsa
# add a symmetrix key
const ksym (afnix:sec:Key afnix:sec:Key:KSYM 128)
ksym:set-name "sym-key"
assert "sym-key" (ksym:get-name)
krng:add ksym
# check keys
trans keys (krng:to-keys)
assert 2 (keys:length)
# check existence
assert true (krng:exists-p "rsa-key")
assert true (krng:exists-p "sym-key")
# check for open
assert true (krng:isopen-p)

# close the keyring
const cred "secret credential"
krng:close cred
# check for close
assert false (krng:isopen-p)
# check keys
trans keys (krng:to-keys)
assert true (nil-p keys)

# open the keyring
krng:open cred
# check for open
assert true (krng:isopen-p)
# check keys
trans keys (krng:to-keys)
assert 2 (keys:length)
# check existence
assert true (krng:exists-p "rsa-key")
assert true (krng:exists-p "sym-key")

