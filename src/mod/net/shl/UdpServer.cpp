// ---------------------------------------------------------------------------
// - UdpServer.cpp                                                           -
// - afnix:net module - udp server socket implementation                     -
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
#include "Integer.hpp"
#include "UdpServer.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a udp server on a ephemeral port

  UdpServer::UdpServer (void) {
    // bind on an ephemeral port
    if (bind (0) == false)
      throw Exception ("server-error", "cannot bind socket");
  }

  // create a udp server with a port

  UdpServer::UdpServer (t_word port) {
    // bind this socket
    if (bind (port) == false)
      throw Exception ("server-error", "cannot bind socket");
  }

  // create a udp server by parameters

  UdpServer::UdpServer (const SockParams& prms) : UdpSocket (false) {
    // get the server parameters
    Address addr = prms.getaddr ();
    t_word  port = prms.getport ();
    // create the server by address
    create (addr);
    setopt (prms);
    // bind the socket
    if (bind (port, addr) == false) {
      throw Exception ("server-error", "cannot bind socket");
    }
  }
  
  // create a udp server with a host and port

  UdpServer::UdpServer (const String& host, t_word port) : UdpSocket (false) {
    // get the host address
    Address addr (host);
    // create the socket
    create (addr);
    // bind the socket 
    if (bind (port, addr) == false)
      throw Exception ("server-error", "cannot bind socket");
  }

  // create a udp server with an address and port

  UdpServer::UdpServer (const Address& addr, t_word port) : UdpSocket (false) {
    // create the socket
    create (addr);
    // bind this socket
    if (bind (port, addr) == false)
      throw Exception ("server-error", "cannot bind socket");
  }

  // return the class name

  String UdpServer::repr (void) const {
    return "UdpServer";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* UdpServer::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for zero argument
    if (argc == 0) return new UdpServer;
    // check for one argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a port
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj != nullptr) {
	t_word port = iobj->tolong ();
	return new UdpServer (port);
      }
      // check for a socket parameters
      SockParams* prms = dynamic_cast <SockParams*> (obj);
      if (prms != nullptr) {
	return new UdpServer (*prms);
      }
    }
    // check for two arguments
    if (argc == 2) {
      Object* obj = argv->get (0);
      // check for a host
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) {
	t_word port    = argv->getlong (1);
	return new UdpServer (*sobj, port);
      }
      // check for an address
      Address* aobj = dynamic_cast <Address*> (obj);
      if (aobj != nullptr) {
	t_word port = argv->getlong (1);
	return new UdpServer (*aobj, port);
      }
    }
    throw Exception ("argument-error", "invalid arguments with udp server");
  }
}
