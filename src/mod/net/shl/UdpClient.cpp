// ---------------------------------------------------------------------------
// - UdpClient.cpp                                                           -
// - afnix:net module - udp client socket implementation                     -
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

#include "Vector.hpp"
#include "UdpClient.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a udp client by host and port

  UdpClient::UdpClient (const String& host, t_word port) : UdpSocket (false) {
    // get the host address
    Address addr (host);
    // create the socket
    create (addr);
    // connect with the server
    if (connect (port, addr, true) == false) {
      throw Exception ("client-error", "cannot connect socket");
    }
  }

  // create a udp client by ip address and port

  UdpClient::UdpClient (const Address& addr, t_word port) : UdpSocket (false) {
    // create the socket
    create (addr);
    // connect with the server
    if (connect (port, addr, true) == false) {
      throw Exception ("client-error", "cannot connect socket");
    }
  }

  // return the class name

  String UdpClient::repr (void) const {
    return "UdpClient";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* UdpClient::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 2) 
      throw Exception ("argument-error", 
                       "invalid arguments with with udp client"); 
    // get the udp client arguments
    Object* obj = argv->get (0);
    if (dynamic_cast <String*> (obj) != nullptr) {
      String host = argv->getstring (0);
      long   port = argv->getlong (1);
      return new UdpClient (host, port);
    }
    Address* addr = dynamic_cast <Address*> (obj);
    if (addr != nullptr) {
      long   port = argv->getlong (1);
      return new UdpClient (*addr, port);
    }
    throw Exception ("argument-error", "invalid object with udp client",
		     Object::repr (obj));
  }
}
