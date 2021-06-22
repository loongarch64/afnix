// ---------------------------------------------------------------------------
// - Prednwg.cpp                                                             -
// - afnix:nwg module - predicates implementation                            -
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
#include "Mime.hpp"
#include "Uuid.hpp"
#include "Json.hpp"
#include "Basexx.hpp"
#include "Prednwg.hpp"
#include "Boolean.hpp"
#include "UriPath.hpp"
#include "UriQuery.hpp"
#include "Hyperlink.hpp"
#include "Exception.hpp"
#include "HttpStream.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

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

  // urip: uri object predicate

  Object* nwg_urip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "uri-p");
    bool result = (dynamic_cast <Uri*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // uripp: uri path object predicate

  Object* nwg_uripp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "uri-path-p");
    bool result = (dynamic_cast <UriPath*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // uriqp: uri query object predicate

  Object* nwg_uriqp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "uri-query-p");
    bool result = (dynamic_cast <UriQuery*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // mimep: mime document object predicate

  Object* nwg_mimep  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "mime-p");
    bool result = (dynamic_cast <Mime*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // hlnkp: hyperlink object predicate

  Object* nwg_hlnkp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "hyperlink-p");
    bool result = (dynamic_cast <Hyperlink*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // uuidp: uuid object predicate

  Object* nwg_uuidp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "uuid-p");
    bool result = (dynamic_cast <Uuid*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // protop: http proto object predicate

  Object* nwg_protop (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "http-proto-p");
    bool result = (dynamic_cast <HttpProto*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // hrqstp: http request object predicate

  Object* nwg_hrqstp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "http-request-p");
    bool result = (dynamic_cast <HttpRequest*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // hrespp: http response object predicate

  Object* nwg_hrespp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "http-response-p");
    bool result = (dynamic_cast <HttpResponse*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // hstrmp: http stream object predicate

  Object* nwg_hstrmp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "http-stream-p");
    bool result = (dynamic_cast <HttpStream*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // cookp: http cookie object predicate

  Object* nwg_cookp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "cookie-p");
    bool result = (dynamic_cast <Cookie*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // cjarp: cookie jar object predicate

  Object* nwg_cjarp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "cookie-jar-p");
    bool result = (dynamic_cast <CookieJar*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // bsxxp: basexx codec object predicate

  Object* nwg_bsxxp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "basexx-p");
    bool result = (dynamic_cast <Basexx*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // jsonp: json object predicate

  Object* nwg_jsonp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "json-p");
    bool result = (dynamic_cast <Json*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
