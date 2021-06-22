// ---------------------------------------------------------------------------
// - Sha1.hpp                                                                -
// - afnix:sec module - Secure Hash Algorithm 1 class definition             -
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

#ifndef  AFNIX_SHA1_HPP
#define  AFNIX_SHA1_HPP

#ifndef  AFNIX_HASHER_HPP
#include "Hasher.hpp"
#endif

namespace afnix {

  /// The Sha1 class is a class that can be used to compute a message
  /// hash based on the Secure Hash Algorithm (SHA-1) algorithm. 
  /// The algorithm used here is the one described by the Federal 
  /// Standard FIPS PUB 180-2. This is an original implementation
  /// similar to the description given in FIPS PUB 180-2.
  /// @author amaury darsch

  class Sha1 : public Hasher {
  private:
    // SHA-1 states
    t_quad d_state[5];

  public:
    /// create a default hasher
    Sha1 (void);

    /// create a hasher with a result length
    /// @param rlen the hasher result length
    Sha1 (const long rlen);

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
    Sha1 (const Sha1&);
    // make the assignment operator private
    Sha1& operator = (const Sha1&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
