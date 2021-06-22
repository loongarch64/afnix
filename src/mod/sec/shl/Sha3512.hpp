// ---------------------------------------------------------------------------
// - Sha3512.hpp                                                             -
// - afnix:sec module - Secure Hash Algorithm 3-512 class definition         -
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

#ifndef  AFNIX_SHA3512_HPP
#define  AFNIX_SHA3512_HPP

#ifndef  AFNIX_HASHER_HPP
#include "Hasher.hpp"
#endif

namespace afnix {

  /// The Sha3512 class is a class that can be used to compute a message
  /// hash based on the Secure Hash Algorithm (SHA-3-512) algorithm. 
  /// The algorithm used here is the one described by the Federal 
  /// Standard FIPS PUB 202. This is an original implementation but largely
  /// inspired by the standard publication and rhash implementation.
  /// @author amaury darsch

  class Sha3512 : public Hasher {
  private:
    // SHA-3-512 hash states
    t_octa d_hsts[25];

  public:
    /// create a default hasher
    Sha3512 (void);

    /// create a hasher with a result length
    /// @param rlen the hasher result length
    Sha3512 (const long rlen);

    /// @return the class name
    String repr (void) const;

    /// reset this digest
    void reset (void);

  protected:
    /// update the hasher state with the buffer data
    /// @return true if the buffer was processed
    bool update (void);

    /// finish processing by padding the data
    void finish (void);

  private:
    // make the copy constructor private
    Sha3512 (const Sha3512&) =delete;
    // make the assignment operator private
    Sha3512& operator = (const Sha3512&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
