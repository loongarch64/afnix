// ---------------------------------------------------------------------------
// - Preditu.hpp                                                             -
// - afnix:itu module - predicates declaration                               -
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

#ifndef  AFNIX_PREDITU_HPP
#define  AFNIX_PREDITU_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix:itu
  /// standard module.
  /// @author amaury darsch

  // -------------------------------------------------------------------------
  // - asn section                                                           -
  // -------------------------------------------------------------------------

  /// the asn node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnnodep (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn buffer node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnbufp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn eoc object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asneocp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn gtm object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asngtmp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn utc object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnutcp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn ias object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asniasp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn sequence object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnseqp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn set object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnsetp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn oid object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnoidp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn relative oid object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnroidp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn boolean object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnboolp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn integer object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnintgp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn bit string object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnbitsp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn octet string object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnoctsp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn null object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnnullp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn nums object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnnumsp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn prts object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnprtsp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn bmps object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnbmpsp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn unvs object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnunvsp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the asn utfs object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_asnutfsp (Evaluable* zobj, Nameset* nset, Cons* args);

  // -------------------------------------------------------------------------
  // - itu section                                                           -
  // -------------------------------------------------------------------------

  /// the oid object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* itu_oidp (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
