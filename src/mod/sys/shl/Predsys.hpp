// ---------------------------------------------------------------------------
// - Predsys.hpp                                                             -
// - afnix:sys module - predicates declaration                               -
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

#ifndef  AFNIX_PREDSYS_HPP
#define  AFNIX_PREDSYS_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix:sys
  /// standard module.
  /// @author amaury darsch

  /// the time object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sys_timep (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the date object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sys_datep (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the option object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sys_optsp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the performance meter object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sys_pmtrp (Evaluable* zobj, Nameset* nset, Cons* args);
}


#endif
