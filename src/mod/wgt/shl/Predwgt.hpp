// ---------------------------------------------------------------------------
// - Predwgt.hpp                                                             -
// - afnix:wgt module - predicates declaration                               -
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

#ifndef  AFNIX_PREDWGT_HPP
#define  AFNIX_PREDWGT_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix:wgt
  /// standard module.
  /// @author amaury darsch

  /// the conditional object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wgt_condp (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
