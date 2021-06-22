// ---------------------------------------------------------------------------
// - Predbce.cpp                                                             -
// - afnix:bce service - predicates implementation                           -
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
#include "Chain.hpp"
#include "Request.hpp"
#include "Predbce.hpp"
#include "Boolean.hpp"
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

  // tranp: transaction object predicate

  Object* bce_tranp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "transaction-p");
    bool result = (dynamic_cast <Transaction*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // blokp: block object predicate

  Object* bce_blokp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "block-p");
    bool result = (dynamic_cast <Block*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rqstp: request object predicate

  Object* bce_rqstp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "request-p");
    bool result = (dynamic_cast <Request*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // lgdrp: ledger object predicate

  Object* bce_lgdrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "ledger-p");
    bool result = (dynamic_cast <Ledger*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // bchnp: blockchain object predicate

  Object* bce_bchnp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "chain-p");
    bool result = (dynamic_cast <Chain*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
