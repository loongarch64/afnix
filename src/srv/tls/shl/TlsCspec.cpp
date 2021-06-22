// ---------------------------------------------------------------------------
// - TlsCspec.cpp                                                            -
// - afnix:tls service - tls change [cipher spec] class implementation       -
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
#include "TlsTypes.hxx"
#include "TlsCspec.hpp"
#include "TlsHblock.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  TlsCspec::TlsCspec (void) {
    reset ();
  }
  
  // create a message by record

  TlsCspec::TlsCspec (TlsRecord* rcd) : TlsMessage (rcd) {
    // check valid type
    if (gettype () != TLS_TYPE_CCS) {
      throw Exception ("tls-error", "invalid type in change cipher spec");
    }
  }

  // create a message by type and version

  TlsCspec::TlsCspec (const t_byte vmaj, const t_byte vmin) :
    TlsMessage (TLS_TYPE_CCS ,vmaj, vmin) {
    // create a single byte chunk
    TlsChunk chk;
    chk.add (0x01);
    // add the chunk to the message
    add (chk);
  }

  // copy construct this message

  TlsCspec::TlsCspec (const TlsCspec& that) {
    that.rdlock ();
    try {
      // copy the message
      TlsMessage::operator = (that);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign a message to this one

  TlsCspec& TlsCspec::operator = (const TlsCspec& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy the message
      TlsMessage::operator = (that);
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // get the class name

  String TlsCspec::repr (void) const {
    return "TlsCspec";
  }

  // clone this object

  Object* TlsCspec::clone (void) const {
    return new TlsCspec (*this);
  }

  // reset the message

  void TlsCspec::reset (void) {
    wrlock ();
    try {
      // reset base message
      TlsMessage::reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the message info as a plist

  Plist TlsCspec::getinfo (void) const {
    rdlock ();
    try {
      // create a header result plist
      Plist plst = TlsHeader::getinfo ();
      // set the message info
      plst.add ("TLS-MSG-TYPE", "TLS CHANGE CIPHER SPEC", (t_long) gettype  ());
      // here it is
      unlock ();
      return plst;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* TlsCspec::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsCspec;
    // check for 1 argument
    if (argc == 1) {
      Object*    obj = argv->get (0);
      TlsRecord* rcd = dynamic_cast<TlsRecord*> (obj);
      if ((rcd == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object as tls record",
			 Object::repr (obj));
      }
      return new TlsCspec (rcd);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls change constructor");
  }
}
