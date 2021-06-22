// ---------------------------------------------------------------------------
// - Md4.hpp                                                                 -
// - afnix:sec module - MD4 message digest class definition                  -
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

#ifndef  AFNIX_MD4_HPP
#define  AFNIX_MD4_HPP

#ifndef  AFNIX_HASHER_HPP
#include "Hasher.hpp"
#endif

namespace afnix {

  /// The Md4 class is a class that can be used to compute a message
  /// digest based on the MD4 algorithm. The algorithm used here is the
  /// one described by Ron Rivest in RFC 1320.
  /// @author amaury darsch

  class Md4 : public Hasher {
  private:
    // MD4 ABCD states
    t_quad d_state[4];

  public:
    /// create a default hasher
    Md4 (void);

    /// create a hasher with a result length
    /// @param rlen the hasher result length
    Md4 (const long rlen);

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
    Md4 (const Md4&);
    // make the assignment operator private
    Md4& operator = (const Md4&);
 
  public:
    /// cerate a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
