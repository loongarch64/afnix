// ---------------------------------------------------------------------------
// - Promise.cpp                                                             -
// - afnix engine - promise class implementation                             -
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

#include "Engsid.hxx"
#include "Promise.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default promise

  Promise::Promise (void) {
    p_form = nullptr;
    p_eobj = nullptr;
    d_dlay = true;
  }

  // create a new promise with a form

  Promise::Promise (Object* form) {
    p_form = Object::iref (form);
    p_eobj = nullptr;
    d_dlay = true;
  }

  // destroy this promise

  Promise::~Promise (void) {
    Object::dref (p_form);
    Object::dref (p_eobj);
  }

  // return the class name

  String Promise::repr (void) const {
    return "Promise";
  }

  // return the serial did
  
  t_word Promise::getdid (void) const {
    return SRL_DEOD_ENG;
  }

  // return the serial sid

  t_word Promise::getsid (void) const {
    return SRL_PMIS_SID;
  }
  
  // serialize this promise

  void Promise::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the form
      if (p_form == nullptr) {
	Serial::wrnilid (os);
      } else {
	Serial* sobj = dynamic_cast <Serial*> (p_form);
	if (sobj == nullptr) {
	  throw Exception ("serial-error", "cannot serialize object", 
			   p_form->repr ());
	}
	sobj->serialize (os);
      }
      // serialize the evaluated object
      if (p_eobj == nullptr) {
	Serial::wrnilid (os);
      } else {
	Serial* sobj = dynamic_cast <Serial*> (p_eobj);
	if (sobj == nullptr) {
	  throw Exception ("serial-error", "cannot serialize object", 
			   p_eobj->repr ());
	}
	sobj->serialize (os);
      }
      // serialize the delay flag
      Boolean bval = d_dlay;
      bval.wrstream (os);
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this promise

  void Promise::rdstream (InputStream& is) {
    wrlock ();
    try {
      // clean the promise
      Object::dref (p_form);
      Object::dref (p_eobj);
      // get the form
      Object::iref (p_form = Serial::deserialize (is));
      // get the object
      Object::iref (p_eobj = Serial::deserialize (is));
      // get the delay flag
      d_dlay = Serial::rdbool (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // force the evaluation of this promise

  Object* Promise::force (Evaluable* zobj, Nameset* nset) {
    wrlock ();
    try {
      if (d_dlay == true) {
	p_eobj = (p_form == nullptr) ? nullptr : p_form->eval (zobj, nset);
	Object::iref (p_eobj);
	d_dlay  = false;
      }
      zobj->post (p_eobj);
      unlock ();
      return p_eobj;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // evaluate this promise

  Object* Promise::eval (Evaluable* zobj, Nameset* nset) {
    rdlock ();
    try {
      Object* result = d_dlay ? this : p_eobj;
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
