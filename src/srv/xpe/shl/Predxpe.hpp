// ---------------------------------------------------------------------------
// - Predxpe.hpp                                                             -
// - afnix:xpe service - predicates declaration                              -
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

#ifndef  AFNIX_PREDXPE_HPP
#define  AFNIX_PREDXPE_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix
  /// xml processing environment (xpe) service.
  /// @author amaury darsch

  /// the xml processor object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xpe_procp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml content object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xpe_xcntp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml feature object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xpe_xftrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml include object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xpe_xincp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml plist object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xpe_xplstp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml print table object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xpe_xptblp (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
