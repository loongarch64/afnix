// ---------------------------------------------------------------------------
// - Class.cpp                                                               -
// - afnix engine - class class implementation                               -
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
#include "Class.hpp"
#include "Boolean.hpp"
#include "Builtin.hpp"
#include "Instance.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                       -
  // -------------------------------------------------------------------------

  // create a new class object

  Class::Class (void) {
    p_infer = nullptr;
    p_defer = nullptr;
    d_ctinf = false;
    d_ctdef = false;
    Object::iref (p_cset = new Localset);
  }

  // destroy this class object

  Class::~Class (void) {
    p_cset->reset ();
    Object::dref (p_cset);
    Object::dref (p_infer);
    Object::dref (p_defer);
  }

  // return the class name

  String Class::repr (void) const {
    return "Class";
  }

  // return an object in the class locally
    
  Object* Class::get (const long quark) const {
    rdlock ();
    try {
      // check in the localset first
      Object* result = p_cset->get (quark);
      // check in the deferent class
      if ((result == nullptr) && (p_defer != nullptr)) {
	result = p_defer->get (quark);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the class inferent object

  Object* Class::setinfer (Object* object, const bool flag) {
    wrlock ();
    try {
      // check for a class
      Class* infer = dynamic_cast <Class*> (object);
      if ((infer == nullptr) && (object != nullptr)) {
	throw Exception ("type-error", 
			 "invalid object to set as inferent class",
			 Object::repr (object));
      }
      Object* result = setinfer (infer, flag);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the class inferent value

  Object* Class::setinfer (Class* infer, const bool flag) {
    wrlock ();
    try {
      if (d_ctinf == true) {
	throw Exception ("const-error", "const violation with infer member");
      }
      Object::iref (infer);
      Object::dref (p_infer);
      p_infer = infer;
      d_ctinf = flag;
      unlock ();
      return infer;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the class inferent object

  Object* Class::getinfer (void) const {
    rdlock ();
    try {
      Object* result = p_infer;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // unreference the class inferent value

  Object* Class::uninfer (void) {
    wrlock ();
    try {
      Object::dref (p_infer);
      p_infer = nullptr;
      d_ctinf = false;
      unlock ();
      return nullptr;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the class deferent object

  Object* Class::setdefer (Object* object, const bool flag) {
    wrlock ();
    try {
      // check for a class
      Class* defer = dynamic_cast <Class*> (object);
      if ((defer == nullptr) && (object != nullptr)) {
	throw Exception ("type-error", 
			 "invalid object to set as deferent class",
			 Object::repr (object));
      }
      Object* result = setdefer (defer, flag);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the class deferent value

  Object* Class::setdefer (Class* defer, const bool flag) {
    wrlock ();
    try {
      if (d_ctdef == true) {
	throw Exception ("const-error", "const violation with defer member");
      }
      Object::iref (defer);
      Object::dref (p_defer);
      p_defer = defer;
      d_ctdef = flag;
      unlock ();
      return defer;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the class deferent object

  Object* Class::getdefer (void) const {
    rdlock ();
    try {
      Object* result = p_defer;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // unreference the class deferent object

  Object* Class::undefer (void) {
    wrlock ();
    try {
      Object::dref (p_defer);
      p_defer = nullptr;
      d_ctdef = false;
      unlock ();
      return nullptr;
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
  static const long QUARK_INFER = zone.intern ("infer");
  static const long QUARK_DEFER = zone.intern ("defer");

  // return true if the given quark is defined

  bool Class::isquark (const long quark, const bool hflg) const {
    rdlock ();
    // check in the quark zone
    if (zone.exists(quark) == true) {
      unlock ();
      return true;
    }
    // check in the localset
    if (p_cset->exists (quark) == true) {
      unlock ();
      return true;
    }
    // check in the deferent class
    if (p_defer != nullptr) {
      bool result = p_defer->isquark (quark, hflg);
      unlock ();
      return result;
    }
    // check in the class base object
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // operate this class with another object

  Object* Class::oper (t_oper type, Object* object) {
    Class* cobj = dynamic_cast <Class*> (object);
    switch (type) {
    case Object::OPER_EQL:
    case Object::OPER_QEQ:
      if (cobj != nullptr) return new Boolean (this == cobj);
      break;
    case Object::OPER_NEQ:
      if (cobj != nullptr) return new Boolean (this != cobj);
      break;
    default:
      throw Exception ("operator-error", "unsupported class operator");
    }
    throw Exception ("type-error", "invalid operand with class",
		     Object::repr (object));
  }

  // create a new class symbol by quark

  Object* Class::cdef (Evaluable* zobj, Nameset* nset, const long quark, 
		       Object* object) {
    // check for reserved quark
    if (quark == QUARK_INFER) return setinfer (object, true);
    if (quark == QUARK_DEFER) return setdefer (object, true);
    // lock and process
    arlock ();
    try {
      // check in the localset
      Object* result = p_cset->cdef (zobj, nset, quark, object);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create or set a new class symbol by quark

  Object* Class::vdef (Evaluable* zobj, Nameset* nset, const long quark, 
		       Object* object) {
    // check for reserved quark
    if (quark == QUARK_INFER) return setinfer (object, false);
    if (quark == QUARK_DEFER) return setdefer (object, false);
    // lock and process
    arlock ();
    try {
      // check in the localset
      Object* result = p_cset->vdef (zobj, nset, quark, object);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // unreference a class symbol by quark

  Object* Class::udef (Evaluable* zobj, Nameset* nset, const long quark) {
    // check for reserved quark
    if (quark == QUARK_INFER) return uninfer ();
    if (quark == QUARK_DEFER) return undefer ();
    // lock and process
    arlock ();
    try {
      // check in the localset
      Object* result = p_cset->udef (zobj, nset, quark);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // evaluate an object member by quark

  Object* Class::eval (Evaluable* zobj, Nameset* nset, const long quark) {
    arlock ();
    try {
      // check for inferent class
      if (quark == QUARK_INFER) {
	Object* result = p_infer;
	zobj->post (result);
	unlock ();
	return result;
      }
      // check for deferent class
      if (quark == QUARK_DEFER) {
	Object* result = p_defer;
	zobj->post (result);
	unlock ();
	return result;
      }
      // look in the class localset
      Object* obj = p_cset->get (quark);
      if (obj != nullptr) {
	Object* result = obj->eval (zobj, nset);
	zobj->post (result);
	unlock ();
	return result;
      }
      // check in the deferent class
      if (p_defer != nullptr) {
	Object* result = p_defer->eval (zobj, nset, quark);
	zobj->post (result);
	unlock ();
	return result;
      }
      // evaluate as a quark method
      unlock ();
      return new Combo (quark, this, true);
    } catch ( ...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments

  Object* Class::apply (Evaluable* zobj, Nameset* nset, Cons* args) {
    arlock ();
    try {
      // create an instance with a class
      Instance* inst = new Instance (this);
      // loop with the inferent class
      Class* infer = p_infer;
      // check for an inferent class
      while (infer != nullptr) {
	// create an inferent instance
	Instance* ii = new Instance (infer);
	// set the super instance
	ii->setsuper (inst, infer->d_ctinf);
	// here is the new result
	inst = ii;
	// get the next inferent class
	infer = infer->p_infer;
      };
      // call the preset method
      Object* result = inst->pdef (zobj, nset, args);
      Object::iref (result);
      zobj->post (inst);
      Object::dref (result);
      unlock ();
      return inst;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply a this object function with a set of arguments and a quark

  Object* Class::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Cons* args) {
    // evaluate the object
    Object* obj = eval (zobj, nset, quark);
    // apply with arguments
    return (obj == nullptr) ? nullptr : obj->apply (zobj, nset, args);
  }

  // -------------------------------------------------------------------------
  // - builtin section                                                       -
  // -------------------------------------------------------------------------

  // create a new class object - this is the builtin function

  Object* Builtin::sfclass (Evaluable* zobj, Nameset* nset, Cons* args) {
    long len  = (args == nullptr) ? 0 : (args->length ());
    // process 0 arguments
    if (len == 0) return new Class;
    // too many arguments
    throw Exception ("argument-error",
		     "too many arguments with class definition");
  }
}
