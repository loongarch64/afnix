// ---------------------------------------------------------------------------
// - Operator.cpp                                                            -
// - afnix engine - operator builtin functions implementation                -
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
#include "Real.hpp"
#include "Global.hpp"
#include "Builtin.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // this procedure extract and evaluate the calling object for an operator
  static inline Object* getobj (Evaluable* zobj, Nameset* nset, Cons* args, 
				const char* opname) {
    Object* car = nullptr;
    Object* obj = nullptr;
    if ((args == nullptr) || (args->length () != 2)) {
      throw Exception ("argument-error",
		       "missing or too many arguments with operator", opname);
    }
    car = args->getcar ();
    obj = (car == nullptr) ? nullptr : car->eval (zobj,nset);
    if (obj == nullptr) {
      throw Exception ("type-error", "invalid nil object with operator",
		       opname);
    }
    return Object::iref (obj);
  }

  // this procedure evaluates the cadr and return it
  static inline Object* getarg (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* car = (args == nullptr) ? nullptr : args->getcadr ();
    Object* obj = (car == nullptr) ? nullptr : car->eval (zobj, nset);
    return Object::iref (obj);
  }

  // operator + 

  Object* Builtin::sfadd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the objects
    Object* obj = getobj (zobj, nset, args, "+");
    Object* arg = nullptr;
    try {
      arg = getarg (zobj, nset, args);
    } catch (...) {
      Object::dref (obj);
      throw;
    }
    // perform operation
    try {
      Object* result = obj->oper (Object::OPER_ADD, arg);
      zobj->post (result);
      Object::dref (obj);
      Object::dref (arg);
      return result;
    } catch (...) {
      Object::dref (obj);
      Object::dref (arg);
      throw;
    }
  }

  // operator -

  Object* Builtin::sfsub (Evaluable* zobj, Nameset* nset, Cons* args) {
    // check arguments
    long argc = 0;
    if ((args == nullptr) || (((argc = args->length ()) != 1) && (argc != 2))) {
      throw Exception ("argument-error",
		       "missing or too many arguments with operator -");
    }
    // evaluate first argument
    Object* car = args->getcar ();
    Object* obj = (car == nullptr) ? nullptr : car->eval (zobj, nset);
    if (obj == nullptr) {
      throw Exception ("type-error", "invalid nil object with operator -");
    }
    Object::iref (obj);
    // process unary operator
    if (argc == 1) {
      try {
	Object* result = obj->oper (Object::OPER_UMN, (Object*) nullptr);
	Object::dref (obj);
	zobj->post (result);
	return result;
      } catch (...) {
	Object::dref (obj);
	throw;
      }
    }
    // process normal operator
    Object* arg = nullptr;
    try {
      arg = getarg (zobj, nset, args);
    } catch (...) {
      Object::dref (obj);
      throw;
    }
    try {
      Object* result = obj->oper (Object::OPER_SUB, arg);
      zobj->post (result);
      Object::dref (obj);
      Object::dref (arg);
      return result;
    } catch (...) {
      Object::dref (obj);
      Object::dref (arg);
      throw;
    }
  }

  // operator * 

  Object* Builtin::sfmul (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the objects
    Object* obj = getobj (zobj, nset, args, "*");
    Object* arg = nullptr;
    try {
      arg = getarg (zobj, nset, args);
    } catch (...) {
      Object::dref (obj);
      throw;
    }
    // perform operation
    try {
      Object* result = obj->oper (Object::OPER_MUL, arg);
      zobj->post (result);
      Object::dref (obj);
      Object::dref (arg);
      return result;
    } catch (...) {
      Object::dref (obj);
      Object::dref (arg);
      throw;
    }
  }

  // operator /

  Object* Builtin::sfdiv (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the objects
    Object* obj = getobj (zobj, nset, args, "/");
    Object* arg = nullptr;
    try {
      arg = getarg (zobj, nset, args);
    } catch (...) {
      Object::dref (obj);
      throw;
    }
    // perform operation
    try {
      Object* result = obj->oper (Object::OPER_DIV, arg);
      zobj->post (result);
      Object::dref (obj);
      Object::dref (arg);
      return result;
    } catch (...) {
      Object::dref (obj);
      Object::dref (arg);
      throw;
    }
  }

  // operator ==

  Object* Builtin::sfeql (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the objects
    Object* obj = getobj (zobj, nset, args, "==");
    Object* arg = nullptr;
    try {
      arg = getarg (zobj, nset, args);
    } catch (...) {
      Object::dref (obj);
      throw;
    }
    // perform operation
    try {
      Object* result = obj->oper (Object::OPER_EQL, arg);
      zobj->post (result);
      Object::dref (obj);
      Object::dref (arg);
      return result;
    } catch (...) {
      Object::dref (obj);
      Object::dref (arg);
      throw;
    }
  }

  // operator !=

  Object* Builtin::sfneq (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the objects
    Object* obj = getobj (zobj, nset, args, "!=");
    Object* arg = nullptr;
    try {
      arg = getarg (zobj, nset, args);
    } catch (...) {
      Object::dref (obj);
      throw;
    }
    // perform operation
    try {
      Object* result = obj->oper (Object::OPER_NEQ, arg);
      zobj->post (result);
      Object::dref (obj);
      Object::dref (arg);
      return result;
    } catch (...) {
      Object::dref (obj);
      Object::dref (arg);
      throw;
    }
  }
  
  // operator >=

  Object* Builtin::sfgeq (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the objects
    Object* obj = getobj (zobj, nset, args, ">=");
    Object* arg = nullptr;
    try {
      arg = getarg (zobj, nset, args);
    } catch (...) {
      Object::dref (obj);
      throw;
    }
    // perform operation
    try {
      Object* result = obj->oper (Object::OPER_GEQ, arg);
      zobj->post (result);
      Object::dref (obj);
      Object::dref (arg);
      return result;
    } catch (...) {
      Object::dref (obj);
      Object::dref (arg);
      throw;
    }
  }
  
  // operator >

  Object* Builtin::sfgth (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the objects
    Object* obj = getobj (zobj, nset, args, ">");
    Object* arg = nullptr;
    try {
      arg = getarg (zobj, nset, args);
    } catch (...) {
      Object::dref (obj);
      throw;
    }
    // perform operation
    try {
      Object* result = obj->oper (Object::OPER_GTH, arg);
      zobj->post (result);
      Object::dref (obj);
      Object::dref (arg);
      return result;
    } catch (...) {
      Object::dref (obj);
      Object::dref (arg);
      throw;
    }
  }
  
  // operator <=

  Object* Builtin::sfleq (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the objects
    Object* obj = getobj (zobj, nset, args, "<=");
    Object* arg = nullptr;
    try {
      arg = getarg (zobj, nset, args);
    } catch (...) {
      Object::dref (obj);
      throw;
    }
    // perform operation
    try {
      Object* result = obj->oper (Object::OPER_LEQ, arg);
      zobj->post (result);
      Object::dref (obj);
      Object::dref (arg);
      return result;
    } catch (...) {
      Object::dref (obj);
      Object::dref (arg);
      throw;
    }
  }

  // operator <

  Object* Builtin::sflth (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the objects
    Object* obj = getobj (zobj, nset, args, "<");
    Object* arg = nullptr;
    try {
      arg = getarg (zobj, nset, args);
    } catch (...) {
      Object::dref (obj);
      throw;
    }
    // perform operation
    try {
      Object* result = obj->oper (Object::OPER_LTH, arg);
      zobj->post (result);
      Object::dref (obj);
      Object::dref (arg);
      return result;
    } catch (...) {
      Object::dref (obj);
      Object::dref (arg);
      throw;
    }
  }
  
  // process an assert command - this does nothing if the assert flag is false

  Object* Builtin::sfassert (Evaluable* zobj, Nameset* nset, Cons* args) {
    // process first object
    Object* car1 = (args == nullptr) ? nullptr : args->getcar ();
    Object* obj1 = (car1 == nullptr) ? nullptr : car1->eval (zobj, nset);
    Object::iref (obj1);
    // process object 2
    Object* car2 = (args == nullptr) ? nullptr : args->getcadr ();
    Object* obj2 = (car2 == nullptr) ? nullptr : car2->eval (zobj, nset);
    Object::iref (obj2);
    // process object 3
    Object* car3 = (args == nullptr) ? nullptr : args->getcaddr ();
    Object* obj3 = (car3 == nullptr) ? nullptr : car3->eval (zobj, nset);
    Object::iref (obj3);
    auto    bobj = dynamic_cast<Boolean*>(obj3);
    bool    bval = (bobj == nullptr) ? true : bobj->tobool ();
    Object::dref (obj3);
    // check for nil
    if ((obj1 == nullptr) && (obj2 == nullptr)) return nullptr;
    // do nothing if assert flag is false
    if (Global::getasrt () == false) {
      Object::dref (obj1);
      Object::dref (obj2);
      return nullptr;
    }
    // normal compare
    bool status = false;
    if (obj1 != nullptr) {
      Object*  aobj =
	obj1->oper (bval ? Object::OPER_EQL : Object::OPER_QEQ, obj2);
      Boolean* bobj = dynamic_cast <Boolean*> (aobj);
      status = bobj->tobool ();
      Object::cref (bobj);
    }
    Object::dref (obj1);
    Object::dref (obj2);
    if (status == true) return nullptr;
    Exception e ("assert-error");
    e.setabf (true);
    throw e;  
  }
}
