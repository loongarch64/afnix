// ---------------------------------------------------------------------------
// - Primality.hpp                                                           -
// - afnix:mth module - primality testing function definitions               -
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

#ifndef  AFNIX_PRIMALITY_HPP
#define  AFNIX_PRIMALITY_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// This file contains specific procedures that are designed to perform
  /// various primality operations. Most of these procedures operate with
  /// relatif numbers.
  /// @author amaury darsch

  /// check the fermat theorem
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_frmtp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// check the miller-rabin primality test
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_mrptp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// check if a number is a probable prime
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_ppntp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// check if a number is a safe prime
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_safep (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
