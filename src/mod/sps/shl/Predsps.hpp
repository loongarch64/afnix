// ---------------------------------------------------------------------------
// - Predsps.hpp                                                             -
// - afnix:sps module - predicates declaration                               -
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

#ifndef  AFNIX_PREDSPS_HPP
#define  AFNIX_PREDSPS_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix:sys
  /// standard module.
  /// @author amaury darsch

  /// the bundle object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sps_bndp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the cell object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sps_celp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the record object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sps_rcdp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the sheet object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sps_shtp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the folio object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sps_folp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the index object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sps_idxp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xref object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sps_xrfp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the csv reader object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sps_csvp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the transit object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sps_tsit (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
