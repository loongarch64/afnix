// ---------------------------------------------------------------------------
// - Prime.cpp                                                               -
// - standard object library - prime number function implementation          -
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

#include "Prime.hpp"
#include "Prime.hxx"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the number of miller-rabin base prime tests
  static const long MRT_PRIME_SIZE = 
    (128 > CPY_PRIME_SIZE) ? CPY_PRIME_SIZE : 128;

  // get the number of miller-rabin tests
  static inline long prv_getmni (const long msb) {
    return (msb >= 1300) ?  2 : 
           (msb >=  850) ?  3 :
           (msb >=  650) ?  4 : 
           (msb >=  550) ?  5 : 
           (msb >=  450) ?  6 :
           (msb >=  400) ?  7 :
           (msb >=  350) ?  8 :
           (msb >=  300) ?  9 :
           (msb >=  250) ? 12 : 
           (msb >=  200) ? 15 : 
           (msb >=  150) ? 18 : 27;
  }

  // generate a random relatif upto a maximum
  static inline Relatif prv_getrnd (const Relatif& p) {
    Relatif a = Relatif::random (p);
    return a.iszero () ? 2 : a;
  }

  // perform the fermat test
  static inline bool prv_isfmat (const Relatif& a, const Relatif& n,
				 const Relatif& p) {
    return Relatif::mme (a, n, p) == 1 ? true : false;
  }

  // perform the miller-rabin test
  static inline bool prv_ismrpt (const Relatif& a, const Relatif& n,
				 const Relatif& p) {
    // extract test components n = p - 1 = d.2^s
    t_long  s = n.getlsb () - 1;
    Relatif d = n >> s;
    // run the base test
    Relatif z = Relatif::mme (a, d, p);
    if (z == 1) return true;
    for (t_long i = 0; i < s; i++) {
      if (z == n) return true;
      z = Relatif::mme (z, 2, p);
      if (z == 1) return false;
    }
    return false;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // generate a table threshold prime number

  long Prime::mkthrp (const long value) {
    long num = (value < 0) ? -value : value;
    for (long i = 0; i < CPY_PLONG_SIZE; i++) {
      long prime = CPY_PLONG_BASE[i];
      if (prime > num) return prime;
    }
    return CPY_PLONG_BASE[CPY_PLONG_SIZE - 1];
  }

  // validate the little fermat theorem: a^p-1 (mod p) = 1

  bool Prime::fermat (const Relatif& a, const Relatif& p) {
    // check numbers
    if (p <= 1) {
      throw Exception ("prime-error", "invalid fermat prime number");
    }
    if ((a < 1) || (a >= p)) {
      throw Exception ("prime-error", "invalid fermat base number");
    }
    // run the fermat test
    return prv_isfmat (a, p - 1, p);
  }


  // run the miller-rabin primality test

  bool Prime::mrpt (const Relatif& a, const Relatif& p) {
    // check numbers
    if (p < 1) {
      throw Exception ("prime-error", "invalid miller-rabin prime number");
    }
    if ((a <= 1) || (a >= p-1)) {
      throw Exception ("prime-error", "invalid miller-rabin base number");
    }
    // check base numbers
    if (p == 1) return false;
    if (p == 2) return true;
    if (p.iseven () == true) return false;
    // run the miller-rabin test
    return prv_ismrpt (a, p - 1, p);
  }

  // return true if the number is a probable prime number

  bool Prime::isprobable (const Relatif& p, const long mni) {
    // reject negative number
    if (p < 0) {
      throw Exception ("prime-error", "invalid negative prime number");
    }
    // trivial case
    if (p == 0) return false;
    if (p == 1) return true;
    if (p == 2) return true;
    // even number
    if (p.iseven () == true) return false;

    // check with base prime numbers
    for (long i = 0; i < MRT_PRIME_SIZE; i++) {
      // get base number
      long prime = CPY_PRIME_BASE[i];
      // trivial equality
      if (p == prime) return true;
      // composite test
      Relatif r = p % prime;
      if (r.iszero () == true) return false;
    }

    // do the main test loop
    for (long i = 0; i < mni; i++) {
      // generate a random relatif
      Relatif a = prv_getrnd (p);
      // do a fast fermat test
      if (Prime::fermat (a, p) == false) return false;
      // do the miller-rabin test
      if (Prime::mrpt (a, p) == false) return false;
    }
    // yeap, probably prime
    return true;
  }

  // return true if the number is a probable prime number
 
  bool Prime::isprobable (const Relatif& p) {
    // miniumum number of iterations (HAC 4.49)
    long msb = p.getmsb ();
    long mni = prv_getmni (msb);
    // check for probable prime
    return Prime::isprobable (p, mni);
  }

  // return true is the number is a safe prime number

  bool Prime::issafe (const Relatif& p) {
    // check for prime
    if (Prime::isprobable (p) == false) return false;
    // check for 2*p+1
    return Prime::isprobable ((p - 1) >> 1);
  }
  
  // generate a random n bits prime numbers

  Relatif Prime::random (const long bits) {
    // check valid bit size
    if (bits <= 2) {
      throw Exception ("prime-error", "invalid size for random prime number");
    }
    // loop until found
    long maxc = (bits > 8) ? bits / 8 : 1;
    while (true) {
      long lcnt = 0L;
      Relatif result = Relatif::random (bits, true);
      do {
	if (Prime::isprobable (result) == true) return result;
	result += 2;
      } while ((lcnt++ < maxc) && (result.getmsb() == bits));
    }
    // we should never be here
    throw Exception ("prime-error", "premature random prime loop exit");
  }
  
  // generate a random n bits safe prime numbers

  Relatif Prime::safe (const long bits) {
    // preset tester
    long maxc = (bits > 8) ? bits / 8 : 1;
    // loop for prime
    while (true) {
      long lcnt = 0L;
      // generate initial candidate
      Relatif p = Relatif::random (bits, true);
      // ensure that p == 11 (12)
      if ((p % 12) != 11) continue;
      // loop locally
      do {
	// check for safe
	if (Prime::issafe (p) == true) return p;
	// next candidate
	p += 12;
      } while ((lcnt++ < maxc) && (p.getmsb() == bits));
    }
    // we should never be here
    throw Exception ("prime-error", "premature random safe loop exit");
  }
}
