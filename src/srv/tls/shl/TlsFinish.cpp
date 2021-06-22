// ---------------------------------------------------------------------------
// - TlsFinish.cpp                                                           -
// - afnix:tls service - tls finished message class implementation           -
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
#include "TlsTypes.hxx"
#include "TlsFinish.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default finish message

  TlsFinish::TlsFinish (void) {
    reset ();
  }

  // create a finish by message block

  TlsFinish::TlsFinish (TlsHblock* hblk) {
    decode (hblk);
  }
  
  // destroy this  block
  
  TlsFinish::~TlsFinish (void) {
    reset ();
  }

  // return the class name
  
  String TlsFinish::repr (void) const {
    return "TlsFinish";
  }

  // reset the finished message

  void TlsFinish::reset (void) {
    wrlock ();
    try {
      d_vbuf.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the finished info as a plist

  Plist TlsFinish::getinfo (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist plst;
      // here it is
      unlock ();
      return plst;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // decode the message block

  void TlsFinish::decode (TlsHblock* hblk) {
    wrlock ();
    try {
      // reset eveything
      reset ();
      if (hblk == nullptr) {
	unlock ();
	return;
      }
      // map the handshake to a buffer
      d_vbuf = hblk->tobuffer();
      // check for valid length
      if (d_vbuf.length () != TLS_SIZE_FIN) {
	throw Exception ("tls-error", "invalid finished block size");
      }
      // done
      unlock ();
    } catch (...) {
      reset ();
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* TlsFinish::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsFinish;
    // check for 1 argument
    if (argc == 1) {
      Object*     obj = argv->get (0);
      TlsHblock* hblk = dynamic_cast<TlsHblock*> (obj);
      if ((hblk == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", 
			 "invalid object as tls finished message",
			 Object::repr (obj));
      }
      return new TlsFinish (hblk);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls finished message");
  }
}
