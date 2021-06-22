# ---------------------------------------------------------------------------
# - SEC0013.als                                                             -
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

# @info   hmac MD5 test unit - RFC2104
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# text
trans text "Hi There"
# key
trans keyx "0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"
trans hkey (afnix:sec:Key afnix:sec:Key:KMAC keyx)
# hmac
trans hmac (afnix:sec:Hmac hkey (afnix:sec:Md5))
hmac:compute text
assert "9294727A3638BB1C13F48EF8158BFC9D" (hmac:format)

# text
trans text "what do ya want for nothing?"
# key
trans keyx "Jefe"
trans hkey (afnix:sec:Key afnix:sec:Key:KMAC keyx)
# hmac
trans hmac (afnix:sec:Hmac hkey (afnix:sec:Md5))
hmac:compute text
assert "750C783E6AB0B503EAA86E310A5DB738" (hmac:format)
