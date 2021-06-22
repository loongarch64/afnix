// ---------------------------------------------------------------------------
// - Predtxt.hpp                                                             -
// - afnix:txt module - predicates declaration                               -
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

#ifndef  AFNIX_PREDTXT_HPP
#define  AFNIX_PREDTXT_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {
  
  /// this file contains the predicates associated with the afnix:txt
  /// standard module.
  /// @author amaury darsch

  /// the pattern object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* txt_patp    (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the lexeme object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* txt_lexp    (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the literate object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* txt_tlitp   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the scanner object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* txt_scanp   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the trie object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* txt_triep   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the lexicon object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* txt_tlexp   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the worder object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* txt_wordp   (Evaluable* zobj, Nameset* nset, Cons* args);
}


#endif
