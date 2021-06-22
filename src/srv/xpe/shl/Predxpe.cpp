// ---------------------------------------------------------------------------
// - Predxpe.cpp                                                             -
// - afnix:xpe service - predicates implementation                           -
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
#include "Predxpe.hpp"
#include "Boolean.hpp"
#include "XmlPlist.hpp"
#include "Exception.hpp"
#include "XmlInclude.hpp"
#include "XmlProcessor.hpp"
#include "XmlPrintTable.hpp"

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

  // procp: xml processor object predicate

  Object* xpe_procp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xml-processor-p");
    bool result = (dynamic_cast <XmlProcessor*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // xcntp: xml content object predicate

  Object* xpe_xcntp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xml-content-p");
    bool result = (dynamic_cast <XmlContent*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // xftrp: xml feature object predicate

  Object* xpe_xftrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xml-feature-p");
    bool result = (dynamic_cast <XmlFeature*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // xincp: xml include object predicate

  Object* xpe_xincp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xml-include-p");
    bool result = (dynamic_cast <XmlInclude*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // xplstp: xml plist object predicate

  Object* xpe_xplstp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xml-plist-p");
    bool result = (dynamic_cast <XmlPlist*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // xptblp: xml print table object predicate

  Object* xpe_xptblp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xml-print-table-p");
    bool result = (dynamic_cast <XmlPrintTable*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
