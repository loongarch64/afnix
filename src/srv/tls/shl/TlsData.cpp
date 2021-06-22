// ---------------------------------------------------------------------------
// - TlsData.cpp                                                             -
// - afnix:tls service - tls application data class implementation       -
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
#include "TlsData.hpp"
#include "TlsTypes.hxx"
#include "TlsHblock.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  TlsData::TlsData (void) {
    reset ();
  }

  // create a message by record

  TlsData::TlsData (TlsRecord* rcd) : TlsMessage (rcd) {
    // check valid type
    if (gettype () != TLS_TYPE_APP) {
      throw Exception ("tls-error", "invalid type in applicatin data");
    }
  }

  // create a message by type and version

  TlsData::TlsData (const t_byte vmaj, const t_byte vmin) :
    TlsMessage (TLS_TYPE_APP ,vmaj, vmin) {
  }

  // copy construct this message

  TlsData::TlsData (const TlsData& that) {
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

  // assign a data to this one

  TlsData& TlsData::operator = (const TlsData& that) {
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

  String TlsData::repr (void) const {
    return "TlsData";
  }

  // clone this object

  Object* TlsData::clone (void) const {
    return new TlsData (*this);
  }

  // reset the message

  void TlsData::reset (void) {
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

  Plist TlsData::getinfo (void) const {
    rdlock ();
    try {
      // create a header result plist
      Plist plst = TlsHeader::getinfo ();
      // set the message info
      plst.add ("TLS-MSG-TYPE", "TLS APPLICATION DATA", (t_long) gettype  ());
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

  Object* TlsData::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsData;
    // check for 1 argument
    if (argc == 1) {
      Object*    obj = argv->get (0);
      TlsRecord* rcd = dynamic_cast<TlsRecord*> (obj);
      if ((rcd == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object as tls record",
			 Object::repr (obj));
      }
      return new TlsData (rcd);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls data constructor");
  }
}
