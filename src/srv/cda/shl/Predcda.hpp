// ---------------------------------------------------------------------------
// - Predcda.hpp                                                             -
// - afnix:cda service - predicates declaration                              -
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

#ifndef  AFNIX_PREDCDA_HPP
#define  AFNIX_PREDCDA_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix:cda service.
  /// @author amaury darsch

  /// the streamable object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* cda_stmp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the sps data streamer object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* cda_spsp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the linear data streamer object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* cda_lnrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the localizer object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* cda_locp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the streamable localizer object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* cda_sdlp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the sampler object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* cda_splp (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
