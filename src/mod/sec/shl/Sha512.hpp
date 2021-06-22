// ---------------------------------------------------------------------------
// - Sha512.hpp                                                              -
// - afnix:sec module - Secure Hash Algorithm 512 class definition           -
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

#ifndef  AFNIX_SHA512_HPP
#define  AFNIX_SHA512_HPP

#ifndef  AFNIX_HASHER_HPP
#include "Hasher.hpp"
#endif

namespace afnix {

  /// The Sha512 class is a class that can be used to compute a message
  /// hash based on the Secure Hash Algorithm (SHA-2-512) algorithm. 
  /// The algorithm used here is the one described by the Federal 
  /// Standard FIPS PUB 180-4. This is an original implementation
  /// similar to the description given in FIPS PUB 180-4. The special
  /// versions SHA-2-512/224 and SHA-2-512/256 are available through the
  /// use of the standard flag with the appropriate length.
  /// @author amaury darsch

  class Sha512 : public Hasher {
  private:
    // SHA-512 states
    t_octa d_state[8];
    // the standard flag
    bool d_sflg;

  public:
    /// create a default hasher
    Sha512 (void);

    /// create a hasher with a result length
    /// @param rlen the hasher result length
    Sha512 (const long rlen);

    /// create a hasher with a result length
    /// @param rlen the hasher result length
    /// @param sflg the standard flag
    Sha512 (const long rlen, const bool sflg);

    /// @return the class name
    String repr (void) const;

    /// reset this digest
    void reset (void);

    /// update the hasher state with the buffer data
    /// @return true if the buffer was processed
    bool update (void);

    /// finish processing by padding the data
    void finish (void);

  private:
    // make the copy constructor private
    Sha512 (const Sha512&);
    // make the assignment operator private
    Sha512& operator = (const Sha512&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
