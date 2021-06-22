// ---------------------------------------------------------------------------
// - Predsvg.hpp                                                             -
// - afnix:svg service - predicates declaration                              -
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

#ifndef  AFNIX_PREDSVG_HPP
#define  AFNIX_PREDSVG_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix
  /// svg (scalable vector graphics) service.
  /// @author amaury darsch

  /// the plot2d object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* svg_pltdp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the svg transform object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* svg_trfrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the svg root object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* svg_rootp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the svg fragment object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* svg_fragp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the svg styling object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* svg_stlgp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the svg rectangle object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* svg_rectp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the svg circle object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* svg_circp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the svg ellipse object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* svg_elpsp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the svg line object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* svg_linep (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the svg polyline object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* svg_plinp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the svg polygon object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* svg_pgonp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the svg group object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* svg_grpp (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
