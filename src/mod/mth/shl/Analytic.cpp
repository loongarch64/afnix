// ---------------------------------------------------------------------------
// - Analytic.cpp                                                            -
// - afnix:mth module - analytic class implementation                        -
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

#include "Math.hpp"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Boolean.hpp"
#include "Analytic.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the analytic vector nan message
  static const String ANC_DBUG_NAN = "[afnix:mth:rvi] nan value at index ";
  // the analytic matrix null row message
  static const String ANC_DBUG_MNR = "[afnix:mth:rmi] null row at index ";
  // the analytic matrix null column message
  static const String ANC_DBUG_MNC = "[afnix:mth:rmi] null column at index ";
    
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default analytic

  Analytic::Analytic (void) {
    p_slg = nullptr;
  }

  // create a analytic by logger

  Analytic::Analytic (Logger* slg) {
    Object::iref (p_slg = slg);
  }

  // copy construct this analytic

  Analytic::Analytic (const Analytic& that) {
    that.rdlock ();
    try {
      Object::iref (p_slg = that.p_slg);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this analytic

  Analytic::~Analytic (void) {
    Object::dref (p_slg);
  }

  // assign a analytic to this one

  Analytic& Analytic::operator = (const Analytic& that) {
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

  // return the class name
  
  String Analytic::repr (void) const {
    return "Analytic";
  }

  // set the analytic logger

  void Analytic::setslg (Logger* slg) {
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

  // get the analytic logger

  Logger* Analytic::getslg (void) const {
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

  // check a vector for nan

  bool Analytic::isnan (const Rvi& x) const {
    rdlock ();
    try {
      // get vector size
      t_long size = x.getsize ();
      // preset result and check
      bool result = false;
      for (t_long k = 0LL; k < size; k++) {
	if (Math::isnan (x.get (k)) == true) {
	  if (p_slg != nullptr) {
	    String mesg = ANC_DBUG_NAN + Utility::tostring (k);
	    p_slg->add (Logger::MLVL_DBUG, mesg);
	  }
	  result = true;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check a matrix for null row

  bool Analytic::isnrow (const Rmi& m) const {
    rdlock ();
    try {
      // get matrix row size
      t_long rsiz = m.getrsiz ();
      // preset result and check
      bool result = false;
      for (t_long k = 0LL; k < rsiz; k++) {
	if (m.isnrow (k) == true) {
	  if (p_slg != nullptr) {
	    String mesg = ANC_DBUG_MNR + Utility::tostring (k);
	    p_slg->add (Logger::MLVL_DBUG, mesg);
	  }
	  result = true;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check a matrix for null column

  bool Analytic::isncol (const Rmi& m) const {
    rdlock ();
    try {
      // get matrix column size
      t_long csiz = m.getcsiz ();
      // preset result and check
      bool result = false;
      for (t_long k = 0LL; k < csiz; k++) {
	if (m.isncol (k) == true) {
	  if (p_slg != nullptr) {
	    String mesg = ANC_DBUG_MNC + Utility::tostring (k);
	    p_slg->add (Logger::MLVL_DBUG, mesg);
	  }
	  result = true;
	}
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
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_NANP   = zone.intern ("nan-p");
  static const long QUARK_NILRP  = zone.intern ("nil-row-p");
  static const long QUARK_NILCP  = zone.intern ("nil-col-p");
  static const long QUARK_SETSLG = zone.intern ("set-logger");
  static const long QUARK_GETSLG = zone.intern ("get-logger");

  // create a new object in a generic way
  
  Object* Analytic::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Analytic;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a logger
      Logger* slg = dynamic_cast <Logger*> (obj);
      if (slg != nullptr) return new Analytic (slg);
      // invalid type
      throw Exception ("type-error", "invalid object for analytic",
                       Object::repr (obj));
    }

    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with linear object");
  }

  // return true if the given quark is defined

  bool Analytic::isquark (const long quark, const bool hflg) const {
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
  
  Object* Analytic::apply (Evaluable* zobj, Nameset* nset, const long quark,
				Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
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
      if (quark == QUARK_NANP) {
	Object* obj = argv->get (0);
	Rvi* x = dynamic_cast <Rvi*> (obj);
	if (x == nullptr){
	  throw Exception ("type-error", "invalid object with nan-p",
			   Object::repr (obj));
	}
	return new Boolean (isnan (*x));
      }
      if (quark == QUARK_NILRP) {
	Object* obj = argv->get (0);
	Rmi* m = dynamic_cast <Rmi*> (obj);
	if (m == nullptr){
	  throw Exception ("type-error", "invalid object with nil-row-p",
			   Object::repr (obj));
	}
	return new Boolean (isnrow (*m));
      }
      if (quark == QUARK_NILCP) {
	Object* obj = argv->get (0);
	Rmi* m = dynamic_cast <Rmi*> (obj);
	if (m == nullptr){
	  throw Exception ("type-error", "invalid object with nil-col-p",
			   Object::repr (obj));
	}
	return new Boolean (isncol (*m));
      }
    }
    // call the object methods
    return Object::apply (zobj, nset, quark, argv);
  }
}
