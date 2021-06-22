# ---------------------------------------------------------------------------
# - XNET010.als                                                             -
# - afnix example : network module example                                  -
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

# send an email to yourself
# @author amaury darsch

# get the modules
interp:library "afnix-sys"
interp:library "afnix-net"

# get your user name
const user-name (afnix:sys:get-user-name)

# prepare the mail
const mail (afnix:net:Mail)
mail:to      user-name
mail:subject "hello from afnix example"
mail:addln   "This is an automatically generated message from the AFNIX"
mail:addln   "mail object - Enjoy the ride"
mail:addln   "The AFNIX team"

# send the mail
mail:send
