// ---------------------------------------------------------------------------
// - AsnUtils.spp                                                            -
// - afnix:itu module - asn utilities implementation                         -
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

#include "Ascii.hpp"
#include "Regex.hpp"
#include "Bitset.hpp"
#include "Unicode.hpp"
#include "AsnUtils.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // check if a string is a valid numeric string

  bool AsnUtils::isnums (const String& sval) {
    // get the string length
    long slen = sval.length ();
    // check each character
    for (long i = 0; i < slen; i++) {
      // check for ascii
      t_quad q = sval[i];
      if (Unicode::isascii (q) == false) return false;
      // convert and verify
      char c = Unicode::tochar (q);
      if (Ascii::isdigit (c) == true) continue;
      if (c == ' ') continue;
      return false;
    }
    return true;
  }

  // check if a string is a valid printable string

  bool AsnUtils::isprts (const String& sval) {
    // get the string length
    long slen = sval.length ();
    // check each character
    for (long i = 0; i < slen; i++) {
      // check for ascii
      t_quad q = sval[i];
      if (Unicode::isascii (q) == false) return false;
      // convert and verify
      char c = Unicode::tochar (q);
      if (Ascii::isalpha (c) == true) continue;
      if ((c >= (char) 0x27) && (c <= (char) 0x29)) continue;
      if ((c >= (char) 0x2B) && (c <= (char) 0x2F)) continue;
      if (c == ':') continue;
      if (c == '=') continue;
      if (c == '?') continue;
      return false;
    }
    return true;
  }

  // check if a string is a valid ia string

  bool AsnUtils::isias (const String& sval) {
    // get the string length
    long slen = sval.length ();
    // check each character
    for (long i = 0; i < slen; i++) {
      if (Unicode::isascii (sval[i]) == false) return false;
    }
    return true;
  }

  // check if a string is a valid bmp string

  bool AsnUtils::isbmps (const String& sval) {
    // get the string length
    long slen = sval.length ();
    // check each character
    for (long i = 0; i < slen; i++) {
      if (Unicode::isbmp (sval[i]) == false) return false;
    }
    return true;
  }

  // check if a string is a generalized time

  bool AsnUtils::isgent (const String& sval) {
    // the base regex
    const String base = "($d$d$d$d)($d$d)($d$d)($d$d)($d$d)($d$d)";
    // the fraction part
    const String frac = "[.$d[$d]?[$d]?]?";
    // the zone part
    const String zone = "[<+->($d$d)($d$d)]?";
    // the zulu part
    const String zulu = "[Z]?";
    // check for local time or zulu
    const Regex lre (base + frac + zulu);
    if (lre == sval) return true;
    // check for time difference
    const Regex dre (base + frac + zone);
    return (dre == sval);    
  }

  // add an asn id to a buffer

  void AsnUtils::aidbuf (Buffer& buf, const t_octa aid) {
    // check for a null value
    if (aid == 0) {
      buf.add (nilc);
      return;
    }
    // create a bitset that can hold the octa value
    Bitset bs; bs.set (aid); bs.clamp (true);
    // mark first octet with msb bit to 0
    bs.add (7, false);
    // we keep adding until the length is a multiple of 8
    long pos = 8;
    while ((bs.length () % 8) != 0) {
      pos += 8;
      bs.add (pos-1, true);
    }
    // we have here a multiple of 8 bits - we can add them
    // in the buffer here
    long size = bs.length () - 8;
    for (long i = size; i >= 0; i -= 8) buf.add ((char) bs.tobyte (i));
  }

  // extract an asn identifier from a buffer

  t_octa AsnUtils::bufaid (Buffer& buf) {
    // the octa result
    t_octa result = 0;
    bool   status = false;
    // loop until empty or byte msb is set to 0
    while ((buf.empty () == false) && (status == false)) {
      // extract msb byte
      t_byte byte = (t_byte) buf.read ();
      // shift result by 7 bits
      result <<= 7;
      // add masked byte
      result |= (byte & 0x7F);
      // check for continuation
      if ((byte & 0x80) == 0x00) status = true;
    }
    if (status == false) {
      throw Exception ("asn-error", "invalid asn identifier encoding");
    }
    return result;
  }

  // extract an asn identifier from an input stream

  t_octa AsnUtils::mapaid (InputStream* is) {
    // check for stream
    if (is == nullptr) return 0;
    // the octa result
    t_octa result = 0;
    bool   status = false;
    // loop until empty or byte msb is set to 0
    while ((is->valid () == true) && (status == false)) {
      // extract msb byte
      t_byte byte = (t_byte) is->read ();
      // shift result by 7 bits
      result <<= 7;
      // add masked byte
      result |= (byte & 0x7F);
      // check for continuation
      if ((byte & 0x80) == 0x00) status = true;
    }
    if (status == false) {
      throw Exception ("asn-error", "invalid asn identifier encoding");
    }
    return result;
  }
}
