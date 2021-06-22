// ---------------------------------------------------------------------------
// - TlsClient.cpp                                                           -
// - afnix:tls service - tls client class implementation                     -
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
#include "TlsClient.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "TcpClient.hpp"
#include "TlsConnect.hpp"

namespace afnix {
 
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a client by name and host

  TlsClient::TlsClient (const String& host, const t_word port) {
    // preset the parameters
    d_prms.sethost (host);
    d_prms.setport (port);
    // try to bind the socket
    TlsState*  ssta = nullptr;
    TcpSocket* tcps = nullptr;
    try {
      // create a tcp socket
      tcps = new TcpClient (host, port);
      // create a tls connect
      TlsConnect tc (false, d_prms);
      // connect the socket
      ssta = tc.connect (tcps, tcps);
      // bind the tls socket
      bind (tcps, ssta);
    } catch (...) {
      delete ssta;
      delete tcps;
    }
  }

  // create a client by name, host and parameters

  TlsClient::TlsClient (const TlsParams& prms) {
    // save the parameters
    d_prms = prms;
    // try to bind the socket
    TlsState*  ssta = nullptr;
    TcpSocket* tcps = nullptr;
    try {
      // create a tcp socket
      tcps = new TcpClient (prms.tosprms());
      // create a tls connect
      TlsConnect tc (false, d_prms);
      // connect the socket
      ssta = tc.connect (tcps, tcps);
      // bind the tls socket
      bind (tcps, ssta);
    } catch (...) {
      delete ssta;
      delete tcps;
    }
  }
 
  // return the class name
  
  String TlsClient::repr (void) const {
    return "TlsClient";
  }

  // get the tls parameters

  TlsParams TlsClient::getprms (void) const {
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

  Object* TlsClient::mknew (Vector* argv) {
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
      return new TlsClient (*prms);
    }
    // check for 2 arguments
    if (argc == 2) {
      String host = argv->getstring (0);
      t_word port = argv->getlong   (1);
      return new TlsClient (host, port);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls client constructor");
  }

  // return true if the given quark is defined

  bool TlsClient::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? TlsSocket::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* TlsClient::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETPRMS) return new TlsParams(getprms());
    }
    // call the tls socket method
    return TlsSocket::apply (zobj, nset, quark, argv);
  }
}
