// ---------------------------------------------------------------------------
// - Kdf1.hpp                                                                -
// - afnix:sec module - key derivation function (KDF1) class definition      -
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

#ifndef  AFNIX_KDF1_HPP
#define  AFNIX_KDF1_HPP

#ifndef  AFNIX_HKDF_HPP
#include "Hkdf.hpp"
#endif

namespace afnix {

  /// The Kdf1 class is a hashed key derivation function class that
  /// implements the KDF1 specification as defined by ISO/IEC 18033-2.
  /// The class is strictly equivalent to the mask generation function
  /// (MGF1) defined in PKCS 2.1. On the other hand, this implementation
  /// does not conform to the KDF1 specification of IEEE 1363-2000 which is
  /// somehow rather bizarre. The class operates in theory with any type of
  /// hashable object as long as the octet string is not too long.
  /// @author amaury darsch

  class Kdf1 : public Hkdf {
  public:
    /// create a hashed kdf1 object by hashable and size
    /// @param hobj the hashable object
    /// @param kbsz the key buffer size
    Kdf1 (Hashable* hobj, const long kbsz);

    /// @return the class name
    String repr (void) const;

    /// derive a key from an octet string by size
    /// @param ostr the octet string to process
    /// @param size the octet string size
    void derive (const t_byte* ostr, const long size);
 
  private:
    // make the copy constructor private
    Kdf1 (const Kdf1&);
    // make the assignment operator private
    Kdf1& operator = (const Kdf1&);

  public:
    /// cerate a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
