// ---------------------------------------------------------------------------
// - DupleixStream.cpp                                                       -
// - standard object library - dupleix stream class implementation           -
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
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "DupleixStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // return the class name

  String DupleixStream::repr (void) const {
    return "DupleixStream";
  }

  // get the input stream channel
  
  InputStream* DupleixStream::getis (void) {
    rdlock ();
    try {
      InputStream* result = this;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the output stream channel
  
  OutputStream* DupleixStream::getos (void) {
    rdlock ();
    try {
      OutputStream* result = this;
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
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_GETIS     = zone.intern ("get-input-stream");
  static const long QUARK_GETOS     = zone.intern ("get-output-stream");
  static const long QUARK_SETEMOD   = zone.intern ("set-encoding-mode");
  static const long QUARK_SETIEMOD  = zone.intern ("set-input-encoding-mode");
  static const long QUARK_GETIEMOD  = zone.intern ("get-input-encoding-mode");
  static const long QUARK_SETOEMOD  = zone.intern ("set-output-encoding-mode");
  static const long QUARK_GETOEMOD  = zone.intern ("get-output-encoding-mode");

  // local quarks
  static const long QUARK_GETEMOD   = String::intern ("get-encoding-mode");
  
  // return true if the given quark is defined
  
  bool DupleixStream::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? InputStream::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? OutputStream::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* DupleixStream::apply (Evaluable* zobj, Nameset* nset, const long quark,
				Vector* argv) {
    // check the special quark first
    if (quark == QUARK_SETIEMOD) {
      return InputStream::apply (zobj, nset, QUARK_SETEMOD, argv);
    }
    if (quark == QUARK_GETIEMOD) {
      return InputStream::apply (zobj, nset, QUARK_GETEMOD, argv);
    }
    if (quark == QUARK_SETOEMOD) {
      return OutputStream::apply (zobj, nset, QUARK_SETEMOD, argv);
    }    
    if (quark == QUARK_GETOEMOD) {
      return OutputStream::apply (zobj, nset, QUARK_GETEMOD, argv);
    }
    if (quark == QUARK_SETEMOD) {
      Object::cref (InputStream::apply  (zobj, nset, quark, argv));
      Object::cref (OutputStream::apply (zobj, nset, quark, argv));
      return nullptr;
    }
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETIS) {
	wrlock ();
	try {
	  Object* result = getis ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETOS) {
	wrlock ();
	try {
	  Object* result = getos ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // check for input stream quark
    if (InputStream::isquark (quark, true) == true)
      return InputStream::apply (zobj, nset, quark, argv);
    // check for output stream quark
    if (OutputStream::isquark (quark, true) == true)
      return OutputStream::apply (zobj, nset, quark, argv);
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
