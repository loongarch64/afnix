// ---------------------------------------------------------------------------
// - MthCalls.hpp                                                            -
// - afnix:mth module - math specific calls definitions                      -
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

#ifndef  AFNIX_MTHCALLS_HPP
#define  AFNIX_MTHCALLS_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains specific system procedures that require only
  /// arguments and that are not bounded to a class
  /// @author amaury darsch

  /// convert a string to a real vector
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_torvi (Evaluable* zobj, Nameset* nset, Cons* args);

  /// convert an object to an infix string
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_toinfix (Evaluable* zobj, Nameset* nset, Cons* args);
}


#endif
