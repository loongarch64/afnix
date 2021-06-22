// ---------------------------------------------------------------------------
// - Predcda.cpp                                                             -
// - afnix:cda module - predicates implementation                            -
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
#include "Spsds.hpp"
#include "Lnrds.hpp"
#include "Stmdl.hpp"
#include "Sampler.hpp"
#include "Predcda.hpp"
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

  // stmp: streamable object predicate

  Object* cda_stmp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "streamable-p");
    bool result =  (dynamic_cast<Streamable*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // spsp: sps data streamer object predicate

  Object* cda_spsp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "sps-streamer-p");
    bool result =  (dynamic_cast<Spsds*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // lnrp: linear data streamer object predicate

  Object* cda_lnrp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "linear-streamer-p");
    bool result =  (dynamic_cast<Lnrds*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // locp: localizer object predicate

  Object* cda_locp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "localizer-p");
    bool result =  (dynamic_cast<Localizer*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sdlp: streamable localizer object predicate

  Object* cda_sdlp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "streamable-localizer-p");
    bool result =  (dynamic_cast<Stmdl*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // splp: sampler object predicate

  Object* cda_splp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "sampler-p");
    bool result =  (dynamic_cast<Sampler*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
