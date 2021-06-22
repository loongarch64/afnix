// ---------------------------------------------------------------------------
// - cgen.hpp                                                                -
// - standard platform library - C generator functions definition            -
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

#ifndef  AFNIX_CGEN_HPP
#define  AFNIX_CGEN_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file contains the various procedures that produces random typed
  /// values from a pseudo-random number generator. The PRNG is based on a
  /// 64 bit mersenne-twister architecture that needs to be seeded prior
  /// its use. Such initialization can be disabled for debugging purpose.
  /// The reference implementation is MT19937-64.
  /// @author amaury darsch

  /// initialize the random generator
  void c_initrnd (void);

  /// @return a random byte
  t_byte c_byternd (void);

  /// @return a random word
  t_word c_wordrnd (void);

  /// @return a random quad
  t_quad c_quadrnd (void);

  /// @return a random octa
  t_octa c_octarnd (void);

  /// generate a real random number between 0.0 and 1.0
  /// @param iflg the inclusion flag
  t_real c_realrnd (const bool iflg);

  /// create a new random engine
  void* c_newprng (void);

  /// create a new random engine by seed
  /// @param seed the prng seed
  void* c_newprng (const long seed);

  /// delete the randonm engine
  /// @param pp the prng pointer
  void c_delprng (void* pp);

  /// get a random byte
  /// @param pp the prng pointer
  t_byte c_byternd (void* pp);

  /// get a random word
  /// @param pp the prng pointer
  t_word c_wordrnd (void* pp);

  /// get a random quad
  /// @param pp the prng pointer
  t_quad c_quadrnd (void* pp);

  /// get a random octa
  /// @param pp the prng pointer
  t_octa c_octarnd (void* pp);

  /// generate a real random number between 0.0 and 1.0
  /// @param iflg the inclusion flag
  /// @param pp the prng pointer
  t_real c_realrnd (void* pp, const bool iflg);
}

#endif
