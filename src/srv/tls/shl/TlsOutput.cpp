// ---------------------------------------------------------------------------
// - TlsOutput.cpp                                                           -
// - afnix:tls service - tls output stream class implementation              -
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
#include "Evaluable.hpp"
#include "TlsAlert.hpp"
#include "TlsOutput.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a tls output stream by state

  TlsOutput::TlsOutput (OutputStream* os, TlsState* sta) {
    // save the tls state
    Object::iref (p_tlss = sta);
    // bind the tls protocol
    Object::iref (p_tlsp = TlsProto::create (sta));
    // save the stream
    Object::iref (p_os = os);
  }

  // destroy this tls stream

  TlsOutput::~TlsOutput (void) {
    reset ();
  }

  // return the class name
  
  String TlsOutput::repr (void) const {
    return "TlsOutput";
  }

  // reset this tls input stream

  void TlsOutput::reset (void) {
    wrlock ();
    try {
      Object::dref (p_os);   p_os = nullptr;
      Object::dref (p_tlss); p_tlss = nullptr;
      Object::dref (p_tlsp); p_tlsp = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls output stream info

  Plist TlsOutput::getinfo (void) const {
    rdlock ();
    try {
      Plist result;
      if (p_tlss != nullptr) result = p_tlss->getinfo();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // close this tls output stream

  bool TlsOutput::close (void) {
    wrlock ();
    try {
      Object::dref (p_os); p_os = nullptr;
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a character to the output sream

  long TlsOutput::write (const char value) {
    wrlock ();
    try {
      // create a one character buffer
      Buffer buf; buf.add (value);
      // push the buffer
      long result = (p_tlsp == nullptr) ? 0L : p_tlsp->pushb(p_os, p_tlss, buf);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a buffer to the output stream

  long TlsOutput::write (const char* value) {
    wrlock ();
    try {
      // create a character buffer
      Buffer buf(value);
      // push the buffer
      long result = (p_tlsp == nullptr) ? 0L : p_tlsp->pushb(p_os, p_tlss, buf);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the tls state
  
  TlsState* TlsOutput::getstate (void) const {
    rdlock ();
    try {
      TlsState* result = p_tlss;
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
  static const long QUARK_GETSTATE = zone.intern ("get-state");
 
  // create a new object in a generic way

  Object* TlsOutput::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 2 arguments
    if (argc == 2) {
      Object* obj = argv->get (0);
      // check for an output stream
      OutputStream* os = dynamic_cast<OutputStream*> (obj);
      if (os == nullptr) {
	throw Exception ("type-error", "invalid object as output stream",
			 Object::repr (obj));
      }
      // check for a state
      obj = argv->get (1);
      TlsState* sta =  dynamic_cast<TlsState*> (obj);
      if (sta == nullptr) {
	throw Exception ("type-error", "invalid object as tls state",
			 Object::repr (obj));
      }
      return new TlsOutput (os, sta);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls output stream constructor");
  }

  // return true if the given quark is defined

  bool TlsOutput::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? TlsInfos::isquark (quark, hflg) : false;
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
  
  Object* TlsOutput::apply (Evaluable* zobj, Nameset* nset, const long quark,
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
    // fallback on the output stream 
    return OutputStream::apply (zobj, nset, quark, argv);
  }
}
