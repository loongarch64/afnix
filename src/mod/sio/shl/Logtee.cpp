// ---------------------------------------------------------------------------
// - Logtee.cpp                                                              -
// - afnix:sio module - message logging tee class implementation             -
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
#include "Logtee.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  static const bool DEF_TEE_MODE = false;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a default logtee 

  Logtee::Logtee (void) {
    p_tos  = nullptr;
    d_teef = DEF_TEE_MODE;
  }

  // create a logtee by size

  Logtee::Logtee (const long size) : Logger (size) {
    p_tos  = nullptr;
    d_teef = DEF_TEE_MODE;
  }

  // create a logtee with an output stream

  Logtee::Logtee (OutputStream* os) {
    Object::iref (p_tos = os);
    d_teef = DEF_TEE_MODE;
  }

  // create a logtee by size with an output stream

  Logtee::Logtee (const long size, OutputStream* os) : Logger (size) {
    Object::iref (p_tos = os);
    d_teef = DEF_TEE_MODE;
  }

  // destroy this logtee

  Logtee::~Logtee (void) {
    Object::dref (p_tos);
  }

  // return the class name

  String Logtee::repr (void) const {
    return "Logtee";
  }

  // set the logtee output stream

  void Logtee::settos (OutputStream* os) {
    wrlock ();
    try {
      Object::iref (os);
      Object::dref (p_tos);
      p_tos = os;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the logtee output stream

  OutputStream* Logtee::gettos (void) const {
    rdlock ();
    try {
      OutputStream* result = p_tos;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the tee flag

  void Logtee::setteef (const bool teef) {
    wrlock ();
    try {
      d_teef = teef;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the tee flag

  bool Logtee::getteef (void) const {
    rdlock ();
    try {
      bool result = d_teef;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a message in the logger by log level

  long Logtee::add (const t_mlvl mlvl, const String& mesg) {
    wrlock ();
    try {
      // add the message
      long result = Logger::add (mlvl, mesg);
      if (result == -1L) {
	unlock ();
	return result;
      }
      // format the message
      if ((p_tos != nullptr) && (d_teef == true)) {
	*p_tos << getfull (result) << eolc;
      }
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETEES = zone.intern ("set-tee-stream");
  static const long QUARK_GETEES = zone.intern ("get-tee-stream");
  static const long QUARK_SETTF  = zone.intern ("set-tee");
  static const long QUARK_GETTF  = zone.intern ("get-tee");

  // create a new object in a generic way

  Object* Logtee::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check 0 argument
    if (argc == 0) return new Logtee;
    // check 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for an integer
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj != nullptr) {
	long size = iobj->tolong ();
	return new Logtee (size);
      }
      // check for an output stream
      OutputStream* os = dynamic_cast <OutputStream*> (obj);
      if (os != nullptr) {
	return new Logtee (os);
      }
      throw Exception ("type-error", "invalid object with logtee",
		       Object::repr (obj));
    }
    // check 2 arguments
    if (argc == 2) {
      long  size = argv->getlong (0);
      Object* obj = argv->get (1);
      OutputStream* os = dynamic_cast <OutputStream*> (obj);
      if (os == nullptr) {
      throw Exception ("type-error", "invalid object with logtee",
		       Object::repr (obj));
      }
      return new Logtee (size, os);
    }
    throw Exception ("argument-error", "too many argument for logtee");
  }

  // return true if the given quark is defined
  
  bool Logtee::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Logger::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

    // apply this object class with a set of arguments and a quark
  
  Object* Logtee::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETTF) return new Boolean (getteef ());
      if (quark == QUARK_GETEES) {
	rdlock ();
	try {
	  Object* result = gettos ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETTF) {
	bool teef = argv->getbool (0);
	setteef (teef);
	return nullptr;
      }
      if (quark == QUARK_SETEES) {
	Object* obj = argv->get (0);
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os == nullptr) {
	  throw Exception ("type-error", "invalid object with set-tee-stream",
			   Object::repr (obj));
	}
	settos (os);
	return nullptr;
      }
    }
    // apply these arguments with the logger
    return Logger::apply (zobj, nset, quark, argv);
  }
}
