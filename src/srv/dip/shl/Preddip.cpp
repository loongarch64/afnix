// ---------------------------------------------------------------------------
// - Preddip.cpp                                                             -
// - afnix:dip service - predicates implementation                           -
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
#include "Pixmap.hpp"
#include "Mixmap.hpp"
#include "Boolean.hpp"
#include "Preddip.hpp"
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

  // pixp: pixel object predicate
  
  Object* dip_pixp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "pixel-p");
    bool result = (dynamic_cast <Pixel*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // imgp: image object predicate
  
  Object* dip_imgp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "image-p");
    bool result = (dynamic_cast <Image*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // slcp: slice object predicate
  
  Object* dip_slcp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "slice-p");
    bool result = (dynamic_cast <Slice*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // pxmp: pixmap object predicate
  
  Object* dip_pxmp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "pixmap-p");
    bool result = (dynamic_cast <Pixmap*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // trnp: tranche object predicate
  
  Object* dip_trnp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "tranche-p");
    bool result = (dynamic_cast <Tranche*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // mxmp: mixmap object predicate
  
  Object* dip_mxmp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "mixmap-p");
    bool result = (dynamic_cast <Mixmap*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
