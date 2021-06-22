// ---------------------------------------------------------------------------
// - Preddip.hpp                                                             -
// - afnix:dip service - predicates declaration                              -
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

#ifndef  AFNIX_PREDDIP_HPP
#define  AFNIX_PREDDIP_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// This file contains the predicates associated with the afnix image
  /// processing service.
  /// @author amaury darsch

  /// the point 1 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_pnt1p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the point 2 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_pnt2p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the point 3 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_pnt3p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the point 4 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_pnt4p (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the vector 1 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_vec1p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the vector 2 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_vec2p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the vector 3 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_vec3p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the vector 4 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_vec4p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the quaternion object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_qtrnp (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the solid object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_sldp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the cuboid object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_cubp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the plane object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_plnp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the pixel object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_pixp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the image object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_imgp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the slice object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_slcp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the pixmap object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_pxmp (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the tranche object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_trnp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the mixmap object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* dip_mxmp (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
