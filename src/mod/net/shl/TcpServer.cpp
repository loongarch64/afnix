// ---------------------------------------------------------------------------
// - TcpServer.cpp                                                           -
// - afnix:net module - tcp server socket implementation                     -
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

#include "Error.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "TcpServer.hpp"
#include "QuarkZone.hpp"
#include "cnet.hpp"
#include "cerr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a tcp server on a ephemeral port
  
  TcpServer::TcpServer (void) {
    // get the server parameters
    SockParams prms;
    t_word port = prms.getport ();
    long   blog = prms.getblog ();
    // bind and listen this socket
    if (bind (port) == false) {
      throw Exception ("server-error", "cannot bind socket");
    }
    // listen on the socket
    if (listen (blog) == false) {
      throw Exception ("server-error", "cannot listen on socket");
    }
  }

  // create a tcp server by parameters

  TcpServer::TcpServer (const SockParams& prms) : TcpSocket (false) {
    // get the server parameters
    Address addr = prms.getaddr ();
    t_word  port = prms.getport ();
    long    blog = prms.getblog ();
    // create the server by address
    create (addr);
    setopt (prms);
    // bind the socket 
    if (bind (port, addr) == false) {
      throw Exception ("server-error", "cannot bind socket");
    }
    // listen on the socket
    if (listen (blog) == false) {
      throw Exception ("server-error", "cannot listen on socket");
    }
  }
  
  // create a tcp server with a port

  TcpServer::TcpServer (const t_word port) {
    // get the server parameters
    SockParams prms;
    long blog = prms.getblog ();
    // bind and listen this socket
    if (bind (port) == false) {
      throw Exception ("server-error", "cannot bind socket");
    }
    // listen on the socket
    if (listen (blog) == false) {
      throw Exception ("server-error", "cannot listen on socket");
    }
  }

  // create a tcp server with a host and port

  TcpServer::TcpServer (const String& host,
			const t_word port) : TcpSocket (false) {
    // get the server parameters
    SockParams prms;
    long blog = prms.getblog ();
    // create the socket by address
    Address addr (host);
    create (addr);
    // bind and listen this socket
    if (bind (port, addr) == false) {
      throw Exception ("server-error", "cannot bind socket");
    }
    if (listen (blog) == false) {
      throw Exception ("server-error", "cannot listen on socket");
    }
  }

  // create a tcp server with an address and port

  TcpServer::TcpServer (const Address& addr,
			const t_word port) : TcpSocket (false) {
    // get the server parameters
    SockParams prms;
    long blog = prms.getblog ();
    // create the socket by address
    create (addr);
    // bind and listen this socket
    if (bind (port, addr) == false) {
      throw Exception ("server-error", "cannot bind socket");
    }
    if (listen (blog) == false) {
      throw Exception ("server-error", "cannot listen on socket");
    }
  }
  
  // return the class name

  String TcpServer::repr (void) const {
    return "TcpServer";
  }

  // listen on this tcp socket
  
  bool TcpServer::listen (const long backlog) const {
    rdlock ();
    try {
      bool result = c_iplisten (d_sid, backlog);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // accept a connection from this tcp socket

  SocketStream* TcpServer::accept (void) const {
    rdlock ();
    try {
      int sid = c_ipaccept (d_sid);
      if (sid < 0) throw Error ("accept-error", c_errmsg (sid), sid);
      TcpSocket* result = new TcpSocket (sid);
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

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_ACCEPT = zone.intern ("accept");
  static const long QUARK_LISTEN = zone.intern ("listen");
  
  // create a new object in a generic way

  Object* TcpServer::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 arguments
    if (argc == 0) return new TcpServer;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a port
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj != nullptr) {
	t_word port = iobj->tolong ();
	return new TcpServer (port);
      }
      // check for a socket parameters
      SockParams* prms = dynamic_cast <SockParams*> (obj);
      if (prms != nullptr) {
	return new TcpServer (*prms);
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      Object* obj = argv->get (0);
      // check for a host
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) {
	t_word port    = argv->getlong (1);
	return new TcpServer (*sobj, port);
      }
      // check for an address
      Address* aobj = dynamic_cast <Address*> (obj);
      if (aobj != nullptr) {
	t_word port    = argv->getlong (1);
	return new TcpServer (*aobj, port);
      }
    }
    throw Exception ("argument-error", "invalid arguments with tcp server");
  }

  // return true if the given quark is defined

  bool TcpServer::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? TcpSocket::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark

  Object* TcpServer::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_ACCEPT) return accept ();
      if (quark == QUARK_LISTEN) return new Boolean (listen (5));
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_LISTEN) {
	long backlog = argv->getlong (0);
	return new Boolean (listen (backlog));
      }
    }
    // call the tcpsocket method
    return TcpSocket::apply (zobj, nset, quark, argv);
  }
}
