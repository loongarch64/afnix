// ---------------------------------------------------------------------------
// - PhyCalls.hpp                                                            -
// - afnix:phy service - physics specific calls definitions                  -
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

#ifndef  AFNIX_PHYCALLS_HPP
#define  AFNIX_PHYCALLS_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// This file contains specific system procedures that require only
  /// arguments and that are not bounded to a class
  /// @author amaury darsch

  /// create a unit by name
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* phy_tounit (Evaluable* zobj, Nameset* nset, Cons* args);

  /// create a physical constant by name
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* phy_tophys (Evaluable* zobj, Nameset* nset, Cons* args);
}


#endif
