// ---------------------------------------------------------------------------
// - Predphy.cpp                                                             -
// - afnix:phy service - predicates implementation                           -
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
#include "Unit.hpp"
#include "Boolean.hpp"
#include "Predphy.hpp"
#include "Physics.hpp"
#include "Periodic.hpp"
#include "Exception.hpp"

namespace afnix {

  // this procedure checks that we have one argument only and returns
  // the evaluated object
  static inline Object* get_obj (Evaluable* zobj, Nameset* nset, Cons* args,
                                 const String& pname) {
    Object* car = nullptr;
    if ((args == nullptr) || (args->length () != 1))
      throw Exception ("argument-error", "illegal arguments with predicate",
                       pname);
    car = args->getcar ();
    return (car == nullptr) ? nullptr : car->eval (zobj,nset);
  }

  // unitp: unit object predicate

  Object* phy_unitp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "unit-p");
    bool result = (dynamic_cast <Unit*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // physp: physics object predicate

  Object* phy_physp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "physics-p");
    bool result = (dynamic_cast <Physics*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // atom: atom object predicate

  Object* phy_atomp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "atom-p");
    bool result = (dynamic_cast <Atom*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // ptbl: periodic table object predicate

  Object* phy_ptblp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "periodic-p");
    bool result = (dynamic_cast <Periodic*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
