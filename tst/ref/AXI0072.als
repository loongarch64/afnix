# ---------------------------------------------------------------------------
# - AXI0072.als                                                             -
# - afnix engine test module                                                -
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

# @info   message class test module
# @author amaury darsch

# create an info message
const name "AFNIX"
const info "AFNIX WRITING SYSTEM"
const mesg (Message name info)

# check representation
assert true (message-p mesg)
assert "Message" (mesg:repr)

# check content
assert Message:INFO (mesg:get-type)
assert name (mesg:get-name)
assert info (mesg:get-info)
assert 0    (mesg:get-code)

# set the content and check
mesg:set Message:ERROR "afnix" "info" 1
assert Message:ERROR (mesg:get-type)
assert "afnix" (mesg:get-name)
assert "info"  (mesg:get-info)
assert 1       (mesg:get-code)

# set individual
mesg:set-type Message:INFO
assert Message:INFO (mesg:get-type)
mesg:set-type Message:WARNING
assert Message:WARNING (mesg:get-type)
mesg:set-type Message:ERROR
assert Message:ERROR (mesg:get-type)
mesg:set-type Message:FATAL
assert Message:FATAL (mesg:get-type)
