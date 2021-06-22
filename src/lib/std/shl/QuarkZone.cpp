// ---------------------------------------------------------------------------
// - QuarkZone.cpp                                                           -
// - standard object library - quark zone class implementation               -
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

#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create an empty quark zone

  QuarkZone::QuarkZone (void) {
    d_size = 0;
    d_zlen = 0;
    p_zone = nullptr;
  }

  // create a quark zone with a size

  QuarkZone::QuarkZone (const long size) {
    d_size = size;
    d_zlen = 0;
    p_zone = new long[size];
  }

  // copy construct a quark zone
  
  QuarkZone::QuarkZone (const QuarkZone& that) {
    d_size = that.d_size;
    d_zlen = that.d_zlen;
    p_zone = new long[d_size];
    for (long i = 0; i < d_zlen; i++) p_zone[i] = that.p_zone[i];
  }

  // assign a quark zone to this one

  QuarkZone& QuarkZone::operator = (const QuarkZone& that) {
    // check if we assign to ourself
    if (this == &that) return *this;
    // clean old zone
    delete [] p_zone;
    // copy new zone
    d_size = that.d_size;
    d_zlen = that.d_zlen;
    p_zone = new long[d_size];
    for (long i = 0; i < d_zlen; i++) p_zone[i] = that.p_zone[i];
    return *this;
  }

  // destroy this quark zone

  QuarkZone::~QuarkZone (void) {
    delete [] p_zone;
  }

  // reset this quark zone

  void QuarkZone::reset (void) {
    d_zlen = 0;
  }

  // return the zone length

  long QuarkZone::length (void) const {
    return d_zlen;
  }

  // intern a new strin in this zone

  long QuarkZone::intern (const String& value) {
    // check if we are full
    if (d_zlen == d_size) {
      long* zone = new long[++d_size];
      for (long i = 0; i < d_zlen; i++) zone[i] = p_zone[i];
      delete [] p_zone;
      p_zone = zone;
    }
    // intern the quark
    long quark = value.toquark ();
    p_zone[d_zlen++] = quark;
    return quark;
  }

  // return true if a quark exists

  bool QuarkZone::exists (const long quark) const {
    for (long i = 0; i < d_zlen; i++) {
      if (p_zone[i] == quark) return true;
    }
    return false;
  } 

  // get a quark by index
  
  long QuarkZone::get (const long index) const {
    // check index
    if ((index < 0) || (index >= d_zlen)) {
      throw Exception ("index-error", "index is out-of range");
    }
    return p_zone[index];
  }

  // map an interned quark to a string

  String QuarkZone::tostring (const long index) const {
    // check index
    if ((index < 0) || (index >= d_zlen)) {
      throw Exception ("index-error", "index is out-of range");
    }
    return String::qmap (p_zone[index]);
  }
}
