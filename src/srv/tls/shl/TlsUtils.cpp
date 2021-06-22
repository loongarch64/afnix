// ---------------------------------------------------------------------------
// - TlsUtils.cpp                                                            -
// - afnix:tls service - tls utility class implementation                    -
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

#include "Pki.hpp"
#include "Time.hpp"
#include "Ascii.hpp"
#include "System.hpp"
#include "Unicode.hpp"
#include "Utility.hpp"
#include "TlsUtils.hpp"
#include "Exception.hpp"
#include "InputMapped.hpp"

namespace afnix {
  
  // convert bytes code to a word
  
  t_word TlsUtils::toword (const t_byte ucod, const t_byte lcod) {
    t_word result = ucod;
    result = result << 8; result += lcod;
    return result;
  }
  
  // convert a word to a upper byte code

  t_byte TlsUtils::toucod (const t_word code) {
    t_byte result = (t_byte) (code >> 8);
    return result;
  }

  // convert a word to a lower byte code

  t_byte TlsUtils::tolcod (const t_word code) {
    t_byte result = (t_byte) (code & 0x00FFU);
    return result;
  }
  
  // convert a code to a string
  
  String TlsUtils::toscod (const t_word code) {
    t_byte ucod = (t_byte) (code >> 8);
    t_byte lcod = (t_byte) (code & 0x00FF);
    String sval = Utility::tohexa (ucod, true, true) + ", " +
      Utility::tohexa (lcod, true, true);
    return sval;
  }

  // create a random buffer by size

  Buffer TlsUtils::random (const long size) {
    Buffer result (size);
    for (long k = 0L; k < size; k++) result.add ((char) Utility::byternd ());
    return result;
  }
  
  // create a random array by size and time flag

  t_byte* TlsUtils::random (const long size, const bool tflg) {
    // check the array size
    if (size < 0L) {
      throw Exception ("tls-error", "invalid random array size");
    }
    // check for nil
    if (size == 0L) return nullptr;
    // allocate the array and fill it
    t_byte* result = new t_byte[size];
    for (long k = 0; k < size; k++) result[k] = Utility::byternd ();
    // eventually set the unix time in the first 4 bytes
    if (tflg == true) {
      union {
	t_quad tunx;
	t_byte tval[4];
      };
      tunx = System::qswap (Time::getuclk ());
      result[0] = tval[0];
      result[1] = tval[1];
      result[2] = tval[2];
      result[3] = tval[3];
    }
    return result;
  }

  // the standard p_hash function of the tls

  Buffer TlsUtils::phash (Mac* hmac, const Buffer& seed, const long rlen) {
    // check for valid arguments
    if ((hmac == nullptr) || (seed.empty () == true) || (rlen <= 0L)) {
      throw Exception ("tls-error", "invalid arguments with p_hash");
    }
    // preset seed buffer
    long    slen = seed.length ();
    t_byte* sbuf = (t_byte*) seed.tochar ();
    try {
      // precompute A(1) = hmac (secret, seed)
      Buffer hbuf;
      hmac->reset   ();
      hmac->process (sbuf, slen);
      hmac->finish  ();
      hmac->pushb   (hbuf);
      // loop until enough bytes
      Buffer rbuf;
      while (rbuf.length () < rlen) {
	// save the buffer
	Buffer ibuf = hbuf;
	// one hash loop
	hmac->reset   ();
	hmac->process (hbuf);
	hmac->process (sbuf, slen);
	hmac->finish  ();
	hmac->pushb   (rbuf);
	// compute A1
	hmac->reset   ();
	hmac->process (ibuf);
	hmac->finish  ();
	hbuf.reset    ();
	hmac->pushb   (hbuf);
      }
      // trim extra bytes and lock
      rbuf.trim (rlen, false);
      delete [] sbuf;
      return rbuf;
    } catch (...) {
      delete [] sbuf;
      throw;
    }
  }
}
