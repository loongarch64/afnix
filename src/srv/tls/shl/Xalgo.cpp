// ---------------------------------------------------------------------------
// - Xalgo.cpp                                                               -
// - afnix:tls service - X509 algorithm class implementation                 -
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

#include "Xalgo.hpp"
#include "Vector.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default algorithm

  Xalgo::Xalgo (void) {
    p_aop = nullptr;
    reset ();
  }

  // create an algorithm by value and parameters

  Xalgo::Xalgo (const String& aid, AsnNode* aop) {
    p_aop = nullptr;
    set (aid, aop);
  }

  // copy construct this object

  Xalgo::Xalgo (const Xalgo& that) {
    that.rdlock ();
    try {
      d_aid = that.d_aid;
      Object::iref (p_aop = that.p_aop);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this algorithm

  Xalgo::~Xalgo (void) {
    Object::dref (p_aop);
  }

  // return the object class name

  String Xalgo::repr (void) const {
    return "Xalgo";
  }

  // return a clone of this object
  
  Object* Xalgo::clone (void) const {
    return new Xalgo (*this);
  }

  // assign an object to this one

  Xalgo& Xalgo::operator = (const Xalgo& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign locally
      d_aid = that.d_aid;
      Object::iref (that.p_aop);
      Object::dref (p_aop);
      p_aop = that.p_aop;
      // unlock and return
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset the algorithm

  void Xalgo::reset (void) {
    wrlock ();
    try {
      d_aid = "";
      Object::dref (p_aop); p_aop = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the algorithm identifier

  String Xalgo::getaid (void) const {
    rdlock ();
    try {
      String result = d_aid;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the algorithm by oid and parameters

  void Xalgo::set (const String& aid, AsnNode* aop) {
    wrlock ();
    try {
      // set the oid
      d_aid = aid;
      // protect and set the parameters
      Object::iref (aop);
      Object::dref (p_aop);
      p_aop = aop;
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SET    = zone.intern ("set");
  static const long QUARK_GETAID = zone.intern ("get-identifier");

  // create a new object in a generic way
 
  Object* Xalgo::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // create a default object
    if (argc == 0) return new Xalgo;
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with algorithm constructor");
  }

  // return true if the given quark is defined

  bool Xalgo::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark

  Object* Xalgo::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETAID) return new String (getaid ());
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SET) {
	String  aid = argv->getstring (0);
	Object* obj = argv->get (1);
	AsnNode* aop = dynamic_cast <AsnNode*> (obj);
	if ((obj != nullptr) && (aop == nullptr)) {
	  throw Exception ("type-error", 
			   "invalid object as algorithm parameter",
			   Object::repr (obj));
	}
	set (aid, aop);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
