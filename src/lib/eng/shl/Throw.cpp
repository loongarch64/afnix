// ---------------------------------------------------------------------------
// - Throw.cpp                                                               -
// - afnix engine - builtin exception functions implementation               -
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
#include "Builtin.hpp"
#include "Evaluable.hpp"
#include "Localset.hpp"
#include "Exception.hpp"

namespace afnix {
  // basic quark const
  static const long QUARK_WHAT = String::intern ("what");

  // throw builtin function implementation
  Object* Builtin::sfthrow (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : (argv->length ());

    // check for 0 argument
    if (argc == 0) {
      delete argv;
      throw Exception ("user-exception");
    }

    // check for one argument
    if (argc == 1) {
      // check for an exception
      Exception* eobj = dynamic_cast <Exception*> (argv->get (0));
      if (eobj != nullptr) {
	Exception e = *eobj;
	delete argv;
	throw e;
      }
      try {
	String eid = argv->getstring (0);
	throw Exception (eid);
      } catch (...) {
	delete argv;
	throw;
      }
    }

    // check for two arguments
    if (argc == 2) {
      try {
	String eid = argv->getstring (0);
	String rsn = argv->getstring (1);
	throw Exception (eid, rsn);
      } catch (...) {
	delete argv;
	throw;
      }
    }

    // check for 3 arguments
    if (argc == 3) {
      try {
	String  eid = argv->getstring (0);
	String  rsn = argv->getstring (1);
	Object* obj = argv->get (2);
	throw Exception (eid, rsn, obj);
      } catch (...) {
	delete argv;
	throw;
      }
    }
    // invalid call
    throw Exception ("argument-error", "invalid argument with throw");
  }

  // try builtin function implementation

  Object* Builtin::sftry (Evaluable* zobj, Nameset* nset, Cons* args) {
    // check for arguments
    long argc = (args == nullptr) ? 0 : args->length ();
    if ((argc == 0) || (argc > 2)) {
      throw Exception ("argument-error", "invalid arguments with try");
    }
    Object* form = args->getcar ();

    // with 1 arguments we just run the form and catch any exception
    // the result is either the form result, the return/exception/any
    Object* result = nullptr;
    if (argc == 1) {
      try {
	result = (form == nullptr) ? nullptr : form->eval (zobj, nset);
	Object::iref (result);
      } catch (const Return& r) {
	throw r;
      } catch (const Exception& e) {
	result = Object::iref (e.getobj ());
      } catch (...) {
	result = nullptr;
      }
    }

    // with two arguments, we have a form to execute when an exception is
    // caught
    if (argc == 2) {
      Object* eform = args->getcadr ();
      try {
	result = (form == nullptr) ? nullptr : form->eval (zobj, nset);
	Object::iref (result);
      } catch (const Return& r) {
	throw r;
      } catch (const Exception& e) {
	Nameset* lset = new Localset (nset);
	Object::iref (lset);	
	lset->symcst (QUARK_WHAT, new Exception (e));
	try {
	  result = (eform == nullptr) ? nullptr : eform->eval (zobj, lset);
	  Object::iref (result);
	} catch (...) {
	  result = nullptr;
	  Object::dref (lset);
	  throw;
	}
	Object::dref (lset);
      } catch (...) {
	// create an internal exception
	Exception* e = new Exception ("internal-error", 
				      "internal exception fault");
	Nameset* lset = new Localset (nset);
	Object::iref (lset);	
	lset->symcst (QUARK_WHAT, e);
	try {
	  result = (eform == nullptr) ? nullptr : eform->eval (zobj, lset);
	  Object::iref (result);
	} catch (...) {
	  result = nullptr;
	  Object::dref (lset);
	  throw;
	}
	Object::dref (lset);
      }
    }
    zobj->post (result);
    Object::tref (result);
    return result;
  }
}
