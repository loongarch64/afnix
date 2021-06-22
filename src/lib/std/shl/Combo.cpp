// ---------------------------------------------------------------------------
// - Combo.cpp                                                               -
// - standard object library - combo object class implementation             -
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

#include "Cons.hpp"
#include "Combo.hpp"
#include "Evaluable.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a new combo with a quark
  
  Combo::Combo (const long quark, Object* czobj) {
    d_type      = QUARK;
    d_cval.qval = quark;
    p_zobj      = Object::iref (czobj);
    d_aflg      = false;
  }

  // create a new combo with an object

  Combo::Combo (Object* coval, Object* czobj) {
    d_type      = OBJECT;
    d_cval.oval = Object::iref (coval);
    p_zobj      = Object::iref (czobj);
    d_aflg      = false;
  }

  // create a new combo with a quark and an apply flag

  Combo::Combo (const long quark, Object* czobj, const bool aflag) {
    d_type      = QUARK;
    d_cval.qval = quark;
    p_zobj      = Object::iref (czobj);
    d_aflg      = aflag;
  }

  // create a new combo with an object and an apply flag

  Combo::Combo (Object* coval, Object* czobj, const bool aflg) {
    d_type      = OBJECT;
    d_cval.oval = Object::iref (coval);
    p_zobj      = Object::iref (czobj);
    d_aflg      = aflg;
  }

  // destroy this combo

  Combo::~Combo (void) {
    if (d_type == OBJECT) Object::dref (d_cval.oval);
    Object::dref (p_zobj);
  }

  // return the class name

  String Combo::repr (void) const {
    return "Combo";
  }

  // check if a combo is looping

  bool Combo::isequal (Object* obj) const {
    // map the combo object
    Combo* cobj = dynamic_cast <Combo*> (obj);
    if (cobj == nullptr) return false;
    // lock and compare
    rdlock ();
    cobj->rdlock ();
    try {
      // check the type
      if (d_type != cobj->d_type) {
	unlock ();
	return false;
      }
      // check the quark type
      if ((d_type == QUARK) && (d_cval.qval != cobj->d_cval.qval)) {
	unlock ();
	return false;
      }
      // check the object type
      if ((d_type == OBJECT) && (d_cval.oval != cobj->d_cval.oval)) {
	unlock ();
	return false;
      }
      // we are looping
      unlock ();
      cobj->unlock ();
      return true;
    } catch (...) {
      unlock ();
      cobj->unlock ();
      throw;
    }
  }

  // get the apply flag

  bool Combo::getaflg (void ) const {
    rdlock ();
    try {
      bool result = d_aflg;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the apply flag

  void Combo::setaflg (const bool aflg) {
    wrlock ();
    try {
      d_aflg = aflg;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // set an object as a const object by quark
  
  Object* Combo::cdef (Evaluable* zobj, Nameset* nset, const long quark,
		       Object* object) {
    wrlock ();
    try {
      Object* result = nullptr;
      if (d_type == OBJECT) {
	result = d_cval.oval->cdef (zobj, nset, quark, object);
      }
      if (d_type == QUARK) {
	throw Exception ("combo-error", "invalid cdef call in quark mode");
      }
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set an object by quark

  Object* Combo::vdef (Evaluable* zobj, Nameset* nset, const long quark,
		       Object* object) {
    wrlock ();
    try {
      Object* result = nullptr;
      if (d_type == OBJECT) {	
	result = d_cval.oval->vdef (zobj, nset, quark, object);
      }
      if (d_type == QUARK) {
	throw Exception ("combo-error", "invalid vdef call in quark mode");
      }
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // unreference an object by quark
  
  Object* Combo::udef (Evaluable* zobj, Nameset* nset, const long quark) {
    wrlock ();
    try {
      Object* result = nullptr;
      if (d_type == OBJECT) {
	result = d_cval.oval->udef (zobj, nset, quark);
      }
      if (d_type == QUARK) {
	throw Exception ("combo-error", "invalid udef call in quark mode");
      }
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // evaluate an object by quark

  Object* Combo::eval (Evaluable* zobj, Nameset* nset, const long quark) {
    // get the read lock
    rdlock ();
    try {
      Object* result = nullptr;
      if (d_type == QUARK) {
	Object* oval = p_zobj->eval (zobj, nset, d_cval.qval);
	Object::iref (oval);
	try {
	  if (isequal (oval) == true) {
	    // set local result
	    result = oval;
	    // adjust reference
	    Object::cref (oval);
	  } else {
	    // evaluate local result
	    result = (oval == nullptr)
	      ? nullptr
	      : oval->eval (zobj, nset, quark);
	    // clean intermediate object
	    Object::dref (oval);
	  }
	} catch (...) {
	  Object::dref (oval);
	  throw;
	}
      }
      if (d_type == OBJECT) {
	result = d_cval.oval;
      }
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments
  
  Object* Combo::apply (Evaluable* zobj, Nameset* nset, Cons* args) {
    // pre-evaluate argument list
    Cons* carg = nullptr;
    try {
      Object* result = nullptr;
      switch (d_type) {
      case Combo::QUARK:
	if (d_aflg == true) 
	  result = p_zobj->Object::apply (zobj, nset, d_cval.qval, args);
	else
	  result = p_zobj->apply (zobj, nset, d_cval.qval, args);
	break;
      case Combo::OBJECT:
	// evaluate the arguments as a list
	carg = Cons::eval (zobj, nset, args);
	// object evaluation combo
	if (d_aflg == true)
	  result = p_zobj->Object::apply (zobj, nset, d_cval.oval, carg);
	else
	  result = p_zobj->apply (zobj, nset, d_cval.oval, carg);
	break;
      }
      zobj->post (result);
      delete carg;
      return result;
    } catch (...) {
      delete carg;
      throw;
    }
  }
}
