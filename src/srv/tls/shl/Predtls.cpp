// ---------------------------------------------------------------------------
// - Predtls.cpp                                                             -
// - afnix:tls service - predicates implementation                           -
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

#include "Pem.hpp"
#include "Cons.hpp"
#include "X509.hpp"
#include "Pkcs1.hpp"
#include "Boolean.hpp"
#include "Predtls.hpp"
#include "TlsShake.hpp"
#include "TlsCerts.hpp"
#include "TlsSuite.hpp"
#include "TlsRecord.hpp"
#include "TlsServer.hpp"
#include "TlsClient.hpp"
#include "TlsShello.hpp"
#include "TlsChello.hpp"
#include "Exception.hpp"
#include "TlsConnect.hpp"

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

  // -------------------------------------------------------------------------
  // - tls section                                                           -
  // -------------------------------------------------------------------------

  // blkp: tls block object predicate
  
  Object* tls_blkp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "block-p");
    bool result = (dynamic_cast <TlsBlock*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // hdrp: tls header object predicate
  
  Object* tls_hdrp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "header-p");
    bool result = (dynamic_cast <TlsHeader*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // recp: tls record object predicate
  
  Object* tls_recp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "record-p");
    bool result = (dynamic_cast <TlsRecord*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // msgp: tls message object predicate
  
  Object* tls_msgp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "message-p");
    bool result = (dynamic_cast <TlsMessage*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // hskp: tls handshake object predicate
  
  Object* tls_shkp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "handshake-p");
    bool result = (dynamic_cast <TlsShake*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // stap: tls state object predicate
  
  Object* tls_stap  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "state-p");
    bool result = (dynamic_cast <TlsState*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // prmp: tls parameter object predicate
  
  Object* tls_prmp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "params-p");
    bool result = (dynamic_cast <TlsParams*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // conp: tls connect object predicate
  
  Object* tls_conp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "connect-p");
    bool result = (dynamic_cast <TlsConnect*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sckp: tls socket object predicate
  
  Object* tls_sokp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "socket-p");
    bool result = (dynamic_cast <TlsSocket*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // srvp: tls server object predicate
  
  Object* tls_srvp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "server-p");
    bool result = (dynamic_cast <TlsServer*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // cltp: tls client object predicate
  
  Object* tls_cltp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "client-p");
    bool result = (dynamic_cast <TlsClient*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // shlop: tls server hello object predicate
  
  Object* tls_shlop  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "server-hello-p");
    bool result = (dynamic_cast <TlsShello*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // chlop: tls client hello object predicate
  
  Object* tls_chlop  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "client-hello-p");
    bool result = (dynamic_cast <TlsChello*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // suitep: tls suite object predicate
  
  Object* tls_suitep  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "suite-p");
    bool result = (dynamic_cast <TlsSuite*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // certsp: tls certificate list object predicate
  
  Object* tls_certsp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "certificate-list-p");
    bool result = (dynamic_cast <TlsCerts*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // -------------------------------------------------------------------------
  // - itu section                                                           -
  // -------------------------------------------------------------------------


  // xalgop: X509 algorithm object predicate

  Object* tls_xalgop  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xalgo-p");
    bool result = (dynamic_cast <Xalgo*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // pki: pki object predicate

  Object* tls_pkcsp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "pki-p");
    bool result = (dynamic_cast <Pki*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // pkcs1: pkcs1 object predicate

  Object* tls_pkcs1p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "pkcs-1-p");
    bool result = (dynamic_cast <Pkcs1*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // x509p: X509 ceritifcate object predicate

  Object* tls_x509p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "x509-p");
    bool result = (dynamic_cast <X509*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // pemp: pem object predicate

  Object* tls_pemp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "pem-p");
    bool result = (dynamic_cast <Pem*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
