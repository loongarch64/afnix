// ---------------------------------------------------------------------------
// - Instance.cpp                                                            -
// - afnix engine - afnix instance class implementation                      -
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
#include "Vector.hpp"
#include "Closure.hpp"
#include "Multiset.hpp"
#include "Evaluable.hpp"
#include "Instance.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the instance eval quarks
  static const long QUARK_THIS   = String::intern ("this");
  static const long QUARK_META   = String::intern ("meta");
  static const long QUARK_MUTE   = String::intern ("mute");
  static const long QUARK_SUPER  = String::intern ("super");
  static const long QUARK_INFER  = String::intern ("infer");
  static const long QUARK_DEFER  = String::intern ("defer");
  static const long QUARK_PRESET = String::intern ("preset");

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default instance

  Instance::Instance (void) {
    Object::iref (p_iset  = new Localset);
    p_meta  = nullptr;
    d_ctmta = false;
    p_super = nullptr;
    d_ctsup = false;
    reset ();
  }

  // create an instance wih a meta class

  Instance::Instance (Class* meta) {
    Object::iref (p_iset  = new Localset);
    p_meta  = nullptr;
    d_ctmta = false;
    p_super = nullptr;
    d_ctsup = false;
    reset   ();
    setmeta (meta, false);
  }

  // destroy this instance

  Instance::~Instance (void) {
    reset ();
    Object::dref (p_iset);
    Object::dref (p_meta);
    Object::dref (p_super);
  }

  // return the class name

  String Instance::repr (void) const {
    return "Instance";
  }

  // reset this instance

  void Instance::reset (void) {
    wrlock ();
    try {
      Object::iref (this);
      p_iset->reset ();
      Object::tref (this);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the instance meta class

  Object* Instance::setmeta (Object* object, const bool flag) {
    wrlock ();
    try {
      // check for a class
      Class* meta = dynamic_cast <Class*> (object);
      if ((meta == nullptr) && (object != nullptr)) {
	throw Exception ("type-error", "invalid object to set as meta class",
			 Object::repr (object));
      }
      // check for constant
      if (d_ctmta == true) {
	throw Exception ("const-error", "const violation with meta class");
      }
      // set the meta class
      Object::iref (meta);
      Object::dref (p_meta);
      p_meta  = meta;
      d_ctmta = flag;
      unlock ();
      return meta;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // unreference the instance meta object

  Object* Instance::unmeta (void) {
    wrlock ();
    try {
      Object::dref (p_meta);
      p_meta  = nullptr;
      d_ctmta = false;
      unlock ();
      return nullptr;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the instance super value

  Object* Instance::setsuper (Object* object, const bool flag) {
    wrlock ();
    try {
      if (d_ctsup == true) {
	throw Exception ("const-error", "const violation with super member");
      }
      Object::iref (object);
      Object::dref (p_super);
      p_super = object;
      d_ctsup = flag;
      unlock ();
      return object;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // unreference the instance super object

  Object* Instance::unsuper (void) {
    wrlock ();
    try {
      Object::dref (p_super);
      p_super  = nullptr;
      d_ctsup = false;
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

  // create a new object in a generic way

  Object* Instance::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check 0 argument
    if (argc == 0) return new Instance;
    // illegal arguments
    throw Exception ("argument-error", "too many arguments with instance");
  }

  // return true if the given quark is defined

  bool Instance::isquark (const long quark, const bool hflg) const {
    // check for local quarks
    if (quark == QUARK_THIS)   return true;
    if (quark == QUARK_META)   return true;
    if (quark == QUARK_MUTE)   return true;
    if (quark == QUARK_SUPER)  return true;
    if (quark == QUARK_INFER)  return true;
    if (quark == QUARK_DEFER)  return true;
    if (quark == QUARK_PRESET) return true;
    // lock and check in the instance set
    rdlock ();
    try {
    if (p_iset->exists (quark) == true) {
      unlock ();
      return true;
    }
    // check in the class
    if ((p_meta != nullptr) && (p_meta->isquark (quark, hflg) == true)) {
      unlock ();
      return true;
    }
    // check in the super instance if hierarchical
    if (p_super != nullptr) {
      bool result = hflg ? p_super->isquark (quark, hflg) : false;
      unlock ();
      return result;
    }
    // check in the instance base object
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // preset the instance with a set of arguments

  Object* Instance::pdef (Evaluable* zobj, Nameset* nset, Cons* args) {
    wrlock ();
    Cons* carg = nullptr;
    try {
      // clean any remaining localset
      reset ();
      // check if we have a meta class
      if (p_meta == nullptr) {
	unlock ();
	return nullptr;
      }
      // evaluate the arguments in the original nameset
      carg = Cons::eval (zobj, nset, args);
      // get the preset form
      Object* iobj = p_meta->get (QUARK_PRESET);
      Object* form = (iobj == nullptr) ?
	nullptr : iobj->eval (zobj, nset);
      // compute the result
      Object* result = (form == nullptr) ?
	nullptr : apply (zobj, nset, form, carg);
      // clean evaluated args and unlock
      delete carg;
      unlock ();
      return result;
    } catch (...) {
      delete carg;
      unlock ();
      throw;
    }
  }

  // mute the instance with a set of arguments
  
  Object* Instance::mute (Evaluable* zobj, Nameset* nset, Cons* args) {
    wrlock ();
    try {
      // trivial check first
      if ((args == nullptr) || (args->length () < 1)) {
	throw Exception ("argument-error",
			 "missing or too many arguments with mute");
      }
      // try to get the class
      Object* car = args->getcar ();
      Object* obj = (car == nullptr) ? nullptr : car->eval (zobj, nset);
      Class*  cls = dynamic_cast <Class*> (obj);
      if (cls == nullptr) {
	throw Exception ("type-error", "invalid object to set as meta class",
			 Object::repr (obj));
      }
      // set the meta class
      setmeta (cls, false);
      // call preset with the remaining arguments
      Cons* cdr      = args->getcdr ();
      Object* result = pdef (zobj, nset, cdr);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a const object by quark

  Object* Instance::cdef (Evaluable* zobj, Nameset* nset, const long quark,
			  Object* object) {
    // check for the instance
    if (quark == QUARK_THIS) {
      throw Exception ("instance-error", "cannot bind this symbol");
    }
    // check for reserved quarks
    if (quark == QUARK_META)  return setmeta (object, true);
    if (quark == QUARK_SUPER) return setsuper (object, true);
    // lock and process
    arlock ();
    try {
      Object* result = p_iset->cdef (zobj, nset, quark, object);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create an object by quark

  Object* Instance::vdef (Evaluable* zobj, Nameset* nset, const long quark,
			  Object* object) {
    // check for the instance
    if (quark == QUARK_THIS) {
      throw Exception ("instance-error", "cannot bind this symbol");
    }
    // check for reserved quarks
    if (quark == QUARK_META)  return setmeta  (object, false);
    if (quark == QUARK_SUPER) return setsuper (object, false);
    // lock and process
    arlock ();
    try {
      // check in the instance localset
      Object* iobj = p_iset->get (quark);
      if (iobj != nullptr) {
	Object* result = iobj->vdef (zobj, nset, object);
	zobj->post (result);
	unlock ();
	return result;
      }
      // check in the class object
      if (p_meta != nullptr) {
	Object* cobj = p_meta->get (quark);
	if (cobj != nullptr) {
	  Object* result = cobj->vdef (zobj, nset, object);
	  zobj->post (result);
	  unlock ();
	  return result;
	}
      }
      // bind locally
      Object* result = p_iset->vdef (zobj, nset, quark, object);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // unreference an object by quark

  Object* Instance::udef (Evaluable* zobj, Nameset* nset, const long quark) {
    // check for the instance
    if (quark == QUARK_THIS) {
      throw Exception ("instance-error", "cannot unreference this symbol");
    }
    // check for reserved quarks
    if (quark == QUARK_META)  return unmeta  ();
    if (quark == QUARK_SUPER) return unsuper ();
    // lock and process
    arlock ();
    try {
      Object* result = p_iset->udef (zobj, nset, quark);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // evaluate an instance member

  Object* Instance::eval (Evaluable* zobj, Nameset* nset, const long quark) {
    arlock ();
    try {
      // check for this
      if (quark == QUARK_THIS) {
	Object* result = this;
	zobj->post (result);
	unlock ();
	return result;
      }
      // check for super 
      if (quark == QUARK_SUPER) {
	Object* result = p_super;
	zobj->post (result);
	unlock ();
	return result;
      }
      // check for infer 
      if (quark == QUARK_INFER) {
	Object* result = (p_meta == nullptr) ? nullptr : p_meta->getinfer ();
	zobj->post (result);
	unlock ();
	return result;
      }
      // check for defer 
      if (quark == QUARK_DEFER) {
	Object* result = (p_meta == nullptr) ? nullptr : p_meta->getdefer ();
	zobj->post (result);
	unlock ();
	return result;
      }
      // check for meta
      if (quark == QUARK_META) {
	Object* result = p_meta;
	zobj->post (result);
	unlock ();
	return result;
      }
      // check for mute
      if (quark == QUARK_MUTE) {
	unlock ();
	return new Combo (quark, this, false);
      }
      // check in the instance localset
      Object* iobj = p_iset->get (quark);
      if (iobj != nullptr) {
	// evaluate the result object
	Object* result = iobj->eval (zobj, nset);
	// check for not a closure
	if (dynamic_cast <Closure*> (result) == nullptr) {
	  zobj->post (result);
	  unlock ();
	  return result;
	}
	// bind to a combo
	Object* combo = new Combo (result, this);
	zobj->post (combo);
	unlock ();
	return combo;
      }
      // check in the class
      if (p_meta != nullptr) {
	Object* cobj = p_meta->get (quark);
	if (cobj != nullptr) {
	  // evaluate the result object
	  Object* result = cobj->eval (zobj, nset);
	  // check for not a closure
	  if (dynamic_cast <Closure*> (result) == nullptr) {
	    zobj->post (result);
	    unlock ();
	    return result;
	  }
	  // bind to a combo
	  Object* combo = new Combo (result, this);
	  zobj->post (combo);
	  unlock ();
	  return combo;
	}
      }
      // check in the super instance
      if (p_super != nullptr) {
	// evaluate the result object
	Object* result = p_super->eval (zobj, nset, quark);
	// check for not a closure
	if (dynamic_cast <Closure*> (result) == nullptr) {
	  zobj->post (result);
	  unlock ();
	  return result;
	}
	// bind to a combo
	Object* combo = new Combo (result, this);
	zobj->post (combo);
	unlock ();
	return combo;
      }
      // last resort is a combo
      Object* combo = new Combo (quark, this, true);
      zobj->post (combo);
      unlock ();
      return combo;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply an object with a set of arguments by quark

  Object* Instance::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Cons* args) {
    // check for mute quark
    if (quark == QUARK_MUTE) {
      wrlock ();
      try {
	Object* result = mute (zobj, nset, args);
	zobj->post (result);
	unlock ();
	return result;
      } catch (...) {
	unlock ();
	throw;
      }
    }
    // evaluate and apply
    Object* obj = eval (zobj, nset, quark);
    return apply (zobj, nset, obj, args);
  }
    
  // apply an object with an object within this instance

  Object* Instance::apply (Evaluable* zobj, Nameset* nset, Object* object,
			   Cons* args) {
    // check for a valid object
    if (object == nullptr) return nullptr;
    // bind a multiset and compute
    Multiset* mset = nullptr;
    try {
      // create a new multiset
      Object::iref (mset = new Multiset (nset, p_iset));
      // bind the this symbol
      mset->symcst (QUARK_THIS, this);
      // compute the result with the local multiset
      Object* result = object->apply (zobj, mset, args);
      // protect us before cleaning
      Object::iref (this);
      // post the result
      zobj->post (result);
      // clean the multiset
      if (mset != nullptr) mset->reset  ();
      Object::dref (mset);
      Object::tref (this);
      // here we are
      return result;
    } catch (...) {
      Object::iref (this);
      if (mset != nullptr) mset->reset  ();
      Object::dref (mset);
      Object::tref (this);
      throw;
    }
  }
}
