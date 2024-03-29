// ---------------------------------------------------------------------------
// - Md2.hpp                                                                 -
// - afnix:sec module - MD2 message digest class definition                  -
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

#ifndef  AFNIX_MD2_HPP
#define  AFNIX_MD2_HPP

#ifndef  AFNIX_HASHER_HPP
#include "Hasher.hpp"
#endif

namespace afnix {

  /// The Md2 class is a class that can be used to compute a message
  /// digest based on the MD2 algorithm. The algorithm used here is the
  /// one described by B. Kaliski in RFC 1319.
  /// @author amaury darsch

  class Md2 : public Hasher {
  private:
    // MD2 states
    t_byte d_state[16];
    // MD2 checksum
    t_byte d_cksum[16];

  public:
    /// create a default hasher
    Md2 (void);

    /// create a hasher with a result length
    /// @param rlen the hasher result length
    Md2 (const long rlen);

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
    Md2 (const Md2&);
    // make the assignment operator private
    Md2& operator = (const Md2&);

  public:
    /// cerate a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
