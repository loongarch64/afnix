// ---------------------------------------------------------------------------
// - Sha224.hpp                                                              -
// - afnix:sec module - Secure Hash Algorithm 224 class definition           -
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

#ifndef  AFNIX_SHA224_HPP
#define  AFNIX_SHA224_HPP

#ifndef  AFNIX_HASHER_HPP
#include "Hasher.hpp"
#endif

namespace afnix {

  /// The Sha224 class is a class that can be used to compute a message
  /// hash based on the Secure Hash Algorithm (SHA-2-224) algorithm. 
  /// The algorithm used here is the one described by the Federal 
  /// Standard FIPS PUB 180-4. This is an original implementation
  /// similar to the description given in FIPS PUB 180-4.
  /// @author amaury darsch

  class Sha224 : public Hasher {
  private:
    // SHA-224 states
    t_quad d_state[8];

  public:
    /// create a default hasher
    Sha224 (void);

    /// create a hasher with a result length
    /// @param rlen the hasher result length
    Sha224 (const long rlen);

    /// @return the class name
    String repr (void) const;

    /// reset this hasher
    void reset (void);

    /// update the hasher state with the buffer data
    /// @return true if the buffer was processed
    bool update (void);

    /// finish processing by padding the data
    void finish (void);

  private:
    // make the copy constructor private
    Sha224 (const Sha224&);
    // make the assignment operator private
    Sha224& operator = (const Sha224&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
