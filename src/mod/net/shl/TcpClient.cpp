// ---------------------------------------------------------------------------
// - TcpClient.cpp                                                           -
// - afnix:net module - tcp client socket implementation                     -
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
#include "TcpClient.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a tcp server by parameters

  TcpClient::TcpClient (const SockParams& prms) : TcpSocket (false) {
    if (connect (prms) == false) {
      throw Exception ("client-error", "cannot connect socket");
    }
  }
  
  // create a tcp client by host and port

  TcpClient::TcpClient (const String& host,
			const t_word  port) : TcpSocket (false) {
    if (connect (host, port) == false) {
      throw Exception ("client-error", "cannot connect socket");
    }
  }

  // create a tcp client by ip address and port

  TcpClient::TcpClient (const Address& addr,
			const t_word   port) : TcpSocket (false) {
    if (connect (addr, port) == false) {
      throw Exception ("client-error", "cannot connect socket");
    }
  }

  // return the class name

  String TcpClient::repr (void) const {
    return "TcpClient";
  }

  // connect this client by parameters
  
  bool TcpClient::connect (const SockParams& prms) {
    wrlock ();
    try {
      // get the server parameters
      Address addr = prms.getaddr ();
      t_word  port = prms.getport ();
      // create the client by address
      create (addr);
      setopt (prms);
      // connect with the server
      bool result = Socket::connect (port, addr, true);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // connect this client by host and port

  bool TcpClient::connect (const String& host, const t_word port) {
    wrlock ();
    try {
      // get the host address
      Address addr (host);
      // create the client by address
      create (addr);
      // connect with the server
      bool result = Socket::connect (port, addr, true);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // connect this client client by ip address and port

  bool TcpClient::connect (const Address& addr, const t_word port) {
    wrlock ();
    try {
      // create the socket
      create (addr);
      // connect with the server
      bool result = Socket::connect (port, addr, true);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* TcpClient::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a socket parameters
      SockParams* prms = dynamic_cast <SockParams*> (obj);
      if (prms != nullptr) {
	return new TcpClient (*prms);
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      // get the tcp client arguments
      Object* obj = argv->get (0);
      if (dynamic_cast <String*> (obj) != nullptr) {
	String host = argv->getstring (0);
	long   port = argv->getlong (1);
	return new TcpClient (host, port);
      }
      Address* addr = dynamic_cast <Address*> (obj);
      if (addr != nullptr) {
	long   port = argv->getlong (1);
	return new TcpClient (*addr, port);
      }
    }
    throw Exception ("argument-error", "invalid arguments with tcp client");

  }
}
