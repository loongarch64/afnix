// ---------------------------------------------------------------------------
// - Switch.cpp                                                              -
// - afnix engine - builtin selector function implementation                 -
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
#include "Builtin.hpp"
#include "Lexical.hpp"
#include "Boolean.hpp"
#include "Exception.hpp"

namespace afnix {

  // this procedure check if an object is a lexical and if yes, check if it 
  // is the else lexical
  static inline bool check_else (Object* object) {
    Lexical* lex = dynamic_cast <Lexical*> (object);
    if (lex == nullptr) return false;
    if (lex->tostring () == "else") return true;
    return false;
  }
  
  // evaluate a switch statement

  Object* Builtin::sfswitch (Evaluable* zobj, Nameset* nset, Cons* args) {
    // trivial check first
    if ((args == nullptr) || (args->length () != 2)) {
      throw Exception ("argument-error",
		       "missing or too many arguments with switch");
    }
    // extract selector and protect it
    Object* car = args->getcar ();
    Object* sel = (car == nullptr) ? nullptr : car->eval (zobj, nset);
    Object::iref (sel);
    
    // extract body
    Object* obj  = args->getcadr ();
    Cons*   body = dynamic_cast <Cons*> (obj);
    if (body == nullptr) {
      Object::dref (sel);
      throw Exception ("type-error", "illegal object as switch body",
		       Object::repr (obj));
    }

    // perform the selection
    while (body != nullptr) {
      // get a selection form
      Object* elem = body->getcar ();
      Cons*   sobj = dynamic_cast <Cons*> (elem);
      if (sobj == nullptr) {
	Object::dref (sel);
	throw Exception ("type-error", "illegal object as switch selector",
			 Object::repr (elem));
      }
      // extract the condition form
      Object* cond = sobj->getcar ();
      // check for else as a lexical
      if (check_else (cond) == true) {
	Object* form = sobj->getcadr ();
	Object* result = (form == nullptr) ? nullptr : form->eval (zobj,nset);
	Object::dref (sel);
	return result;
      }
      // check for equality between the selector and the condition
      Object*  data = (cond == nullptr) ? nullptr : cond->eval (zobj, nset);
      Object::iref (data);
      Object*  bobj =
	(sel == nullptr) ? nullptr:sel->oper (Object::OPER_EQL, data);
      Boolean* bval = dynamic_cast <Boolean*> (bobj);
      bool     bloc = (bval == nullptr) ? false : bval->tobool ();
      Object::cref (bval);
      if (bloc == true) {
	Object* form = sobj->getcadr ();
	Object* result = (form == nullptr) ? nullptr : form->eval (zobj,nset);
	Object::dref (sel);
	Object::dref (data);
	return result;
      }
      // no match - try next form
      Object::dref (data);
      body = body->getcdr ();
    }
    Object::dref (sel);
    return nullptr;
  }
}
