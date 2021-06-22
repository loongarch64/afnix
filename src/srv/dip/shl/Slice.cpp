// ---------------------------------------------------------------------------
// - Slice.cpp                                                               -
// - afnix:dip service - slice class implementation                          -
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

#include "Slice.hpp"
#include "Pixel.hxx"
#include "Vector.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // set a pixel component by buffer, index and format
  static long dip_topixl (Pixel::t_pixl& pixl, const Pixel::t_pfmt pfmt,
			  const Buffer&  pbuf, const long pidx) {
    // check for valid format
    if (pfmt == Pixel::PFMT_NONE) {
      throw Exception ("slice-error", "invalid nil buffer pixel format");
    }
    // check for byte
    if (pfmt == Pixel::PFMT_BYTE) {
      pixl.d_b[0] = pbuf.get (pidx);
      pixl.d_b[1] = pixl.d_b[0];
      pixl.d_b[2] = pixl.d_b[0];
      pixl.d_b[3] = 0xFFU;
      return pidx + 1L;
    }
    // check for word
    if (pfmt == Pixel::PFMT_WORD) {
      pixl.d_w[0]  = (t_word) pbuf.get (pidx); pixl.d_w[0] <<= 8;
      pixl.d_w[0] += (t_word) pbuf.get (pidx+1);
      pixl.d_w[1] = pixl.d_w[0];
      pixl.d_w[2] = pixl.d_w[0];
      pixl.d_w[3] = 0xFFFFU;
      return pidx + 2L;
    }
    // check for real
    if (pfmt == Pixel::PFMT_REAL) {
      union {
	t_byte d_b[8];
	t_real d_r;
      };
      for (long k = 0; k < 8; k++) d_b[k] =  pbuf.get (pidx+k);
      pixl.d_r[0] = d_r;
      pixl.d_r[1] = pixl.d_r[0];
      pixl.d_r[2] = pixl.d_r[0];
      pixl.d_r[3] = 1.0;
      return pidx + 8L;
    }
    // check for float
    if (pfmt == Pixel::PFMT_FLOT) {
      union {
	t_byte d_b[4];
	float  d_f;
      };
      for (long k = 0; k < 4; k++) d_b[k] =  pbuf.get (pidx+k);
      pixl.d_f[0] = d_f;
      pixl.d_f[1] = pixl.d_f[0];
      pixl.d_f[2] = pixl.d_f[0];
      pixl.d_f[3] = 1.0F;
      return pidx + 4L;
    }
    // check for rgba/bgra
    if ((pfmt == Pixel::PFMT_RGBA) || (pfmt == Pixel::PFMT_BGRA)) {
      for (long k = 0; k < 4; k++) pixl.d_b[k] =  pbuf.get (pidx+k);
      return pidx + 4L;
    }
    // check for rgbo/bgro
    if ((pfmt == Pixel::PFMT_RGBO) || (pfmt == Pixel::PFMT_BGRO)) {
      long k = pidx;
      pixl.d_w[0] = (t_word) pbuf.get (k++); pixl.d_w[0] <<= 8;
      pixl.d_w[0]+= (t_word) pbuf.get (k++);
      pixl.d_w[1] = (t_word) pbuf.get (k++); pixl.d_w[1] <<= 8;
      pixl.d_w[1]+= (t_word) pbuf.get (k++);
      pixl.d_w[2] = (t_word) pbuf.get (k++); pixl.d_w[2] <<= 8;
      pixl.d_w[2]+= (t_word) pbuf.get (k++);
      pixl.d_w[3] = (t_word) pbuf.get (k++); pixl.d_w[3] <<= 8;
      pixl.d_w[3]+= (t_word) pbuf.get (k++);
      return pidx + 8L;
    }
    // check for rgbr
    if (pfmt == Pixel::PFMT_RGBR) {
      union {
	t_byte d_b[8];
	t_real d_r;
      };
      for (long k = 0; k < 8; k++) d_b[k] = pbuf.get (pidx+k);
      pixl.d_r[0] = d_r;
      for (long k = 0; k < 8; k++) d_b[k] = pbuf.get (pidx+8+k);
      pixl.d_r[1] = d_r;
      for (long k = 0; k < 8; k++) d_b[k] = pbuf.get (pidx+16+k);
      pixl.d_r[2] = d_r;
      for (long k = 0; k < 8; k++) d_b[k] = pbuf.get (pidx+24+k);
      pixl.d_r[3] = d_r;
      return pidx + 32L;
    }
    // check for rgbf
    if (pfmt == Pixel::PFMT_RGBF) {
      union {
	t_byte d_b[4];
	float  d_f;
      };
      for (long k = 0; k < 4; k++) d_b[k] = pbuf.get (pidx+k);
      pixl.d_f[0] = d_f;
      for (long k = 0; k < 4; k++) d_b[k] = pbuf.get (pidx+4+k);
      pixl.d_f[1] = d_f;
      for (long k = 0; k < 4; k++) d_b[k] = pbuf.get (pidx+8+k);
      pixl.d_f[2] = d_f;
      for (long k = 0; k < 4; k++) d_b[k] = pbuf.get (pidx+12+k);
      pixl.d_f[3] = d_f;
      return pidx + 16L;
    }
    throw Exception ("slice-error", "invalid buffer pixel format");
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default slice

  Slice::Slice (void) {
    reset ();
  }
  
  // copy construct this slice

  Slice::Slice (const Slice& that) {
    that.rdlock ();
    try {
      Image::operator = (that);
      d_pfmt = that.d_pfmt;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }  
  }

  // move construct this slice

  Slice::Slice (Slice&& that) noexcept :
    Image (static_cast<Image&&>(that)) {
    that.wrlock ();
    try {
      d_pfmt = that.d_pfmt; that.d_pfmt = Pixel::PFMT_NONE;
      that.unlock ();
    } catch (...) {
      that.unlock ();
    }
  }

  // assign an slice to this one

  Slice& Slice::operator = (const Slice& that) {
    // check for self-move
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Image::operator = (that);
      d_pfmt = that.d_pfmt;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // move an slice to this one

  Slice& Slice::operator = (Slice&& that) noexcept {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      Object::operator = (static_cast<Image&&>(that));
      d_pfmt = that.d_pfmt; that.d_pfmt = Pixel::PFMT_NONE;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      return *this;
    }
  }
  
  // serialize this slice

  void Slice::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the image
      Image::wrstream (os);
      // serialize locally
      Serial::wrbyte (d_pfmt, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // deserialize this slice

  void Slice::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the image
      Image::rdstream (is);
      // deserialize locally
      d_pfmt = static_cast<Pixel::t_pfmt>(Serial::rdbyte (is));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // reset this slice

  void Slice::reset (void) {
    wrlock ();
    try {
      Image::reset ();
      d_pfmt = Pixel::PFMT_NONE;	    
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the slice format

  Pixel::t_pfmt Slice::getpfmt (void) const {
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

  // set a slice by buffer and format

  void Slice::setpixl (const Buffer& pbuf, const Pixel::t_pfmt pfmt) {
    wrlock ();
    try {
      // loop in the buffer
      long pidx = 0L;
      for (long x = 0; x < d_wdth; x++) {
	for (long y = 0; y < d_hght; y++) {
	  // extract the pixel by format
	  Pixel::t_pixl pixl;
	  pidx += dip_topixl (pixl, pfmt, pbuf, pidx);
	  // set the pixel by coordinate
	  setpixl (x, y, Pixel (pfmt, pixl));
	}
      }
      unlock ();
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
  static const long QUARK_CONVERT = zone.intern ("convert");
  static const long QUARK_GETPFMT = zone.intern ("get-format");
  static const long QUARK_SETPIXL = zone.intern ("set-pixel");
  static const long QUARK_GETPIXL = zone.intern ("get-pixel");

  // return true if the given quark is defined
  
  bool Slice::isquark (const long quark, const bool hflg) const {
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

  Object* Slice::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETPFMT) {
	return pfmt_to_item (getpfmt ());
      }
    }
    // dispatch 1 arguments
    if (argc == 1) {
      if (quark == QUARK_CONVERT) {
	Object* obj = argv->get (0);
	auto item = dynamic_cast<Item*>(obj);
	if (item == nullptr) {
	  throw Exception ("type-error", "invalid object with convert",
			   Object::repr (obj));
	}
	return convert (item_to_pfmt (*item));
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_GETPIXL) {
 	long x = argv->getlong (0);
 	long y = argv->getlong (1);
	return new Pixel (getpixl (x,y));
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_SETPIXL) {
 	long x = argv->getlong (0);
 	long y = argv->getlong (1);
	Object* obj = argv->get (2);
	auto pixl = dynamic_cast<Pixel*>(obj);
	if (pixl == nullptr) {
	  throw Exception ("type-error", "invalid object as pixel",
			   Object::repr (obj));
	}
	setpixl (x, y, *pixl);
	return nullptr;
      }
    }
    // call the image method
    return Image::apply (zobj, nset, quark, argv);
  }
}
