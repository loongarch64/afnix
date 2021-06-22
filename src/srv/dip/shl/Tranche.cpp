// ---------------------------------------------------------------------------
// - Tranche.cpp                                                             -
// - afnix:dip service - tranche class implementation                        -
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

#include "Pixel.hxx"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Tranche.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default tranche

  Tranche::Tranche (void) {
    reset ();
  }
  
  // copy construct this tranche

  Tranche::Tranche (const Tranche& that) {
    that.rdlock ();
    try {
      Image::operator = (that);
      d_pfmt = that.d_pfmt;
      d_dpth = that.d_dpth;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }  
  }

  // move construct this tranche

  Tranche::Tranche (Tranche&& that) noexcept :
    Image (static_cast<Image&&>(that)) {
    that.wrlock ();
    try {
      d_pfmt = that.d_pfmt; that.d_pfmt = Pixel::PFMT_NONE;
      d_dpth = that.d_dpth; that.d_dpth = 0L;
      that.unlock ();
    } catch (...) {
      that.unlock ();
    }
  }

  // assign an tranche to this one

  Tranche& Tranche::operator = (const Tranche& that) {
    // check for self-move
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Image::operator = (that);
      d_pfmt = that.d_pfmt;
      d_dpth = that.d_dpth;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // move an tranche to this one

  Tranche& Tranche::operator = (Tranche&& that) noexcept {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      Object::operator = (static_cast<Image&&>(that));
      d_pfmt = that.d_pfmt; that.d_pfmt = Pixel::PFMT_NONE;
      d_dpth = that.d_dpth; that.d_dpth = 0L;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      return *this;
    }
  }

  // serialize this tranche

  void Tranche::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the image
      Image::wrstream (os);
      // serialize locally
      Serial::wrbyte (d_pfmt, os);
      Serial::wrlong (d_dpth, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // deserialize this tranche

  void Tranche::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the image
      Image::rdstream (is);
      // deserialize locally
      d_pfmt = static_cast<Pixel::t_pfmt>(Serial::rdbyte (is));
      d_dpth = Serial::rdlong (is); 
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // reset this tranche

  void Tranche::reset (void) {
    wrlock ();
    try {
      Image::reset ();
      d_pfmt = Pixel::PFMT_NONE;
      d_dpth = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tranche format

  Pixel::t_pfmt Tranche::getpfmt (void) const {
    rdlock ();
    try {
      Pixel::t_pfmt result = d_pfmt;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the depth

  long Tranche::getdpth (void) const {
    rdlock ();
    try {
      long result = d_dpth;
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
  static const long QUARK_GETPFMT  = zone.intern ("get-format");
  static const long QUARK_GETDPTH  = zone.intern ("get-depth");
  static const long QUARK_SETPIXL  = zone.intern ("set-pixel");
  static const long QUARK_GETPIXL  = zone.intern ("get-pixel");

  // return true if the given quark is defined
  
  bool Tranche::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Image::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark

  Object* Tranche::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETDPTH) return new Integer (getdpth());
      if (quark == QUARK_GETPFMT) {
	return pfmt_to_item (getpfmt ());
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_GETPIXL) {
 	long x = argv->getlong (0);
 	long y = argv->getlong (1);
 	long d = argv->getlong (2);
	return new Pixel (getpixl (x,y, d));
      }
    }
    // dispatch 4 arguments
    if (argc == 4) {
      if (quark == QUARK_SETPIXL) {
 	long x = argv->getlong (0);
 	long y = argv->getlong (1);
 	long d = argv->getlong (2);
	Object* obj = argv->get (3);
	auto pixl = dynamic_cast<Pixel*>(obj);
	if (pixl == nullptr) {
	  throw Exception ("type-error", "invalid object as pixel",
			   Object::repr (obj));
	}
	setpixl (x, y, d, *pixl);
	return nullptr;
      }
    }
    // call the image method
    return Image::apply (zobj, nset, quark, argv);
  }
}
