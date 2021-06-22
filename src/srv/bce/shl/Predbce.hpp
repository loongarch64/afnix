// ---------------------------------------------------------------------------
// - Predbce.hpp                                                             -
// - afnix:bce service - predicates declaration                              -
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

#ifndef  AFNIX_PREDBCE_HPP
#define  AFNIX_PREDBCE_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix:bce
  /// standard service.
  /// @author amaury darsch

  /// the transaction object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* bce_tranp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the block object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* bce_blokp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the request object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* bce_rqstp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the ledger object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* bce_lgdrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the blockchain object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* bce_bchnp (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
