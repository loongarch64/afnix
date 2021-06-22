// ---------------------------------------------------------------------------
// - SocketStream.cpp                                                        -
// - afnix:net module - socket stream class implementation                   -
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
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "SocketStream.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 10;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_IPV6P     = zone.intern ("ipv6-p");
  static const long QUARK_OPENP     = zone.intern ("open-p");
  static const long QUARK_BCASTP    = zone.intern ("broadcast-p");
  static const long QUARK_GETPRTO   = zone.intern ("get-protocol");
  static const long QUARK_SOCKADDR  = zone.intern ("get-socket-address");
  static const long QUARK_SOCKPORT  = zone.intern ("get-socket-port");
  static const long QUARK_SOCKAUTH  = zone.intern ("get-socket-authority");
  static const long QUARK_PEERADDR  = zone.intern ("get-peer-address");
  static const long QUARK_PEERPORT  = zone.intern ("get-peer-port");
  static const long QUARK_PEERAUTH  = zone.intern ("get-peer-authority");

  // return true if the given quark is defined

  bool SocketStream::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? DupleixStream::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* SocketStream::apply (Evaluable* zobj, Nameset* nset, const long quark,
			       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_IPV6P)    return new Boolean (isipv6 ());
      if (quark == QUARK_OPENP)    return new Boolean (isopen ());
      if (quark == QUARK_BCASTP)   return new Boolean (isbcast ());
      if (quark == QUARK_GETPRTO)  return new String  (getprotocol ());
      if (quark == QUARK_SOCKPORT) return new Integer (getsockport ());
      if (quark == QUARK_PEERPORT) return new Integer (getpeerport ());
      if (quark == QUARK_SOCKAUTH) return new String  (getsockauth ());
      if (quark == QUARK_PEERAUTH) return new String  (getpeerauth ());
      if (quark == QUARK_SOCKADDR) {
	rdlock ();
	try {
	  Object* result = getsockaddr ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_PEERADDR) {
	rdlock ();
	try {
	  Object* result = getpeeraddr ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // call the dupleix stream method
    return DupleixStream::apply (zobj, nset, quark, argv);
  }
}
