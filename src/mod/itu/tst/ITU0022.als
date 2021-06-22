# ---------------------------------------------------------------------------
# - ITU0022.als                                                             -
# - afnix:itu module test unit                                              -
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

# @info   asn generalized time node test unit
# @author amaury darsch

# get the module
interp:library "afnix-itu"

# this function check the asn node
# @param tval the time value to check
# @param bval the buffer format value
# @param cval the content value
# @param uval the utc flag
const asn-gtm-check (tval bval cval uval) {
  # create an asn gtm node
  trans  atn  (afnix:itu:AsnGtm tval)
  assert true (afnix:itu:asn-node-p atn)
  assert true (afnix:itu:asn-gtm-p  atn)
  
  # check name and contents
  assert "AsnGtm" (atn:repr)
  assert uval     (atn:utc-p)
  assert tval     (atn:to-string)
  
  # create a buffer and check
  trans buf (Buffer)
  atn:write buf
  assert bval (buf:format)
  
  # parse the buffer and check
  const ant (afnix:itu:AsnBuffer buf)
  assert afnix:itu:AsnNode:UNIVERSAL (ant:get-class)
  assert true  (ant:primitive-p)
  assert false (ant:constructed-p)
  assert 24    (ant:get-tag-number)
  
  # check the content
  const  cbuf (ant:get-content-buffer)
  assert (cbuf:length) (ant:get-content-length)
  assert cval (cbuf:format)
  
  # map the node to a gtm
  trans atn (ant:node-map)
  assert true (afnix:itu:asn-node-p atn)
  assert true (afnix:itu:asn-gtm-p  atn)
  
  # check contents
  assert uval     (atn:utc-p)
  assert tval     (atn:to-string)

  # create a buffer and check
  trans buf (Buffer)
  atn:write buf
  assert bval (buf:format)
}

# check for zulu format
trans tval "19641120123456Z"
trans bval "180F31393634313132303132333435365A"
trans cval "31393634313132303132333435365A"
asn-gtm-check tval bval cval true

# check for zulu with fraction
trans tval "19641120123456.789Z"
trans bval "181331393634313132303132333435362E3738395A"
trans cval "31393634313132303132333435362E3738395A"
asn-gtm-check tval bval cval true

# check for local format
trans tval "19641120123456"
trans bval "180E3139363431313230313233343536"
trans cval "3139363431313230313233343536"
asn-gtm-check tval bval cval false

# check for local with fraction
trans tval "19641120123456.789"
trans bval "181231393634313132303132333435362E373839"
trans cval "31393634313132303132333435362E373839"
asn-gtm-check tval bval cval false

# check for local format with time zone
trans tval "19641120123456+0800"
trans bval "181331393634313132303132333435362B30383030"
trans cval "31393634313132303132333435362B30383030"
asn-gtm-check tval bval cval false

# check for local format with time zone and fraction
trans tval "19641120123456.789-0800"
trans bval "181731393634313132303132333435362E3738392D30383030"
trans cval "31393634313132303132333435362E3738392D30383030"
asn-gtm-check tval bval cval false
