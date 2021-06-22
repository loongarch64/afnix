// ---------------------------------------------------------------------------
// - Multicast.cpp                                                           -
// - afnix:net module - multicast udp client socket implementation           -
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
#include "Multicast.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a mct client by host

  Multicast::Multicast (const String& host) : UdpSocket (false) {
    // save the address and port
    d_addr = Address (host);
    d_port = 0;
    // create the socket
    create (d_addr);
    // bind this socket on a ephemeral port
    if (bind (d_port, d_addr) == false)
      throw Exception ("multicast-error", "cannot bind socket");
    // fix port 0 with allocated port
    d_port = getsockport ();
    if (d_port == 0)
      throw Exception ("multicast-error", "cannot get ephemeral port");
    // join the multicast group
    if (join (d_addr) == false)
      throw Exception ("multicast-error", "cannot join multicast group");
  }

  // create a mct client by ip address

  Multicast::Multicast (const Address& addr) : UdpSocket (false) {
    // save the address and port
    d_addr = addr;
    d_port = 0;
    // create the socket
    create (d_addr);
    // bind this socket on a ephemeral port
    if (bind (d_port, d_addr) == false)
      throw Exception ("multicast-error", "cannot bind socket");
    // fix port 0 with allocated port
    d_port = getsockport ();
    if (d_port == 0)
      throw Exception ("multicast-error", "cannot get ephemeral port");
    // join the multicast group
    if (join (d_addr) == false)
      throw Exception ("multicast-error", "cannot join multicast group");
  }

  // create a mct client by host and port

  Multicast::Multicast (const String& host, t_word port) : UdpSocket (false) {
    // save the address and port
    d_addr = Address (host);
    d_port = port;
    // create the socket
    create (d_addr);
    // bind this socket
    if (bind (d_port, d_addr) == false)
      throw Exception ("multicast-error", "cannot bind socket");
    // fix port 0 with allocated port
    if (d_port == 0) d_port = getsockport ();
    // join the multicast group
    if (join (d_addr) == false)
      throw Exception ("multicast-error", "cannot join multicast group");
  }

  // create a mct client by ip address and port

  Multicast::Multicast (const Address& addr, t_word port) : UdpSocket (false) {
    // save the address and port
    d_addr = addr;
    d_port = port;
    // create the socket
    create (d_addr);
    // bind this socket
    if (bind (d_port, d_addr) == false)
      throw Exception ("multicast-error", "cannot bind socket");
    // fix port 0 with allocated port
    if (d_port == 0) d_port = getsockport ();
    // join the multicast group
    if (join (d_addr) == false)
      throw Exception ("multicast-error", "cannot join multicast group");
  }

  // destroy this multicast socket

  Multicast::~Multicast (void) {
    close ();
  }

  // return the class name

  String Multicast::repr (void) const {
    return "Multicast";
  }

  // close this multicast socket

  bool Multicast::close (void) {
    wrlock ();
    if ((d_sid == -1) || (Object::uref (this) == false)) {
      unlock ();
      return true;
    }
    // drop membership and close
    drop (d_addr);
    bool result = Socket::close ();
    unlock ();
    return result;
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* Multicast::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      // get the mct client argument
      Object* obj = argv->get (0);
      if (dynamic_cast <String*> (obj) != nullptr) {
	String host = argv->getstring (0);
	return new Multicast (host);
      }
      Address* addr = dynamic_cast <Address*> (obj);
      if (addr != nullptr) {
	return new Multicast (*addr);
      }
      throw Exception ("argument-error", "invalid arguments with multicast",
		       Object::repr (obj));
    }
    // check for 2 arguments
    if (argc == 2) {
      // get the mct client arguments
      Object* obj = argv->get (0);
      if (dynamic_cast <String*> (obj) != nullptr) {
	String host = argv->getstring (0);
	long   port = argv->getlong (1);
	return new Multicast (host, port);
      }
      Address* addr = dynamic_cast <Address*> (obj);
      if (addr != nullptr) {
	long   port = argv->getlong (1);
	return new Multicast (*addr, port);
      }
      throw Exception ("argument-error", "invalid arguments with multicast",
		       Object::repr (obj));
    }
    throw Exception ("argument-error", "invalid arguments with multicast");
  }
}
