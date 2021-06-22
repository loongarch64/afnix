// ---------------------------------------------------------------------------
// - Prime.hpp                                                               -
// - standard object library - prime number functions definition             -
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

#ifndef  AFNIX_PRIME_HPP
#define  AFNIX_PRIME_HPP

#ifndef  AFNIX_RELATIF_HPP
#include "Relatif.hpp"
#endif

namespace afnix {

  /// The Prime class is a collection of static convenient functions designed
  /// to manipulate prime numbers. These functions are designed to interact
  /// primarily with the cryptographic engine via the relatif numbers.
  /// @author amaury darsch

  class Prime {
  public:
    /// @return a table threshold prime number
    static long mkthrp (const long value);

    /// @return true if the fermat theorem is validated
    static bool fermat (const Relatif& a, const Relatif& p);

    /// return true if the miller-rabin primality test is validated
    static bool mrpt (const Relatif& a, const Relatif& p);

    /// @return true if the number is a probable prime number
    static bool isprobable (const Relatif& p, const long mni);

    /// @return true if the number is a probable prime number
    static bool isprobable (const Relatif& p);

    /// @return true if the number is a safe prime number
    static bool issafe (const Relatif& p);

    /// generate a random n bits prime number
    /// @param bits the number of bits
    static Relatif random (const long bits);

    /// generate a random n bits safe prime number
    /// @param bits the number of bits
    static Relatif safe (const long bits);
  };
}

#endif
