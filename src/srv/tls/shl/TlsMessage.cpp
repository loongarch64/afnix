// ---------------------------------------------------------------------------
// - TlsMessage.cpp                                                          -
// - afnix:tls service - tls message record class implementation             -
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
#include "TlsShake.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty message

  TlsMessage::TlsMessage (void) {
    p_brcd = nullptr;
  }

  // create a message by record

  TlsMessage::TlsMessage (TlsRecord* rcd) {
    Object::iref (p_brcd = rcd);
  }

  // create a message by record type and version

  TlsMessage::TlsMessage (const t_byte type, 
			  const t_byte vmaj, const t_byte vmin) {
    // bind a new record
    Object::iref (p_brcd = new TlsRecord (type, vmaj, vmin));
  }

  // copy construct this message

  TlsMessage::TlsMessage (const TlsMessage& that) {
    that.rdlock ();
    try {
      Object::iref (p_brcd = that.p_brcd);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // move construct this message

  TlsMessage::TlsMessage (TlsMessage&& that) noexcept {
    that.wrlock ();
    try {
      p_brcd = that.p_brcd; that.p_brcd = nullptr;
      that.unlock ();
    } catch (...) {
      p_brcd = nullptr;
      that.unlock ();
    } 
  }

  // destroy this message

  TlsMessage::~TlsMessage (void) {
    Object::dref (p_brcd);
  }


  // assign a message to this one

  TlsMessage& TlsMessage::operator = (const TlsMessage& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Object::iref (that.p_brcd);
      Object::dref (p_brcd);
      p_brcd = that.p_brcd;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // move a message to this one

  TlsMessage& TlsMessage::operator = (TlsMessage&& that) {
    // check for self-move
    if (this == &that) return *this;
    wrlock ();
    that.wrlock ();
    try {
      // move locally
      Object::dref (p_brcd); p_brcd = that.p_brcd; that.p_brcd = nullptr;
      unlock ();
      that.unlock ();
    } catch (...) {
      that.p_brcd = nullptr;
      unlock ();
      that.unlock ();
    }
    return *this;
  }
  
  // return the class name
  
  String TlsMessage::repr (void) const {
    return "TlsMessage";
  }

  // clone this object

  Object* TlsMessage::clone (void) const {
    return new TlsMessage (*this);
  }

  // reset the message

  void TlsMessage::reset (void) {
    wrlock ();
    try {
      if (p_brcd != nullptr) p_brcd->reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the header type

  t_byte TlsMessage::gettype (void) const {
    rdlock ();
    try {
      t_byte result = (p_brcd == nullptr) ? nilc : p_brcd->gettype ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the record length

  long TlsMessage::length (void) const {
    rdlock ();
    try {
      long result = (p_brcd == nullptr) ? 0L : p_brcd->length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a message byte by position

  t_byte TlsMessage::getbyte (const long pos) const {
    rdlock ();
    try {
      t_byte result = (p_brcd == nullptr) ? nilc : p_brcd->getbyte (pos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the message major version

  t_byte TlsMessage::getmajor (void) const {
    rdlock ();
    try {
      t_byte result = (p_brcd == nullptr) ? nilc : p_brcd->getmajor ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the message minor version

  t_byte TlsMessage::getminor (void) const {
    rdlock ();
    try {
      t_byte result = (p_brcd == nullptr) ? nilc : p_brcd->getminor ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a chunk block to the message
  
  void TlsMessage::add (const TlsChunk& chnk) {
    wrlock ();
    try {
      // check for nil record
      if (p_brcd == nullptr) {
	throw Exception ("tls-error", "cannot add chunk block in nil record");
      }
      p_brcd->add (chnk);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // push the record data into a buffer

  Buffer& TlsMessage::pushb (Buffer& buf) const {
    rdlock ();
    try {
      Buffer& result = (p_brcd != nullptr) ? p_brcd->pushb (buf) : buf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // write a message to the output stream
  
  long TlsMessage::write (OutputStream* os) {
    wrlock ();
    try {
      // check for nil record
      if (p_brcd == nullptr) {
	throw Exception ("tls-error", "cannot write message with nil record");
      }
      long result = p_brcd->write (os);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // write a message to the output stream
  
  long TlsMessage::write (OutputStream* os, TlsCodec* ec) {
    wrlock ();
    try {
      // check for nil record
      if (p_brcd == nullptr) {
	throw Exception ("tls-error", "cannot write message with nil record");
      }
      long result = p_brcd->write (os, ec);
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

  Object* TlsMessage::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsMessage;
    // check for 1 argument
    if (argc == 1) {
      Object*    obj = argv->get (0);
      TlsRecord* rcd = dynamic_cast<TlsRecord*> (obj);
      if ((rcd == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object as tls record",
			 Object::repr (obj));
      }
      return new TlsMessage (rcd);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls message constructor");
  }
}
