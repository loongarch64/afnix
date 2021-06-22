// ---------------------------------------------------------------------------
// - Mpia.hpp                                                                -
// - afnix:mth module - multi-precision integer arithmetic definitions       -
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

#ifndef  AFNIX_MPIA_HPP
#define  AFNIX_MPIA_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// This file contains specific procedures that perfom various operations
  /// with multi-precision integers
  /// @author amaury darsch

  /// compute the greatest common divisor of two relatifs
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_gcd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// compute the least common multiple of two relatifs
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_lcm (Evaluable* zobj, Nameset* nset, Cons* args);

  /// compute the multiplicative modular inverse of a relatif
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_mmi (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
