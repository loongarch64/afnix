// ---------------------------------------------------------------------------
// - Closure.hpp                                                             -
// - afnix engine - closure class implementation                             -
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
#include "Vector.hpp"
#include "Return.hpp"
#include "Boolean.hpp"
#include "Lexical.hpp"
#include "Closure.hpp"
#include "Multiset.hpp"
#include "Evaluable.hpp"
#include "Reserved.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SELF = zone.intern ("self");

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the object eval quarks
  static const long QUARK_ARGS  = String::intern ("args");
  static const long QUARK_CONST = String::intern ("const");

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default closure
  
  Closure::Closure (void) {
    d_lflg = true;
    p_form = nullptr;
    Object::iref (p_cset = new Localset);
  }

  // create a default closure with a type
  
  Closure::Closure (const bool type) {
    d_lflg = type;
    p_form = nullptr;
    Object::iref (p_cset = new Localset);
  }

  // create a new closure 

  Closure::Closure (const bool type, Cons* argl, Object* form) {
    // save the arguments
    d_lflg = type;
    Object::iref (p_form = form);
    Object::iref (p_cset = new Localset);
    // add the arguments
    try {
      while (argl != nullptr) {
	addarg (argl->getcar ());
	argl = argl->getcdr ();
      }
    } catch (...) {
      Object::dref (p_cset);
      throw;
    }
  }

  // destroy this closure

  Closure::~Closure (void) {
    // reset before removal
    if (p_cset != nullptr) p_cset->reset ();
    // destroy object
    Object::dref (p_form);
    Object::dref (p_cset);
  }

  // return the class name

  String Closure::repr (void) const {
    return "Closure";
  }

  // add an argument by name to this closure

  void Closure::addarg (const long quark, const bool cflag) {
    wrlock ();
    try {
      // check if the argument already exists
      if (d_argl.exists (quark) == true) {
	throw Exception ("argument-error", "duplicate argument name", 
			 String::qmap (quark));
      }
      // get the argument flag
      bool argf = d_argl.exists (QUARK_ARGS);
      // check if the args flag is set
      if (argf == true) {
	throw Exception ("argument-error", "cannot add arguments after args");
      }
      // add the argument
      d_argl.add (quark, cflag);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an argument by object - we can either add a lexical name, a
  // form with a const flag or a string

  void Closure::addarg (Object* object) {
    wrlock ();
    try {
      String* sobj = dynamic_cast <String*> (object);
      if (sobj != nullptr) {
	addarg (sobj->toquark (), false);
	unlock ();
	return;
      }
      Lexical* lex = dynamic_cast <Lexical*> (object);
      if (lex != nullptr) {
	addarg (lex->toquark (), false);
	unlock ();
	return;
      }
      Cons* form = dynamic_cast <Cons*> (object);
      if (form != nullptr) {
	if (form->length () != 2) {
	  throw Exception ("argument-error", "invalid argument list form");
	}
	Reserved* crsv = dynamic_cast <Reserved*> (form->getcar  ());
	Lexical*  alex = dynamic_cast <Lexical*>  (form->getcadr ());
	if ((crsv == nullptr) || (alex == nullptr)) {
	  throw Exception ("argument-error", "invalid argument list form");
	}
	if (crsv->toquark () != QUARK_CONST) {
	  throw Exception ("argument-error", "invalid argument list form");
	}
	addarg (alex->toquark (), true);
	unlock ();
	return;
      }
      throw Exception ("argument-error", "invalid argument list form");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a closed variable to this closure

  void Closure::addclv (const long quark, const bool cflag, Object* object) {
    wrlock ();
    try {
      // check if the closed variable already exists
      if (p_cset->exists (quark) == true) {
	throw Exception ("argument-error", "duplicate closed variable", 
			 String::qmap (quark));
      }
      p_cset->symset (quark, cflag, object);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set this closure form

  void Closure::setform (Object* form) {
    wrlock ();
    try {
      Object::iref (form);
      Object::dref (p_form);
      p_form = form;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the closure is a lambda expression

  bool Closure::islambda (void) const {
    rdlock ();
    try {
      bool result = d_lflg;
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

  // the object supported quarks
  static const long QUARK_ADDARG  = zone.intern ("add-argument");
  static const long QUARK_GAMMAP  = zone.intern ("gamma-p");
  static const long QUARK_LAMBDAP = zone.intern ("lambda-p");
  static const long QUARK_SETFORM = zone.intern ("set-form");
  static const long QUARK_GETFORM = zone.intern ("get-form");

  // create a ew object in a generic way

  Object* Closure::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new Closure;
    // check for 1 argument
    if (argc == 1) {
      bool type = argv->getbool (0);
      return new Closure (type);
    }
    // illegal arguments
    throw Exception ("argument-error", "too many arguments with closure");
  }

  // return true if the given quark is defined

  bool Closure::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // set this object with a const object by quark

  Object* Closure::cdef (Evaluable* zobj, Nameset* nset, const long quark,
			 Object* object) {
    arlock ();
    try {
      Object* result = p_cset->cdef (zobj, nset, quark, object);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set this object with an object by quark

  Object* Closure::vdef (Evaluable* zobj, Nameset* nset, const long quark,
			 Object* object) {
    arlock ();
    try {
      Object* result = p_cset->vdef (zobj, nset, quark, object);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // unreference a object by quark

  Object* Closure::udef (Evaluable* zobj, Nameset* nset, const long quark) {
    arlock ();
    try {
      Object* result = p_cset->udef (zobj, nset, quark);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // evaluate this object

  Object* Closure::eval (Evaluable* zobj, Nameset* nset, const long quark) {
    arlock ();
    try {
      Object* result = nullptr;
      if (p_cset->exists (quark) == true) {
	result = p_cset->eval (zobj, nset, quark);
      } else {
	result = Object::eval (zobj, nset, quark);
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

  Object* Closure::apply (Evaluable* zobj, Nameset* nset, Cons* args) {
    arlock ();
    Multiset* mset = nullptr;
    try {
      // create the running multiset
      Object::iref (mset = new Multiset (true));
      // bind this closure in the localset
      mset->symcst (QUARK_SELF, this);
      // get the argument descriptors
      long argc = d_argl.length  ();
      bool argf = d_argl.exists  (QUARK_ARGS);
      bool acst = d_argl.isconst (QUARK_ARGS);
      // initialize argument counters
      long argm = argf ? argc - 1 : argc;
      long acnt = 0;
      // loop in the argument cells
      while (args != nullptr) {
	// check the argument count
	if (acnt == argm) break;
	// get the argument info
	long quark = d_argl.getquark (acnt);
	bool cflag = d_argl.getconst (acnt++);
	// evaluate thie argument value
	Object* car = args->getcar ();
	Object* obj = (car == nullptr) ? nullptr : car->eval (zobj, nset);
	// bind the object in the localset
	mset->symset (quark, cflag, obj);
	// next argument
	args = args->getcdr ();
      }
      // check for the remaining arguments
      if (args != nullptr) {
	// check is the args flag is set
	if (argf == false) {
	  throw Exception ("argument-error", "too many arguments at call");
	}
	Cons* larg = nullptr;
	try {
	  while (args != nullptr) {
	    Object* car = args->getcar ();
	    Object* arg = (car == nullptr) ? nullptr : car->eval (zobj, nset);
	    if (larg == nullptr) {
	      larg = new Cons (arg);
	    } else {
	      larg->add (arg);
	    }
	    args = args->getcdr ();
	  }
	} catch (...) {
	  delete larg;
	  throw;
	}
	mset->symset (QUARK_ARGS, acst, larg);
      } else {
	if (argf == true) {
	  mset->symset (QUARK_ARGS, acst, nullptr);
	} else if (acnt != argc) {
	  throw Exception ("argument-error", "missing arguments at call");
	}
      }
      // link the namesets
      if (d_lflg == true) { 
	mset->linkset (nset, p_cset);
      } else {
	mset->linkset (zobj->getgset (), p_cset);
      }
      // evaluate the result object
      Object* result = p_form->eval (zobj, mset);
      zobj->post (result);
      mset->reset  ();
      Object::dref (mset);
      unlock ();
      return result;
    } catch (const Return& r) {
      Object* result = r.getobj ();
      zobj->post (result);
      mset->reset  ();
      Object::dref (mset);
      unlock ();
      return result;
    } catch (...) {
      mset->reset  ();
      Object::dref (mset);
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Closure::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GAMMAP)  return new Boolean (!islambda ());
      if (quark == QUARK_LAMBDAP) return new Boolean ( islambda ());
      if (quark == QUARK_GETFORM) {
	rdlock ();
	Object* result = p_form;
	zobj->post (result);
	unlock ();
	return result;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETFORM) {
	Object* form = argv->get (0);
	setform (form);
	return nullptr;
      }
      if (quark == QUARK_ADDARG) {
	Object* arg = argv->get (0);
	addarg (arg);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
