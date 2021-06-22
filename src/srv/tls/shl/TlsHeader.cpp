// ---------------------------------------------------------------------------
// - TlsHeader.cpp                                                           -
// - afnix:tls service - tls record header class implementation              -
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
#include "Utility.hpp"
#include "Integer.hpp"
#include "TlsHeader.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // get the string version

  String TlsHeader::toversion (void) const {
    rdlock ();
    try {
      String result = Utility::tostring ((t_long) getmajor ());
      result += '.';
      result += Utility::tostring ((t_long) getminor ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the record info as a plist

  Plist TlsHeader::getinfo (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist plst = TlsBlock::getinfo ();
      // set the header info
      plst.add ("TLS-HDR-VMAJ", "TLS HEADER MAJOR VERSION", 
		(t_long) getmajor ());
      plst.add ("TLS-HDR-VMIN", "TLS HEADER MINOR VERSION",
		(t_long) getminor ());
      // here it is
      unlock ();
      return plst;
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
  static const long QUARK_MAJOR   = zone.intern ("get-major-version");
  static const long QUARK_MINOR   = zone.intern ("get-major-version");
  static const long QUARK_VERSION = zone.intern ("to-version");

  // return true if the given quark is defined

  bool TlsHeader::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? TlsBlock::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark

  Object* TlsHeader::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_MAJOR)   return new Integer (getmajor  ());
      if (quark == QUARK_MINOR)   return new Integer (getminor  ());
      if (quark == QUARK_VERSION) return new String  (toversion ());
    }
    // call the block method
    return TlsBlock::apply (zobj, nset, quark, argv);
  }
}
