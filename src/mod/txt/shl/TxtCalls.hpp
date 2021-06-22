// ---------------------------------------------------------------------------
// - TxtCalls.hpp                                                            -
// - afnix:txt module - txt specific calls definitions                       -
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

#ifndef  AFNIX_TXTCALLS_HPP
#define  AFNIX_TXTCALLS_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains specific system procedures that require only
  /// arguments and that are not bounded to a class
  /// @author amaury darsch

  /// indent a string with start and end controls
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* txt_indent (Evaluable* zobj, Nameset* nset, Cons* args);

  /// sort an array in ascending order
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* txt_qslth (Evaluable* zobj, Nameset* nset, Cons* args);

  /// sort an array in descending order
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* txt_qsgth (Evaluable* zobj, Nameset* nset, Cons* args);

  /// sort an array in lexicographic order
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* txt_qslex (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
