// ---------------------------------------------------------------------------
// - Predcsm.cpp                                                             -
// - afnix:csm service - predicates implementation                           -
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
#include "Visa.hpp"
#include "Domain.hpp"
#include "Notary.hpp"
#include "Tracker.hpp"
#include "Predcsm.hpp"
#include "Boolean.hpp"
#include "Delegate.hpp"
#include "Exception.hpp"
#include "LocalZone.hpp"
#include "RealmZone.hpp"
#include "UserSpace.hpp"
#include "SessionSet.hpp"

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

  // partp: part object predicate

  Object* csm_partp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "part-p");
    bool result = (dynamic_cast <Part*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // blobp: blob object predicate

  Object* csm_blobp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "blob-p");
    bool result = (dynamic_cast <Blob*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // blocp: bloc object predicate

  Object* csm_blocp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "bloc-p");
    bool result = (dynamic_cast <Bloc*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // cblbp: carrier blob object predicate

  Object* csm_cblbp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "carrier-p");
    bool result = (dynamic_cast <Carrier*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // dblbp: delegate blob object predicate

  Object* csm_dblbp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "delegate-p");
    bool result = (dynamic_cast <Delegate*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // tckrp: tracker blob object predicate

  Object* csm_tckrp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "tracker-p");
    bool result = (dynamic_cast <Tracker*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // collp: collection object predicate

  Object* csm_collp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "collection-p");
    bool result = (dynamic_cast <Collection*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // domnp: domain object predicate

  Object* csm_domnp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "domain-p");
    bool result = (dynamic_cast <Domain*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // woisp: whois object predicate

  Object* csm_woisp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "whois-p");
    bool result = (dynamic_cast <Whois*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // waisp: whatis object predicate

  Object* csm_waisp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "whatis-p");
    bool result = (dynamic_cast <Whatis*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sessp: session object predicate
  
  Object* csm_sessp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "session-p");
    bool result = (dynamic_cast <Session*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // ssetp: session set object predicate
  
  Object* csm_ssetp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "session-set-p");
    bool result = (dynamic_cast <SessionSet*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // wzonp: workzone predicate

  Object* csm_wzonp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "workzone-p");
    bool result = (dynamic_cast <WorkZone*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // wspcp: workspace predicate

  Object* csm_wspcp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "workspace-p");
    bool result = (dynamic_cast <WorkSpace*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // lzonp: localzone predicate

  Object* csm_lzonp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "localzone-p");
    bool result = (dynamic_cast <LocalZone*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // lspcp: localspace predicate

  Object* csm_lspcp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "localspace-p");
    bool result = (dynamic_cast <LocalSpace*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // uspcp: userspace predicate

  Object* csm_uspcp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "userspace-p");
    bool result = (dynamic_cast <UserSpace*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // relmp: realm predicate

  Object* csm_relmp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "realm-p");
    bool result = (dynamic_cast <Realm*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // rzonp: realm zone predicate

  Object* csm_rzonp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "realzonem-p");
    bool result = (dynamic_cast <RealmZone*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // idtyp: identity predicate

  Object* csm_idtyp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "identity-p");
    bool result = (dynamic_cast <Identity*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // authp: authority predicate

  Object* csm_authp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "authority-p");
    bool result = (dynamic_cast <Authority*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // pcplp: principal predicate

  Object* csm_pcplp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "principal-p");
    bool result = (dynamic_cast <Principal*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // xactp: act predicate

  Object* csm_xactp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "act-p");
    bool result = (dynamic_cast <Act*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // visap: visa predicate

  Object* csm_visap  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "visa-p");
    bool result = (dynamic_cast <Visa*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // credp: credential predicate

  Object* csm_credp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "credential-p");
    bool result = (dynamic_cast <Credential*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // ntryp: notary predicate

  Object* csm_ntryp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "notary-p");
    bool result = (dynamic_cast <Notary*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
