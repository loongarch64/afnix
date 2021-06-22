// ---------------------------------------------------------------------------
// - For.cpp                                                                 -
// - afnix engine - for builtin function implementation                      -
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
#include "Symbol.hpp"
#include "Builtin.hpp"
#include "Lexical.hpp"
#include "Iterator.hpp"
#include "Localset.hpp"
#include "Exception.hpp"

namespace afnix {

  // this function check that the object is a lexical name
  static inline Lexical* get_lex (Object* object) {
    Lexical* lex = dynamic_cast <Lexical*> (object);
    if (lex == nullptr) 
      throw Exception ("type-error", "invalid object in for symbol list",
		       Object::repr (object));
    return lex;
  }

  // this function check that a form is made only with lexical name and bind
  // the symbol in the specified nameset. The function returns list of 
  /// symbols which are later set during the iteration

  static Cons* get_itsym (Cons* itlist, Nameset* nset) {
    Cons* result = nullptr;
    while (itlist != nullptr) {
      // get lexical and create a symbol
      Lexical* lex = get_lex (itlist->getcar ());
      long quark   = lex->toquark ();
      Symbol* sym = new Symbol (quark);
      // bind the symbol and add it in the result list
      nset->bind (quark, sym);
      if (result == nullptr)
	result = new Cons (sym);
      else
	result->add (sym);
      // next in list
      itlist = itlist->getcdr ();
    }
    return result;
  }

  // this function check that the object is an iterable object and return the
  // associated iterator
  static inline Iterator* get_ito (Object* object) {
    if (object == nullptr) return nullptr;
    Iterable* ito = dynamic_cast <Iterable*> (object);
    if (ito == nullptr) 
      throw Exception ("type-error", "non iterable object found with for list",
		       Object::repr (object));
    return ito->makeit ();
  }

  // this function evaluates the list of iterable object and returns a list
  // of iterators
  static Cons* get_itobj (Evaluable* zobj, Cons* itlist, Nameset* lset) {
    Cons* result = nullptr;
    while (itlist != nullptr) {
      // get the iterable object
      Object*   obj = itlist->getcar ();
      Object*   eio = (obj == nullptr) ? nullptr : obj->eval (zobj, lset);
      Iterator* ito = get_ito (eio);
      if (result == nullptr)
	result = new Cons (ito);
      else
	result->add (ito);
      // next object
      itlist = itlist->getcdr ();
    }
    return result;
  }

  // this function return true if one of the iterator is at the end
  static bool get_itend (Cons* itlist) {
    while (itlist != nullptr) {
      Iterator* ito = dynamic_cast <Iterator*> (itlist->getcar ());
      if (ito == nullptr) return true;
      if (ito->isend () == true) return true;
      itlist = itlist->getcdr ();
    }
    return false;
  }

  // this procedure binds the symbols with the current iterator values
  static void get_itvalue (Cons* itsym, Cons* itobj) {
    while (itsym != nullptr) {
      // set the symbol and iterator
      Symbol*   sym = dynamic_cast <Symbol*>   (itsym->getcar ());
      Iterator* ito = dynamic_cast <Iterator*> (itobj->getcar ());
      sym->setobj (ito == nullptr ? nullptr : ito->getobj ());
      // go to next position
      itsym = itsym->getcdr ();
      itobj = itobj->getcdr ();
    }
  }

  // this procedure moves the iterators to their next position
  static void get_itnext (Cons* itobj) {
    while (itobj != nullptr) {
      Iterator* ito = dynamic_cast <Iterator*> (itobj->getcar ());
      if (ito != nullptr) ito->next ();
      itobj = itobj->getcdr ();
    }
  }

  // this function implements the behavior of the for builtin function

  Object* Builtin::sffor (Evaluable* zobj, Nameset* nset, Cons* args) {
    // check that we have at least 3 arguments
    if ((args == nullptr) || (args->length () != 3))
      throw Exception ("argument-error", "invalid argument with for");

    // get the lexical name list
    Cons* lexlst = dynamic_cast <Cons*> (args->getcar ());
    if (lexlst == nullptr) 
      throw Exception ("type-error", "lexical list expected with for");
    // get the iterable object list
    Cons* objlst = dynamic_cast <Cons*> (args->getcadr ());
    if (objlst == nullptr) 
      throw Exception ("type-error", "lexical list expected with for");
    // check that both list have the same size
    if (lexlst->length () != objlst->length ())
      throw Exception ("argument-error", "for argument list size mismatch");
    // get the form to execute
    Object* form = args->getcaddr ();

    // evaluate the iterable object and reset the iterators
    Cons* itobj = get_itobj (zobj, objlst, nset);
    // create a new local set for the symbol binding
    Localset* lset = new Localset (nset);
    Object::iref (lset);

    // evaluate the symbol list binding
    Cons* itsym = nullptr;
    try {
      itsym = get_itsym (lexlst, lset);
    } catch (...) {
      delete itobj;
      Object::dref (lset);
      throw;
    }

    // now we are ready to iterate
    Object* result = nullptr;
    try {
      while (get_itend (itobj) == false) {
	get_itvalue (itsym, itobj);
	get_itnext  (itobj);
	Object::cref (result);
	result = form->eval (zobj,lset);
      }
    } catch (...) {
      delete itsym;
      delete itobj;
      Object::dref (lset);
      throw;
    }

    // we are done - clean everything and return
    delete itsym;
    delete itobj;
    Object::dref (lset);
    return result;
  }
}
