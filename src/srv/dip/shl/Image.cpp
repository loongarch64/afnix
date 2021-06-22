// ---------------------------------------------------------------------------
// - Image.cpp                                                              -
// - afnix:dip service - image class implementation                        -
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

#include "Image.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default image

  Image::Image (void) {
    reset ();
  }
  
  // copy construct this image

  Image::Image (const Image& that) {
    that.rdlock ();
    try {
      d_wdth = that.d_wdth;
      d_hght = that.d_hght;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }  
  }

  // move construct this image

  Image::Image (Image&& that) noexcept {
    that.wrlock ();
    try {
      // copy base object
      Object::operator = (static_cast<Serial&&>(that));
      Object::operator = (static_cast<Viewable&&>(that));
      // copy locally
      d_wdth = that.d_wdth; that.d_wdth = 0L;
      d_hght = that.d_hght; that.d_hght = 0L;
      that.unlock ();
    } catch (...) {
      that.unlock ();
    }
  }

  // assign an image to this one

  Image& Image::operator = (const Image& that) {
    // check for self-move
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_wdth = that.d_wdth;
      d_hght = that.d_hght;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // move an image to this one

  Image& Image::operator = (Image&& that) noexcept {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // move base objects
      Object::operator = (static_cast<Serial&&>(that));
      Object::operator = (static_cast<Viewable&&>(that));
      // move locally
      d_wdth = that.d_wdth; that.d_wdth = 0L;
      d_hght = that.d_hght; that.d_hght = 0L;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      return *this;
    }
  }

  // serialize this image

  void Image::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      Serial::wrlong (d_wdth, os);
      Serial::wrlong (d_hght, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // deserialize this image

  void Image::rdstream (InputStream& is) {
    wrlock ();
    try {
      d_wdth = Serial::rdlong (is);
      d_hght = Serial::rdlong (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // reset this image

  void Image::reset (void) {
    wrlock ();
    try {
      d_wdth = 0L;
      d_hght = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the image width

  long Image::getwdth (void) const {
    rdlock ();
    try {
      long result = d_wdth;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the image height

  long Image::gethght (void) const {
    rdlock ();
    try {
      long result = d_hght;
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET    = zone.intern ("reset");
  static const long QUARK_CLEAR    = zone.intern ("clear");
  static const long QUARK_GETWDTH  = zone.intern ("get-width");
  static const long QUARK_GETHGHT  = zone.intern ("get-height");

  // return true if the given quark is defined
  
  bool Image::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Viewable::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? Serial::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark

  Object* Image::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETWDTH) return new Integer (getwdth ());
      if (quark == QUARK_GETHGHT) return new Integer (gethght ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_CLEAR) {
	clear ();
	return nullptr;
      }
    }
    // check the signable method
    if (Viewable::isquark (quark, true) == true) {
      return Viewable::apply (zobj, nset, quark, argv);
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
