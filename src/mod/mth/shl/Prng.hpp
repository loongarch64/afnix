// ---------------------------------------------------------------------------
// - Prng.hpp                                                                -
// - afnix:mth module - pseudo-random number generator definitions           -
// ---------------------------------------------------------------------------
// - This program is free software;  you can redistribute it  and/or  modify -
// - it provided that this copyright notice is kept intact.                  -
// -                                                                         -
// - This program  is  distributed in  the hope  that it will be useful, but -
// - without  any  warranty;  without  even   the   implied    warranty   of -
// - merchantability or prngness for a particular purpose.  In no event shall -
// - the copyright holder be liable for any  direct, indirect, incidental or -
// - special damages arising in any way out of the use of this software.     -
// ---------------------------------------------------------------------------
// - copyright (c) 1999-2021 amaury darsch                                   -
// ---------------------------------------------------------------------------

#ifndef  AFNIX_PRNG_HPP
#define  AFNIX_PRNG_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif
 
namespace afnix {

  /// The Prng class is pseudo-random number generator capable of producing
  /// various random object. The main difference with the standard random
  /// functions, is that the object can be initialized with a seed and
  /// controlled directly. The underlying implementation is platform specific
  /// which corresponds to a Mersenne twister.
  /// @author amaury darsch

  class Prng : public Object {
  private:
    /// the underlying prng
    void* p_prng;
    
  public:
    /// create a default prng
    Prng (void);

    /// create a prng by seed
    /// @param seed the prng seed
    Prng (const long seed);

    /// destroy this prng
    ~Prng (void);

    /// @return the class name
    String repr (void) const;

    /// @return a random byte
    t_byte getbyte (void);
    
    /// @return a random word
    t_word getword (void);
    
    /// @return a random quad
    t_quad getquad (void);
    
    /// @return a random octa
    t_octa getocta (void);
    
    /// @return a random real
    t_real getreal (const bool iflg);

  private:
    // make the copy constructor private
    Prng (const Prng&) =delete;
    // make the assignment operator private
    Prng& operator = (const Prng&) =delete;
    
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv);
  };
}

#endif
