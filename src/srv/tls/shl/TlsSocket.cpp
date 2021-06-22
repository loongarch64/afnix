// ---------------------------------------------------------------------------
// - TlsSocket.cpp                                                          -
// - afnix:tls service - tls socket stream class implementation             -
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
#include "TlsAlert.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "TlsSocket.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a null tls socket

  TlsSocket::TlsSocket (void) {
    p_tcps = nullptr;
    p_ssta = nullptr;
    p_is   = nullptr;
    p_os   = nullptr;
  }
  
  // create a tls socket by streams and state

  TlsSocket::TlsSocket (TcpSocket* tcps, TlsState* ssta) {
    // reset everything
    p_tcps = nullptr;
    p_ssta = nullptr;
    p_is   = nullptr;
    p_os   = nullptr;
    // bind the socket and state
    bind (tcps, ssta);
  }

  // destroy this tls socket

  TlsSocket::~TlsSocket (void) {
    reset ();
  }

  // return the class name
  
  String TlsSocket::repr (void) const {
    return "TlsSocket";
  }

  // reset this tls socket

  void TlsSocket::reset (void) {
    wrlock ();
    try {
      Object::dref (p_ssta);
      Object::dref (p_is); p_os = nullptr;
      Object::dref (p_os); p_os = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the socket input stream

  InputStream* TlsSocket::getis (void) {
    rdlock ();
    try {
      InputStream* result = p_is;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the socket input stream

  OutputStream* TlsSocket::getos (void) {
    rdlock ();
    try {
      OutputStream* result = p_os;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the tls socket info

  Plist TlsSocket::getinfo (void) const {
    rdlock ();
    try {
      Plist result;
      if (p_ssta != nullptr) result = p_ssta->getinfo();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // close this socket stream

  bool TlsSocket::close (void) {
    wrlock ();
    try {
      Object::dref (p_is); p_is = nullptr;
      Object::dref (p_os); p_os = nullptr;
      bool result = p_tcps->close();
      Object::dref (p_tcps); p_tcps = nullptr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return true if the eos flag is set
  
  bool TlsSocket::iseos (void) const {
    wrlock ();
    try {
      if (d_sbuf.length () != 0) {
	unlock ();
	return false;
      }
      bool result = (p_is == nullptr) ? true : p_is->iseos ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // check if we can read a character
  
  bool TlsSocket::valid (void) const {
    wrlock ();
    try {
      // check local buffer
      if (d_sbuf.length () != 0) {
	unlock ();
	return true;
      }
      // check stream validity
      bool result = (p_is == nullptr) ? false : p_is->valid ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // read one character from the socket

  char TlsSocket::read (void) {
    wrlock ();
    try {
      // check valid stream
      if (p_is == nullptr) {
	char result = eosc;
	unlock ();
	return result;
      }
      // check the pushback buffer first
      if (d_sbuf.empty () == false) {
	char result = d_sbuf.read ();
	unlock ();
	return result;
      }
      // read a character
      char result = p_is->read ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // write one character to the socket stream
  
  long TlsSocket::write (const char value) {
    wrlock ();
    try {
      long result = (p_os == nullptr) ? 0L : p_os->write (value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // write a data buffer to the socket

  long TlsSocket::write (const char* value) {
    // lock and write
    wrlock ();
    try {
      long result = (p_os == nullptr) ? 0L : p_os->write (value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if we have an ipv6 socket

  bool TlsSocket::isipv6 (void) const {
    rdlock ();
    try {
      bool result = (p_tcps == nullptr) ? false : p_tcps->isipv6 ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  

  // check if we can broadcast messages

  bool TlsSocket::isbcast (void) const {
    rdlock ();
    try {
      bool result = (p_tcps == nullptr) ? false : p_tcps->isbcast ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if the socket is open

  bool TlsSocket::isopen (void) const {
    rdlock ();
    try {
      bool result = (p_tcps == nullptr) ? false : p_tcps->isopen ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the socket address
  
  Address* TlsSocket::getsockaddr (void) const {
    rdlock ();
    try {
      Address* result = (p_tcps == nullptr) ? nullptr : p_tcps->getsockaddr ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the socket port

  t_word TlsSocket::getsockport (void) const {
    rdlock ();
    try {
      t_word result = (p_tcps == nullptr) ? 0 : p_tcps->getsockport ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // @return the socket authority

  String TlsSocket::getsockauth (void) const {
    rdlock ();
    try {
      String result;
      if (p_tcps != nullptr) result = p_tcps->getsockauth ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get peer address

  Address* TlsSocket::getpeeraddr (void) const {
    rdlock ();
    try {
      Address* result = (p_tcps == nullptr) ? nullptr : p_tcps->getpeeraddr ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the peer port
  
  t_word TlsSocket::getpeerport (void) const {
    rdlock ();
    try {
      t_word result = (p_tcps == nullptr) ? 0 : p_tcps->getpeerport ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the peer authority

  String TlsSocket::getpeerauth (void) const {
    rdlock ();
    try {
      String result;
      if (p_tcps != nullptr) result = p_tcps->getpeerauth ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the socket protocol

  String TlsSocket::getprotocol (void) const {
    rdlock ();
    try {
      String result;
      if (p_tcps != nullptr) result = p_tcps->getprotocol ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the tls state

  TlsState* TlsSocket::getstate (void) const {
    rdlock ();
    try {
      TlsState* result = p_ssta;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  void TlsSocket::bind (TcpSocket* tcps, TlsState* ssta) {
    wrlock ();
    try {
      // delete old streams
      Object::dref (p_is); p_is = nullptr;
      Object::dref (p_os); p_os = nullptr;
      // save the tcp socket and state
      Object::iref (tcps); Object::dref (p_tcps); p_tcps = tcps;
      Object::iref (ssta); Object::dref (p_ssta); p_ssta = ssta;
      // set the streams
      if (tcps != nullptr) {
	Object::iref (p_is = new TlsInput  (tcps->getis (), ssta));
	Object::iref (p_os = new TlsOutput (tcps->getos (), ssta));
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETTIS   = zone.intern ("get-input-stream");
  static const long QUARK_GETTOS   = zone.intern ("get-output-stream");
  static const long QUARK_GETSTATE = zone.intern ("get-state");
 
  // create a new object in a generic way

  Object* TlsSocket::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 2 arguments
    if (argc == 2) {
      Object* obj = argv->get (0);
      // check for a tcp socket
      TcpSocket* tcps = dynamic_cast<TcpSocket*> (obj);
      if (tcps == nullptr) {
	throw Exception ("type-error", "invalid object as tcp socket",
			 Object::repr (obj));
      }
      // check for a state
      obj = argv->get (1);
      TlsState* ssta =  dynamic_cast<TlsState*> (obj);
      if (ssta == nullptr) {
	throw Exception ("type-error", "invalid object as tls state",
			 Object::repr (obj));
      }
      return new TlsSocket (tcps, ssta);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls socket constructor");
  }

  // return true if the given quark is defined

  bool TlsSocket::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? SocketStream::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? TlsInfos::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* TlsSocket::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETSTATE) {
	rdlock ();
	try {
	  TlsState* result = getstate ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // check the tls infos
    if (TlsInfos::isquark (quark, true) == true) {
      return TlsInfos::apply (zobj, nset, quark, argv);
    }
    // fallback on the socket stream 
    return SocketStream::apply (zobj, nset, quark, argv);
  }
}
