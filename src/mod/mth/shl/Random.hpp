// ---------------------------------------------------------------------------
// - Random.hpp                                                              -
// - afnix:mth module - random number generator definitions                  -
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

#ifndef  AFNIX_RANDOM_HPP
#define  AFNIX_RANDOM_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// This file contains specific procedures that produce random numbers
  /// in different formats and objects.
  /// @author amaury darsch

  /// get a random byte
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_byternd (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// get a random integer value
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_longrnd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a random real value
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_realrnd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a large random relatif value
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_relnrnd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a random prime number
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_primrnd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a random safe prime number
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_safernd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a random bitset
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_bitsrnd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a random byte buffer
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_bbufrnd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a random integer vector
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_ivecrnd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a random numeral vector
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_nvecrnd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a random real vector
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_rvecrnd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a random numeral block
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_nblkrnd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a random real block
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_rblkrnd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a random sparse matrix
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_rmtxrnd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a block matrix
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_rblksps (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a sparse matrix
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_rmtxsps (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a uniform deviate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_unidev (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a normal deviate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* mth_nrmdev (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
