// ---------------------------------------------------------------------------
// - TlsServer.cpp                                                           -
// - afnix:tls service - tls server class implementation                     -
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
#include "TlsServer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "TlsConnect.hpp"
#include "cnet.hpp"
#include "cerr.hpp"

namespace afnix {
 
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a server by name and host

  TlsServer::TlsServer (const String& host, const t_word port) :
    TcpServer (host, port) {
    d_prms.sethost (host);
    d_prms.setport (port);
  }

  // create a server by name, host and parameters

  TlsServer::TlsServer (const TlsParams& prms) : TcpServer (prms.tosprms()) {
    d_prms = prms;
  }
 
  // return the class name
  
  String TlsServer::repr (void) const {
    return "TlsServer";
  }

  // accept a tls socket connection

  TlsSocket* TlsServer::accept (void) const {
    rdlock ();
    TlsState* ssta = nullptr;
    try {
      // get the accepted socket
      auto s = dynamic_cast<TcpSocket*>(TcpServer::accept ());
      // create a tls connect
      TlsConnect tc (true, d_prms);
      // connect the socket
      ssta = tc.connect (s, s);
      if (ssta == nullptr) {
	s->close ();
	unlock ();
	return nullptr;
      }
      // create the result socket
      TlsSocket* result = new TlsSocket (s, ssta);
      unlock ();
      return result;
    } catch (...) {
      delete ssta;
      unlock ();
      throw;
    }
  }

  // get the tls parameters

  TlsParams TlsServer::getprms (void) const {
    rdlock ();
    try {
      TlsParams result = d_prms;
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
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETPRMS  = zone.intern ("get-parameters");
 
  // create a new object in a generic way

  Object* TlsServer::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();    
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      TlsParams* prms = dynamic_cast <TlsParams*> (obj);
      if (prms == nullptr) {
	throw Exception ("type-error", "invalid object as parameters",
			 Object::repr (obj));
      }
      return new TlsServer (*prms);
    }
    // check for 2 arguments
    if (argc == 2) {
      String host = argv->getstring (0);
      t_word port = argv->getlong   (1);
      return new TlsServer (host, port);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls server constructor");
  }

  // return true if the given quark is defined

  bool TlsServer::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? TcpServer::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* TlsServer::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETPRMS) return new TlsParams(getprms());
    }
    // call the tcp server method
    return TcpServer::apply (zobj, nset, quark, argv);
  }
}
