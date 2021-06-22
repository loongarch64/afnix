// ---------------------------------------------------------------------------
// - TlsInput.cpp                                                            -
// - afnix:tls service - tls input stream class implementation               -
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
#include "TlsInput.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a tls input stream by state

  TlsInput::TlsInput (InputStream* is, TlsState* sta) {
    // save the tls state
    Object::iref (p_tlss = sta);
    // bind the tls proto
    Object::iref (p_tlsp = TlsProto::create (sta));
    // save the stream
    Object::iref (p_is = is);
  }

  // destroy this tls input

  TlsInput::~TlsInput (void) {
    reset ();
  }

  // return the class name
  
  String TlsInput::repr (void) const {
    return "TlsInput";
  }

  // reset this tls input stream

  void TlsInput::reset (void) {
    wrlock ();
    try {
      Object::dref (p_is);   p_is = nullptr;
      Object::dref (p_tlss); p_tlss = nullptr;
      Object::dref (p_tlsp); p_tlsp = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls input stream info

  Plist TlsInput::getinfo (void) const {
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

  // close this tls input stream

  bool TlsInput::close (void) {
    wrlock ();
    try {
      Object::dref (p_is); p_is = nullptr;
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the stream timeout

  long TlsInput::gettout (void) const {
    rdlock ();
    try {
      long result = (p_is == nullptr) ? -1L : p_is->gettout ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // set the stream timeout

  void TlsInput::settout (const long tout) {
    wrlock ();
    try {
      if (p_is != nullptr) p_is->settout (tout);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the eos flag is set
  
  bool TlsInput::iseos (void) const {
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
  
  bool TlsInput::valid (void) const {
    wrlock ();
    try {
      // check for a protocol
      if (p_tlsp == nullptr) {
	throw Exception ("tls-error", "nil protocol for tls input");
      }
      // check local buffer
      if (d_sbuf.length () != 0) {
	unlock ();
	return true;
      }
      // check stream validity
      bool result = (p_is == nullptr) ? true : p_is->valid ();
      // eventually check that a message is not empty
      if (result == true) {
	Buffer buf = p_tlsp->popb (p_is, p_tlss);
	d_sbuf.add (buf);
	result = buf.empty () ? false : true;
	if (result == false) result = valid ();
      }
      unlock ();
      return result;
    } catch (const TlsAlert& alt) {
      if (alt.getdesc () == TlsAlert::TLS_DESC_CLS) {
	const_cast<TlsInput*>(this)->close ();
	unlock ();
	return false;
      }
      unlock ();
      throw;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // read one character from the input stream

  char TlsInput::read (void) {
    wrlock ();
    try {
      // check valid stream
      if (p_is == nullptr) {
	throw Exception ("tls-error", "invalid nil input stream in read");
      }
      // check the pushback buffer first
      if (d_sbuf.empty () == false) {
	char result = d_sbuf.read ();
	unlock ();
	return result;
      }
      // check if we can read a character
      if (p_is->valid () == false) {
	unlock ();
	return eosc;
      }
      // add the next available message buffer
      Buffer buf = p_tlsp->popb (p_is, p_tlss);
      d_sbuf.add (buf);
      // read the next character
      char result = d_sbuf.empty () ? eosc : d_sbuf.read ();
      unlock ();
      return result;
    } catch (const TlsAlert& alt) {
      if (alt.getdesc () == TlsAlert::TLS_DESC_CLS) {
	close ();
	unlock ();
	return eosc;
      }
      unlock ();
      throw;
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // get the tls state

  TlsState* TlsInput::getstate (void) const {
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

  Object* TlsInput::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 2 arguments
    if (argc == 2) {
      Object* obj = argv->get (0);
      // check for an input stream
      InputStream* is = dynamic_cast<InputStream*> (obj);
      if (is == nullptr) {
	throw Exception ("type-error", "invalid object as input stream",
			 Object::repr (obj));
      }
      // check for a state
      obj = argv->get (1);
      TlsState* sta =  dynamic_cast<TlsState*> (obj);
      if (sta == nullptr) {
	throw Exception ("type-error", "invalid object as tls state",
			 Object::repr (obj));
      }
      return new TlsInput (is, sta);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls input stream constructor");
  }

  // return true if the given quark is defined

  bool TlsInput::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? TlsInfos::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? InputStream::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* TlsInput::apply (Evaluable* zobj, Nameset* nset, const long quark,
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
    // fallback on the input stream 
    return InputStream::apply (zobj, nset, quark, argv);
  }
}
