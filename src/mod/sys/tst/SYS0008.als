# ---------------------------------------------------------------------------
# - SYS0008.als                                                             -
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

# @info   host/domain/fqdn test unit
# @author amaury darsch

# get the module
interp:library "afnix-sys"

# get the fqdn/domain/host
const fqdn   (afnix:sys:get-host-fqdn)
const domain (afnix:sys:get-domain-name)
const host   (afnix:sys:get-host-name)
if (domain:nil-p) (const name host) (const name   (+ (+ host '.') domain))

# rebuild and test
if (nil-p domain) (assert fqdn host) (assert fqdn name)
