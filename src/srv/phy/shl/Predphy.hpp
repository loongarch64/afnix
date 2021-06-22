// ---------------------------------------------------------------------------
// - Predphy.hpp                                                             -
// - afnix:phy service - predicates declaration                              -
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

#ifndef  AFNIX_PREDPHY_HPP
#define  AFNIX_PREDPHY_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix
  /// physic service.
  /// @author amaury darsch

  /// the unit object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* phy_unitp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the physics object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* phy_physp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the atom object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* phy_atomp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the periodic object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* phy_ptblp (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
