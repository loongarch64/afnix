// ---------------------------------------------------------------------------
// - t_message.cpp                                                           -
// - standard object library - message class tester                          -
// ---------------------------------------------------------------------------
// - This program is free software;  you can redistribute it  and/or  modify -
// - it provided that this copyright notice is kept intact.                  -
// -                                                                         -
// - This program  is  distributed in  the hope  that it will be useful, but -
// - without  any  warranty;  without  even   the   implied    warranty   of -
// - merchantability or fitness for a particular purpose.  In no event shall -
// - the copyright holder be liable for any  direct, indirect, incidental or -
// - special damages arising in any way out of the use of this software.     -
// ---------------------------------------------------------------------------
// - copyright (c) 1999-2021 amaury darsch                                   -
// ---------------------------------------------------------------------------

#include "Message.hpp"

int main (int, char**) {
  using namespace afnix;
  
  // create a message
  String name = "AFNIX";
  String info = "AFNIX WRITING SYSTEM";

  Message mesg (name, info);

  // check content
  if (mesg.gettype () != Message::MESG_INFO) return 1;
  if (mesg.getname () != name) return 1;
  if (mesg.getinfo () != info) return 1;
  if (mesg.getcode () != 0)    return 1;

  // set the message content
  mesg.settype (Message::MESG_MERR);
  if (mesg.gettype () != Message::MESG_MERR) return 1;
  mesg.setname ("afnix");
  if (mesg.getname () != "afnix") return 1;
  mesg.setinfo ("afnix");
  if (mesg.getinfo () != "afnix") return 1;
  mesg.setcode (1);
  if (mesg.getcode () != 1) return 1;

  // success
  return 0;
}
