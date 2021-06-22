// ---------------------------------------------------------------------------
// - TlsBlock.cpp                                                            -
// - afnix:tls service - tls block class implementation                      -
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

#include "Byte.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "TlsBlock.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // get the block info as a plist

  Plist TlsBlock::getinfo (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist plst;
      // set the block info
      plst.add ("TLS-HDR-TYPE", "TLS HEADER TYPE",   (t_long) gettype ());
      plst.add ("TLS-HDR-RLEN", "TLS HEADER LENGTH", (t_long) length  ());
      // here it is
      unlock ();
      return plst;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map the block as a buffer

  Buffer TlsBlock::tobuffer (void) const {
    rdlock ();
    try {
      // get the buffer length
      long blen = length ();
      // fill the buffer
      Buffer result (blen);
      for (long k= 0L; k < blen; k++) result.add ((char) getbyte (k));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_GETTYPE = zone.intern ("get-type");
  static const long QUARK_GETBYTE = zone.intern ("get-byte");

  // return true if the given quark is defined

  bool TlsBlock::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? TlsInfos::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark

  Object* TlsBlock::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH)  return new Integer (length  ());
      if (quark == QUARK_GETTYPE) return new Byte    (gettype ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_GETBYTE) {
	long pos = argv->getlong (0);
	return new Byte (getbyte (pos));
      }
    }
    // call the tls infos method
    return TlsInfos::apply (zobj, nset, quark, argv);
  }
}
