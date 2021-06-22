// ---------------------------------------------------------------------------
// - Preditu.cpp                                                             -
// - afnix:itu module - predicates implementation                            -
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
#include "AsnEoc.hpp"
#include "AsnGtm.hpp"
#include "AsnUtc.hpp"
#include "AsnIas.hpp"
#include "AsnOid.hpp"
#include "AsnSet.hpp"
#include "AsnNull.hpp"
#include "AsnRoid.hpp"
#include "AsnBits.hpp"
#include "AsnNums.hpp"
#include "AsnPrts.hpp"
#include "AsnBmps.hpp"
#include "AsnUnvs.hpp"
#include "AsnUtfs.hpp"
#include "Preditu.hpp"
#include "Boolean.hpp"
#include "AsnBuffer.hpp"
#include "AsnOctets.hpp"
#include "Exception.hpp"
#include "AsnBoolean.hpp"
#include "AsnInteger.hpp"
#include "AsnSequence.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

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
  // - asn section                                                           -
  // -------------------------------------------------------------------------

  // asnnodep: asn node object predicate

  Object* itu_asnnodep  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-node-p");
    bool result = (dynamic_cast <AsnNode*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asnbufp: asn buffer node object predicate

  Object* itu_asnbufp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-buffer-p");
    bool result = (dynamic_cast <AsnBuffer*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asneocp: asn eoc object predicate

  Object* itu_asneocp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-eoc-p");
    bool result = (dynamic_cast <AsnEoc*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asngtmp: asn generalized time object predicate

  Object* itu_asngtmp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-gtm-p");
    bool result = (dynamic_cast <AsnGtm*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asngtmp: asn utc time object predicate

  Object* itu_asnutcp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-utc-p");
    bool result = (dynamic_cast <AsnUtc*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asniasp: IA5 asn string object predicate

  Object* itu_asniasp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-ias-p");
    bool result = (dynamic_cast <AsnIas*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asnseqp: asn sequence object predicate

  Object* itu_asnseqp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-sequence-p");
    bool result = (dynamic_cast <AsnSequence*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asnsetp: asn set object predicate

  Object* itu_asnsetp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-set-p");
    bool result = (dynamic_cast <AsnSet*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // asnoidp: asn oid object predicate

  Object* itu_asnoidp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-oid-p");
    bool result = (dynamic_cast <AsnOid*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asnroidp: asn relative oid object predicate

  Object* itu_asnroidp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-roid-p");
    bool result = (dynamic_cast <AsnRoid*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asnboolp: asn boolean object predicate

  Object* itu_asnboolp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-boolean-p");
    bool result = (dynamic_cast <AsnBoolean*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asnintgp: asn integer object predicate

  Object* itu_asnintgp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-integer-p");
    bool result = (dynamic_cast <AsnInteger*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asnbitsp: asn bit string object predicate

  Object* itu_asnbitsp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-bits-p");
    bool result = (dynamic_cast <AsnBits*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asnoctsp: asn octet string object predicate

  Object* itu_asnoctsp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-octets-p");
    bool result = (dynamic_cast <AsnOctets*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asnnullp: asn null object predicate

  Object* itu_asnnullp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-null-p");
    bool result = (dynamic_cast <AsnNull*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asnnumsp: asn numeric string object predicate

  Object* itu_asnnumsp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-nums-p");
    bool result = (dynamic_cast <AsnNums*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asnprtsp: asn printable string object predicate

  Object* itu_asnprtsp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-prts-p");
    bool result = (dynamic_cast <AsnPrts*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asnbmpsp: asn bmp string object predicate

  Object* itu_asnbmpsp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-bmps-p");
    bool result = (dynamic_cast <AsnBmps*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asnunvsp: asn unversal string object predicate

  Object* itu_asnunvsp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-unvs-p");
    bool result = (dynamic_cast <AsnUnvs*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // asnutfsp: asn utf string object predicate

  Object* itu_asnutfsp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "asn-utfs-p");
    bool result = (dynamic_cast <AsnUtfs*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // -------------------------------------------------------------------------
  // - itu section                                                           -
  // -------------------------------------------------------------------------

  // oidp: oid object predicate

  Object* itu_oidp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "oid-p");
    bool result = (dynamic_cast <Oid*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
