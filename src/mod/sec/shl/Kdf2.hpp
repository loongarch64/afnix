// ---------------------------------------------------------------------------
// - Kdf2.hpp                                                                -
// - afnix:sec module - key derivation function (KDF2) class definition      -
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

#ifndef  AFNIX_KDF2_HPP
#define  AFNIX_KDF2_HPP

#ifndef  AFNIX_HKDF_HPP
#include "Hkdf.hpp"
#endif

namespace afnix {

  /// The Kdf2 class is a hashed key derivation function class that
  /// implements the KDF2 specification as defined by ISO/IEC 18033-2.
  /// The class is strictly equivalent to the key function derivation
  /// (KDF1) except that the internal counter runs from 1 to k instead of
  /// 0 to k-1. The class operates in theory with any type of hashable object
  /// as long as the octet string is not too long.
  /// @author amaury darsch

  class Kdf2 : public Hkdf {
  protected:
    /// the number of iteration
    long d_inum;
    
  public:
    /// create a hashed kdf2 object by hashable and size
    /// @param hobj the hashable object
    /// @param kbsz the key buffer size
    Kdf2 (Hashable* hobj, const long kbsz);

    /// create a hashed kdf2 object by hashable, size and iteration number
    /// @param hobj the hashable object
    /// @param kbsz the key buffer size
    /// @param inum the iteration number
    Kdf2 (Hashable* hobj, const long kbsz, const long inum);

    /// @return the class name
    String repr (void) const;

    /// derive a key from an octet string by size
    /// @param ostr the octet string to process
    /// @param size the octet string size
    void derive (const t_byte* ostr, const long size);
 
  private:
    // make the copy constructor private
    Kdf2 (const Kdf2&);
    // make the assignment operator private
    Kdf2& operator = (const Kdf2&);

  public:
    /// cerate a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
