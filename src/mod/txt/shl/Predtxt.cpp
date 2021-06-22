// ---------------------------------------------------------------------------
// - Predtxt.cpp                                                             -
// - afnix:txt module - predicates implementation                            - 
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
#include "Trie.hpp"
#include "Worder.hpp"
#include "Predtxt.hpp"
#include "Scanner.hpp"
#include "Boolean.hpp"
#include "Lexicon.hpp"
#include "Literate.hpp"
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

  // patp: pattern object predicate

  Object* txt_patp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "pattern-p");
    bool result =  (dynamic_cast <Pattern*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // lexp: lexeme object predicate

  Object* txt_lexp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "lexeme-p");
    bool result =  (dynamic_cast <Lexeme*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // tlitp: (trans)literate object predicate

  Object* txt_tlitp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "literate-p");
    bool result =  (dynamic_cast <Literate*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // scanp: scanner object predicate

  Object* txt_scanp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "scanner-p");
    bool result =  (dynamic_cast <Scanner*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // triep: trie object predicate

  Object* txt_triep (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "trie-p");
    bool result =  (dynamic_cast <Trie*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // tlexp: lexicon object predicate

  Object* txt_tlexp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "lexicon-p");
    bool result =  (dynamic_cast <Lexicon*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // wordp: worder object predicate

  Object* txt_wordp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "worder-p");
    bool result =  (dynamic_cast <Worder*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
