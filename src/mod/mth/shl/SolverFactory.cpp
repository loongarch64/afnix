// ---------------------------------------------------------------------------
// - SolverFactory.cpp                                                       -
// - afnix:mth module - solver factory class implementation                  -
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
#include "Exception.hpp"
#include "SolverFactory.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default factory

  SolverFactory::SolverFactory (void) {
    p_slg = nullptr;
  }

  // create a factory by logger

  SolverFactory::SolverFactory (Logger* slg) {
    Object::iref (p_slg = slg);
  }

  // copy construct this factory

  SolverFactory::SolverFactory (const SolverFactory& that) {
    that.rdlock ();
    try {
      Object::iref (p_slg = that.p_slg);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this factory

  SolverFactory::~SolverFactory (void) {
    Object::dref (p_slg);
  }

  // assign a factory to this one

  SolverFactory& SolverFactory::operator = (const SolverFactory& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // protect the new object
      Object::iref (that.p_slg);
      // clean the old object
      Object::dref (p_slg);
      // assign locally
      p_slg = that.p_slg;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  // set the system logger

  void SolverFactory::setslg (Logger* slg) {
    wrlock ();
    try {
      Object::iref (slg);
      Object::dref (p_slg);
      p_slg = slg;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the solver logger

  Logger* SolverFactory::getslg (void) const {
    rdlock ();
    try {
      Logger* result = p_slg;
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
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_NEWSLV = zone.intern ("new-solver");
  static const long QUARK_SETSLG = zone.intern ("set-logger");
  static const long QUARK_GETSLG = zone.intern ("get-logger");

  // return true if the given quark is defined

  bool SolverFactory::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true){
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark
  
  Object* SolverFactory::apply (Evaluable* zobj, Nameset* nset, const long quark,
				Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_NEWSLV) return newslv ();
      if (quark == QUARK_GETSLG) {
	rdlock ();
	try {
	  Logger* result = getslg ();
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
      if (quark == QUARK_SETSLG) {
	Object* obj = argv->get (0);
	Logger* slg = dynamic_cast <Logger*> (obj);
	if ((slg == nullptr) && (obj != nullptr)){
	  throw Exception ("type-error", "invalid object with set-logger",
			   Object::repr (obj));
	}
	setslg (slg);
	return nullptr;
      }
    }
    // call the object methods
    return Object::apply (zobj, nset, quark, argv);
  }
}
