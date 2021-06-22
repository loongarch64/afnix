// ---------------------------------------------------------------------------
// - Pixel.cpp                                                               -
// - afnix:dip service - pixel class implementation                          -
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
#include "Math.hpp"
#include "Real.hpp"
#include "Pixel.hpp"
#include "Pixel.hxx"
#include "Dipsid.hxx"
#include "Vector.hpp"
#include "Utility.hpp"
#include "QuarkZone.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // convert a string to a pixel format
  Pixel::t_pfmt Pixel::topfmt (const String& pfmt) {
    // check for nil
    if (pfmt.isnil () == true) return Pixel::PFMT_NONE;
    // check for format
    String ufmt = pfmt.toupper();
    if (ufmt == "NONE") return Pixel::PFMT_NONE;
    if (ufmt == "BYTE") return Pixel::PFMT_BYTE;
    if (ufmt == "WORD") return Pixel::PFMT_WORD;
    if (ufmt == "REAL") return Pixel::PFMT_REAL;
    if (ufmt == "FLOT") return Pixel::PFMT_FLOT;
    if (ufmt == "RGBA") return Pixel::PFMT_RGBA;
    if (ufmt == "BGRA") return Pixel::PFMT_BGRA;
    if (ufmt == "RGBO") return Pixel::PFMT_RGBO;
    if (ufmt == "BGRO") return Pixel::PFMT_BGRO;
    if (ufmt == "RGBR") return Pixel::PFMT_RGBR;
    if (ufmt == "RGBF") return Pixel::PFMT_RGBF;
    // invalid format
    throw Exception ("pixel-error", "invalid pixel format", pfmt);
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default pixel

  Pixel::Pixel (void) {
    reset ();
  }
  
  // create a pixel by components
  
  Pixel::Pixel (const t_pfmt pfmt, const t_pixl pixl) {
    setpixl (pfmt, pixl);
  }

  // copy construct this pixel

  Pixel::Pixel (const Pixel& that) {
    that.rdlock ();
    try {
      d_pfmt = that.d_pfmt;
      d_pixl = that.d_pixl;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }  
  }

  // assign a pixel to this one

  Pixel& Pixel::operator = (const Pixel& that) {
    // check for self-move
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_pfmt = that.d_pfmt;
      d_pixl = that.d_pixl;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // return the class name

  String Pixel::repr (void) const {
    return "Pixel";
  }

  // return a clone of this object

  Object* Pixel::clone (void) const {
    return new Pixel (*this);
  }

  // return the serial did
  
  t_word Pixel::getdid (void) const {
    return SRL_DEOD_DIP;
  }
  
  // return the serial sid
  
  t_word Pixel::getsid (void) const {
    return SRL_PIXL_SID;
  }

  // serialize this pixmap

  void Pixel::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      Serial::wrbyte (d_pfmt, os);
      switch (d_pfmt) {
      case PFMT_NONE:
	break;
      case PFMT_BYTE:
      case PFMT_RGBA:
      case PFMT_BGRA:
	for (long k = 0L; k < 4; k++)
	  Serial::wrbyte (d_pixl.d_b[k], os);
	break;
      case PFMT_WORD:
      case PFMT_RGBO:
      case PFMT_BGRO:
	for (long k = 0L; k < 4; k++)
	  Serial::wrlong ((long) d_pixl.d_w[k], os);
	break;	  
      case PFMT_REAL:
      case PFMT_RGBR:
	for (long k = 0L; k < 4; k++)
	  Serial::wrreal (d_pixl.d_r[k], os);
	break;
      case PFMT_FLOT:
      case PFMT_RGBF:
	for (long k = 0L; k < 4; k++)
	  Serial::wrreal ((t_real) d_pixl.d_f[k], os);
	break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // deserialize this pixmap

  void Pixel::rdstream (InputStream& is) {
    wrlock ();
    try {
      d_pfmt = static_cast<t_pfmt>(Serial::rdbyte (is));
      switch (d_pfmt) {
      case PFMT_NONE:
	break;
      case PFMT_BYTE:
      case PFMT_RGBA:
      case PFMT_BGRA:
	for (long k = 0L; k < 4; k++)
	  d_pixl.d_b[k] = Serial::rdbyte (is);
	break;
      case PFMT_WORD:
      case PFMT_RGBO:
      case PFMT_BGRO:
	for (long k = 0L; k < 4; k++)
	  d_pixl.d_w[k] = (t_word) Serial::rdlong (is);
	break;	  
      case PFMT_REAL:
      case PFMT_RGBR:
	for (long k = 0L; k < 4; k++)
	  d_pixl.d_r[k] = Serial::rdreal (is);
	break;
      case PFMT_FLOT:
      case PFMT_RGBF:
	for (long k = 0L; k < 4; k++)
	  d_pixl.d_f[k] = (float) Serial::rdreal (is);
	break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // reset this pixel

  void Pixel::reset (void) {
    wrlock ();
    try {
      d_pfmt = PFMT_NONE;
      Utility::tonull (&d_pixl, sizeof (d_pixl));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // clear this pixel

  void Pixel::clear (void) {
    wrlock ();
    try {
      switch (d_pfmt) {
      case PFMT_NONE:
	Utility::tonull (&d_pixl, sizeof (d_pixl));
	break;
      case PFMT_BYTE:
      case PFMT_RGBA:
      case PFMT_BGRA:
	d_pixl.d_b[0] = 0x00U;
	d_pixl.d_b[1] = 0x00U;
	d_pixl.d_b[2] = 0x00U;
	d_pixl.d_b[3] = 0x00U;
	break;
      case PFMT_WORD:
      case PFMT_RGBO:
      case PFMT_BGRO:
	d_pixl.d_w[0] = 0x0000U;
	d_pixl.d_w[1] = 0x0000U;
	d_pixl.d_w[2] = 0x0000U;
	d_pixl.d_w[3] = 0x0000U;
	break;
      case PFMT_REAL:
      case PFMT_RGBR:
	d_pixl.d_r[0] = 0.0;
	d_pixl.d_r[1] = 0.0;
	d_pixl.d_r[2] = 0.0;
	d_pixl.d_r[3] = 0.0;
	break;
      case PFMT_FLOT:
      case PFMT_RGBF:
	d_pixl.d_f[0] = 0.0F;
	d_pixl.d_f[1] = 0.0F;
	d_pixl.d_f[2] = 0.0F;
	d_pixl.d_f[3] = 0.0F;
	break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a pixel format

  void Pixel::setpfmt (const t_pfmt pfmt) {
    wrlock ();
    try {
      d_pfmt = pfmt;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the pixel format

  Pixel::t_pfmt Pixel::getpfmt (void) const {
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
  
  // set a pixel component

  void Pixel::setpixl (const t_pixl pixl) {
    wrlock ();
    try {
      d_pixl = pixl;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a pixel component

  Pixel::t_pixl Pixel::getpixl (void) const {
    rdlock ();
    try {
      t_pixl result = d_pixl;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a pixel component by format

  void Pixel::setpixl (const t_pfmt pfmt, const t_pixl pixl) {
    wrlock ();
    try {
      d_pfmt = pfmt;
      d_pixl = pixl;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a component value by object

  void Pixel::setcval (Object* cobj) {
    wrlock ();
    try {
      // check for a byte
      auto bobj = dynamic_cast<Byte*>(cobj);
      if (bobj != nullptr) {
	switch (d_pfmt) {
	case PFMT_BYTE:
	  d_pixl.d_b[0] = bobj->tobyte ();
	  d_pixl.d_b[1] = d_pixl.d_b[0];
	  d_pixl.d_b[2] = d_pixl.d_b[0];
	  d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_WORD:
	  d_pixl.d_w[0] = (t_word) bobj->tobyte ();
	  d_pixl.d_w[1] = d_pixl.d_w[0];
	  d_pixl.d_w[2] = d_pixl.d_w[0];
	  d_pixl.d_w[3] = 0xFFFFU;
	  break;
	default:
	  break;
	}
      }
      // check for an integer
      auto iobj = dynamic_cast<Integer*>(cobj);
      if (iobj != nullptr) {
	switch (d_pfmt) {
	case PFMT_BYTE:
	  d_pixl.d_b[0] = (t_byte) iobj->tolong ();
	  d_pixl.d_b[1] = d_pixl.d_b[0];
	  d_pixl.d_b[2] = d_pixl.d_b[0];
	  d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_WORD:
	  d_pixl.d_w[0] = (t_word) iobj->tolong ();
	  d_pixl.d_w[1] = d_pixl.d_w[0];
	  d_pixl.d_w[2] = d_pixl.d_w[0];
	  d_pixl.d_w[3] = 0xFFFFU;
	  break;
	default:
	  break;
	}
      }
      // check for a real
      auto real = dynamic_cast<Real*>(cobj);
      if (real != nullptr) {
	switch (d_pfmt) {
	case PFMT_BYTE:
	  d_pixl.d_b[0] = (t_byte) real->tolong ();
	  d_pixl.d_b[1] = d_pixl.d_b[0];
	  d_pixl.d_b[2] = d_pixl.d_b[0];
	  d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_WORD:
	  d_pixl.d_w[0] = (t_word) real->tolong ();
	  d_pixl.d_w[1] = d_pixl.d_w[0];
	  d_pixl.d_w[2] = d_pixl.d_w[0];
	  d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_REAL:
	  d_pixl.d_r[0] = real->toreal ();
	  d_pixl.d_r[1] = d_pixl.d_r[0];
	  d_pixl.d_r[2] = d_pixl.d_r[0];
	  d_pixl.d_r[3] = 1.0;	  
	  break;
	case PFMT_FLOT:
	  d_pixl.d_f[0] = (float) real->toreal ();
	  d_pixl.d_f[1] = d_pixl.d_f[0];
	  d_pixl.d_f[2] = d_pixl.d_f[0];
	  d_pixl.d_f[3] = 1.0F;
	  break;
	default:
	  break;
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set a component value by object

  void Pixel::setcval (const long cidx, Object* cobj) {
    wrlock ();
    try {
      // check for valid index
      if ((cidx < 0L) || (cidx >= 4L)) {
	throw Exception ("pixel-error", "invalid component index in setcval");
      }
      // check for a byte
      auto bobj = dynamic_cast<Byte*>(cobj);
      if (bobj != nullptr) {
	switch (d_pfmt) {
	case PFMT_RGBA:
	case PFMT_BGRA:
	  d_pixl.d_b[cidx] = bobj->tobyte ();
	  break;
	case PFMT_RGBO:
	case PFMT_BGRO:
	  d_pixl.d_w[cidx] = (t_word) bobj->tobyte ();
	  break;
	default:
	  break;
	}
      }
      // check for an integer
      auto iobj = dynamic_cast<Integer*>(cobj);
      if (iobj != nullptr) {
	switch (d_pfmt) {
	case PFMT_RGBA:
	case PFMT_BGRA:
	  d_pixl.d_b[cidx] = (t_byte) iobj->tolong ();
	  break;
	case PFMT_RGBO:
	case PFMT_BGRO:
	  d_pixl.d_w[cidx] = (t_word) iobj->tolong ();
	  break;
	default:
	  break;
	}
      }
      // check for a real
      auto real = dynamic_cast<Real*>(cobj);
      if (real != nullptr) {
	switch (d_pfmt) {
	case PFMT_RGBA:
	case PFMT_BGRA:
	  d_pixl.d_b[cidx] = (t_byte) real->tolong ();
	  break;
	case PFMT_RGBO:
	case PFMT_BGRO:
	  d_pixl.d_w[cidx] = (t_word) real->tolong ();
	  break;
	case PFMT_RGBR:
	  d_pixl.d_r[cidx] = real->toreal ();
	  break;
	case PFMT_RGBF:
	  d_pixl.d_f[cidx] = (float) real->toreal ();
	  break;
	default:
	  break;
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a component object

  Object* Pixel::getcval (void) const {
    rdlock ();
    try {
      // process by format
      Object* result = nullptr;
      switch (d_pfmt) {
      case PFMT_BYTE:
	result = new Integer (d_pixl.d_b[0]);
	break;
      case PFMT_WORD:
	result = new Integer (d_pixl.d_w[0]);
	break;
      case PFMT_REAL:
	result = new Real (d_pixl.d_r[0]);
	break;
      case PFMT_FLOT:
	result = new Real (d_pixl.d_f[0]);
	break;
      default:
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a component object

  Object* Pixel::getcval (const long cidx) const {
    rdlock ();
    try {
      // check for valid index
      if ((cidx < 0L) || (cidx >= 4L)) {
	throw Exception ("pixel-error", "invalid component index in getcval");
      }
      // process by format
      Object* result = nullptr;
      switch (d_pfmt) {
      case PFMT_RGBA:
      case PFMT_BGRA:
	result = new Integer (d_pixl.d_b[cidx]);
	break;
      case PFMT_RGBO:
      case PFMT_BGRO:
	result = new Integer (d_pixl.d_w[cidx]);
	break;
      case PFMT_RGBR:
	result = new Real (d_pixl.d_r[cidx]);
	break;
      case PFMT_RGBF:
	result = new Real (d_pixl.d_f[cidx]);
	break;
      default:
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert this pixel to another format

  Pixel Pixel::convert (const t_pfmt pfmt) const {
    rdlock ();
    try {
      // prepare result
      Pixel result;
      // select from format
      switch (d_pfmt) {
      case PFMT_NONE:
	result.clear ();
	break;
      case PFMT_BYTE:
	// select from target
	switch (pfmt) {
	case PFMT_NONE:
	  result.clear ();
	  break;
	case PFMT_BYTE:
	case PFMT_RGBA:
	case PFMT_BGRA:
	  result.d_pixl.d_b[0] = d_pixl.d_b[0];
	  result.d_pixl.d_b[1] = d_pixl.d_b[0];
	  result.d_pixl.d_b[2] = d_pixl.d_b[0];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_WORD:
	case PFMT_RGBO:
	case PFMT_BGRO:
	  result.d_pixl.d_w[0] =
	    ((t_word) d_pixl.d_b[0] << 8) | ((t_word) d_pixl.d_b[0]);
	  result.d_pixl.d_w[1] = result.d_pixl.d_w[0];
	  result.d_pixl.d_w[2] = result.d_pixl.d_w[0];
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_REAL:
	case PFMT_RGBR:
	  result.d_pixl.d_r[0] = ((t_real) d_pixl.d_b[0]) / 255.0;
	  result.d_pixl.d_r[1] = result.d_pixl.d_r[0];
	  result.d_pixl.d_r[2] = result.d_pixl.d_r[0];
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_FLOT:
	case PFMT_RGBF:
	  result.d_pixl.d_f[0] = ((t_real) d_pixl.d_b[0]) / 255.0F;
	  result.d_pixl.d_f[1] = result.d_pixl.d_f[0];
	  result.d_pixl.d_f[2] = result.d_pixl.d_f[0];
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	}
	result.d_pfmt = pfmt;
	break;
      case PFMT_WORD:
	// select from target
	switch (pfmt) {
	case PFMT_NONE:
	  result.clear ();
	  break;
	case PFMT_BYTE:
	case PFMT_RGBA:
	case PFMT_BGRA:
	  result.d_pixl.d_b[0] = (t_byte) (d_pixl.d_w[0] >> 8);
	  result.d_pixl.d_b[1] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[2] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_WORD:
	case PFMT_RGBO:
	case PFMT_BGRO:
	  result.d_pixl.d_w[0] = d_pixl.d_w[0];
	  result.d_pixl.d_w[1] = d_pixl.d_w[0];
	  result.d_pixl.d_w[2] = d_pixl.d_w[0];
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_REAL:
	case PFMT_RGBR:
	  result.d_pixl.d_r[0] = ((t_real) d_pixl.d_w[0]) / 65535.0;
	  result.d_pixl.d_r[1] = result.d_pixl.d_r[0];
	  result.d_pixl.d_r[2] = result.d_pixl.d_r[0];
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_FLOT:
	case PFMT_RGBF:
	  result.d_pixl.d_f[0] = ((t_real) d_pixl.d_b[0]) / 65535.0F;
	  result.d_pixl.d_f[1] = result.d_pixl.d_f[0];
	  result.d_pixl.d_f[2] = result.d_pixl.d_f[0];
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	}
	break;
      case PFMT_REAL:
	// select from target
	switch (pfmt) {
	case PFMT_NONE:
	  result.clear ();
	  break;
	case PFMT_BYTE:
	case PFMT_RGBA:
	case PFMT_BGRA:
	  result.d_pixl.d_b[0] = (t_byte) (d_pixl.d_r[0] * 255.0);
	  result.d_pixl.d_b[1] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[2] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_WORD:
	case PFMT_RGBO:
	case PFMT_BGRO:
	  result.d_pixl.d_w[0] = (t_word) (d_pixl.d_r[0] * 65535.0);
	  result.d_pixl.d_w[1] = d_pixl.d_w[0];
	  result.d_pixl.d_w[2] = d_pixl.d_w[0];
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_REAL:
	case PFMT_RGBR:
	  result.d_pixl.d_r[0] = d_pixl.d_r[0];
	  result.d_pixl.d_r[1] = d_pixl.d_r[0];
	  result.d_pixl.d_r[2] = d_pixl.d_r[0];
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_FLOT:
	case PFMT_RGBF:
	  result.d_pixl.d_f[0] = (float) d_pixl.d_r[0];
	  result.d_pixl.d_f[1] = result.d_pixl.d_f[0];
	  result.d_pixl.d_f[2] = result.d_pixl.d_f[0];
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	}
	result.d_pfmt = pfmt;
	break;
      case PFMT_FLOT:
	// select from target
	switch (pfmt) {
	case PFMT_NONE:
	  result.clear ();
	  break;
	case PFMT_BYTE:
	case PFMT_RGBA:
	case PFMT_BGRA:
	  result.d_pixl.d_b[0] = (t_byte) (d_pixl.d_f[0] * 255.0F);
	  result.d_pixl.d_b[1] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[2] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_WORD:
	case PFMT_RGBO:
	case PFMT_BGRO:
	  result.d_pixl.d_w[0] = (t_word) (d_pixl.d_f[0] * 65535.0F);
	  result.d_pixl.d_w[1] = d_pixl.d_w[0];
	  result.d_pixl.d_w[2] = d_pixl.d_w[0];
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_REAL:
	case PFMT_RGBR:
	  result.d_pixl.d_r[0] = (t_real) d_pixl.d_f[0];
	  result.d_pixl.d_r[1] = d_pixl.d_r[0];
	  result.d_pixl.d_r[2] = d_pixl.d_r[0];
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_FLOT:
	case PFMT_RGBF:
	  result.d_pixl.d_f[0] = d_pixl.d_f[0];
	  result.d_pixl.d_f[1] = d_pixl.d_f[0];
	  result.d_pixl.d_f[2] = d_pixl.d_f[0];
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	}
	break;
      case PFMT_RGBA:
	// select from target
	switch (pfmt) {
	case PFMT_NONE:
	  result.clear ();
	  break;
	case PFMT_BYTE:
	  result.d_pixl.d_b[0] = (t_byte) (((t_quad) d_pixl.d_b[0] +
					    (t_quad) d_pixl.d_b[1] +
					    (t_quad) d_pixl.d_b[2]) / 3);
	  result.d_pixl.d_b[1] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[2] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_WORD:
	  result.d_pixl.d_w[0] = (t_word) (((t_quad) d_pixl.d_b[0] +
					    (t_quad) d_pixl.d_b[1] +
					    (t_quad) d_pixl.d_b[2]) / 3);
	  result.d_pixl.d_w[1] = d_pixl.d_w[0];
	  result.d_pixl.d_w[2] = d_pixl.d_w[0];
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_REAL:
	  result.d_pixl.d_r[0] = ((t_real) d_pixl.d_b[0] +
				  (t_real) d_pixl.d_b[1] +
				  (t_real) d_pixl.d_b[2]) / 3.0;
	  result.d_pixl.d_r[1] = d_pixl.d_r[0];
	  result.d_pixl.d_r[2] = d_pixl.d_r[0];
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_FLOT:
	  result.d_pixl.d_f[0] = ((float) d_pixl.d_b[0] +
				  (float) d_pixl.d_b[1] +
				  (float) d_pixl.d_b[2]) / 3.0F;
	  result.d_pixl.d_f[1] = d_pixl.d_f[0];
	  result.d_pixl.d_f[2] = d_pixl.d_f[0];
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	case PFMT_RGBA:
	  result.d_pixl.d_b[0] = d_pixl.d_b[0];
	  result.d_pixl.d_b[1] = d_pixl.d_b[1];
	  result.d_pixl.d_b[2] = d_pixl.d_b[2];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_BGRA:
	  result.d_pixl.d_b[0] = d_pixl.d_b[2];
	  result.d_pixl.d_b[1] = d_pixl.d_b[1];
	  result.d_pixl.d_b[2] = d_pixl.d_b[0];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_RGBO:
	  result.d_pixl.d_w[0]  = ((t_word) d_pixl.d_b[0]) << 8;
	  result.d_pixl.d_w[0] |= ((t_word) d_pixl.d_b[0]);
	  result.d_pixl.d_w[1]  = ((t_word) d_pixl.d_b[1]) << 8;
	  result.d_pixl.d_w[1] |= ((t_word) d_pixl.d_b[1]);
	  result.d_pixl.d_w[2]  = ((t_word) d_pixl.d_b[2]) << 8;
	  result.d_pixl.d_w[2] |= ((t_word) d_pixl.d_b[2]);
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_BGRO:
	  result.d_pixl.d_w[0]  = ((t_word) d_pixl.d_b[2]) << 8;
	  result.d_pixl.d_w[0] |= ((t_word) d_pixl.d_b[2]);
	  result.d_pixl.d_w[1]  = ((t_word) d_pixl.d_b[1]) << 8;
	  result.d_pixl.d_w[1] |= ((t_word) d_pixl.d_b[1]);
	  result.d_pixl.d_w[2]  = ((t_word) d_pixl.d_b[0]) << 8;
	  result.d_pixl.d_w[2] |= ((t_word) d_pixl.d_b[0]);
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_RGBR:
	  result.d_pixl.d_r[0] = ((t_real) d_pixl.d_b[0]) / 255.0;
	  result.d_pixl.d_r[1] = ((t_real) d_pixl.d_b[1]) / 255.0;
	  result.d_pixl.d_r[2] = ((t_real) d_pixl.d_b[2]) / 255.0;
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_RGBF:
	  result.d_pixl.d_f[0] = ((float) d_pixl.d_b[0]) / 255.0F;
	  result.d_pixl.d_f[1] = ((float) d_pixl.d_b[1]) / 255.0F;
	  result.d_pixl.d_f[2] = ((float) d_pixl.d_b[2]) / 255.0F;
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	}
	break;
      case PFMT_BGRA:
	// select from target
	switch (pfmt) {
	case PFMT_NONE:
	  result.clear ();
	  break;
	case PFMT_BYTE:
	  result.d_pixl.d_b[0] = (t_byte) (((t_quad) d_pixl.d_b[0] +
					    (t_quad) d_pixl.d_b[1] +
					    (t_quad) d_pixl.d_b[2]) / 3);
	  result.d_pixl.d_b[1] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[2] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_WORD:
	  result.d_pixl.d_w[0] = (t_word) (((t_quad) d_pixl.d_b[0] +
					    (t_quad) d_pixl.d_b[1] +
					    (t_quad) d_pixl.d_b[2]) / 3);
	  result.d_pixl.d_w[1] = d_pixl.d_w[0];
	  result.d_pixl.d_w[2] = d_pixl.d_w[0];
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_REAL:
	  result.d_pixl.d_r[0] = ((t_real) d_pixl.d_b[0] +
				  (t_real) d_pixl.d_b[1] +
				  (t_real) d_pixl.d_b[2]) / 3.0;
	  result.d_pixl.d_r[1] = d_pixl.d_r[0];
	  result.d_pixl.d_r[2] = d_pixl.d_r[0];
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_FLOT:
	  result.d_pixl.d_f[0] = ((float) d_pixl.d_b[0] +
				  (float) d_pixl.d_b[1] +
				  (float) d_pixl.d_b[2]) / 3.0F;
	  result.d_pixl.d_f[1] = d_pixl.d_f[0];
	  result.d_pixl.d_f[2] = d_pixl.d_f[0];
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	case PFMT_RGBA:
	  result.d_pixl.d_b[0] = d_pixl.d_b[2];
	  result.d_pixl.d_b[1] = d_pixl.d_b[1];
	  result.d_pixl.d_b[2] = d_pixl.d_b[0];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_BGRA:
	  result.d_pixl.d_b[0] = d_pixl.d_b[0];
	  result.d_pixl.d_b[1] = d_pixl.d_b[1];
	  result.d_pixl.d_b[2] = d_pixl.d_b[2];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_RGBO:
	  result.d_pixl.d_w[0]  = ((t_word) d_pixl.d_b[2]) << 8;
	  result.d_pixl.d_w[0] |= ((t_word) d_pixl.d_b[2]);
	  result.d_pixl.d_w[1]  = ((t_word) d_pixl.d_b[1]) << 8;
	  result.d_pixl.d_w[1] |= ((t_word) d_pixl.d_b[1]);
	  result.d_pixl.d_w[2]  = ((t_word) d_pixl.d_b[0]) << 8;
	  result.d_pixl.d_w[2] |= ((t_word) d_pixl.d_b[0]);
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_BGRO:
	  result.d_pixl.d_w[0]  = ((t_word) d_pixl.d_b[0]) << 8;
	  result.d_pixl.d_w[0] |= ((t_word) d_pixl.d_b[0]);
	  result.d_pixl.d_w[1]  = ((t_word) d_pixl.d_b[1]) << 8;
	  result.d_pixl.d_w[1] |= ((t_word) d_pixl.d_b[1]);
	  result.d_pixl.d_w[2]  = ((t_word) d_pixl.d_b[2]) << 8;
	  result.d_pixl.d_w[2] |= ((t_word) d_pixl.d_b[2]);
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_RGBR:
	  result.d_pixl.d_r[0] = ((t_real) d_pixl.d_b[2]) / 255.0;
	  result.d_pixl.d_r[1] = ((t_real) d_pixl.d_b[1]) / 255.0;
	  result.d_pixl.d_r[2] = ((t_real) d_pixl.d_b[0]) / 255.0;
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_RGBF:
	  result.d_pixl.d_f[0] = ((float) d_pixl.d_b[2]) / 255.0F;
	  result.d_pixl.d_f[1] = ((float) d_pixl.d_b[1]) / 255.0F;
	  result.d_pixl.d_f[2] = ((float) d_pixl.d_b[0]) / 255.0F;
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	}
	break;
      case PFMT_RGBO:
	// select from target
	switch (pfmt) {
	case PFMT_NONE:
	  result.clear ();
	  break;
	case PFMT_BYTE:
	  result.d_pixl.d_b[0] = (t_byte) (((t_quad) d_pixl.d_w[0] +
					    (t_quad) d_pixl.d_w[1] +
					    (t_quad) d_pixl.d_w[2]) / 3);
	  result.d_pixl.d_b[1] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[2] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_WORD:
	  result.d_pixl.d_w[0] = (t_word) (((t_quad) d_pixl.d_w[0] +
					    (t_quad) d_pixl.d_w[1] +
					    (t_quad) d_pixl.d_w[2]) / 3);
	  result.d_pixl.d_w[1] = d_pixl.d_w[0];
	  result.d_pixl.d_w[2] = d_pixl.d_w[0];
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_REAL:
	  result.d_pixl.d_r[0] = ((t_real) d_pixl.d_w[0] +
				  (t_real) d_pixl.d_w[1] +
				  (t_real) d_pixl.d_w[2]) / 3.0;
	  result.d_pixl.d_r[1] = d_pixl.d_r[0];
	  result.d_pixl.d_r[2] = d_pixl.d_r[0];
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_FLOT:
	  result.d_pixl.d_f[0] = ((float) d_pixl.d_w[0] +
				  (float) d_pixl.d_w[1] +
				  (float) d_pixl.d_w[2]) / 3.0F;
	  result.d_pixl.d_f[1] = d_pixl.d_f[0];
	  result.d_pixl.d_f[2] = d_pixl.d_f[0];
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	case PFMT_RGBA:
	  result.d_pixl.d_b[0] = (t_byte) d_pixl.d_w[0];
	  result.d_pixl.d_b[1] = (t_byte) d_pixl.d_w[1];
	  result.d_pixl.d_b[2] = (t_byte) d_pixl.d_w[2];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_BGRA:
	  result.d_pixl.d_b[0] = (t_byte) d_pixl.d_w[2];
	  result.d_pixl.d_b[1] = (t_byte) d_pixl.d_w[1];
	  result.d_pixl.d_b[2] = (t_byte) d_pixl.d_w[0];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_RGBO:
	  result.d_pixl.d_w[0] = d_pixl.d_w[0];
	  result.d_pixl.d_w[1] = d_pixl.d_w[1];
	  result.d_pixl.d_w[2] = d_pixl.d_w[2];
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_BGRO:
	  result.d_pixl.d_w[0] = d_pixl.d_w[2];
	  result.d_pixl.d_w[1] = d_pixl.d_w[1];
	  result.d_pixl.d_w[2] = d_pixl.d_w[0];
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_RGBR:
	  result.d_pixl.d_r[0] = ((t_real) d_pixl.d_w[0]) / 65535.0;
	  result.d_pixl.d_r[1] = ((t_real) d_pixl.d_w[1]) / 65535.0;
	  result.d_pixl.d_r[2] = ((t_real) d_pixl.d_w[2]) / 65535.0;
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_RGBF:
	  result.d_pixl.d_f[0] = ((float) d_pixl.d_w[0]) / 65535.0F;
	  result.d_pixl.d_f[1] = ((float) d_pixl.d_w[1]) / 65535.0F;
	  result.d_pixl.d_f[2] = ((float) d_pixl.d_w[2]) / 65535.0F;
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	}
	break;
      case PFMT_BGRO:
	// select from target
	switch (pfmt) {
	case PFMT_NONE:
	  result.clear ();
	  break;
	case PFMT_BYTE:
	  result.d_pixl.d_b[0] = (t_byte) (((t_quad) d_pixl.d_w[0] +
					    (t_quad) d_pixl.d_w[1] +
					    (t_quad) d_pixl.d_w[2]) / 3);
	  result.d_pixl.d_b[1] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[2] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_WORD:
	  result.d_pixl.d_w[0] = (t_word) (((t_quad) d_pixl.d_w[0] +
					    (t_quad) d_pixl.d_w[1] +
					    (t_quad) d_pixl.d_w[2]) / 3);
	  result.d_pixl.d_w[1] = d_pixl.d_w[0];
	  result.d_pixl.d_w[2] = d_pixl.d_w[0];
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_REAL:
	  result.d_pixl.d_r[0] = ((t_real) d_pixl.d_w[0] +
				  (t_real) d_pixl.d_w[1] +
				  (t_real) d_pixl.d_w[2]) / 3.0;
	  result.d_pixl.d_r[1] = d_pixl.d_r[0];
	  result.d_pixl.d_r[2] = d_pixl.d_r[0];
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_FLOT:
	  result.d_pixl.d_f[0] = ((float) d_pixl.d_w[0] +
				  (float) d_pixl.d_w[1] +
				  (float) d_pixl.d_w[2]) / 3.0F;
	  result.d_pixl.d_f[1] = d_pixl.d_f[0];
	  result.d_pixl.d_f[2] = d_pixl.d_f[0];
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	case PFMT_RGBA:
	  result.d_pixl.d_b[0] = (t_byte) d_pixl.d_w[2];
	  result.d_pixl.d_b[1] = (t_byte) d_pixl.d_w[1];
	  result.d_pixl.d_b[2] = (t_byte) d_pixl.d_w[0];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_BGRA:
	  result.d_pixl.d_b[0] = (t_byte) d_pixl.d_w[0];
	  result.d_pixl.d_b[1] = (t_byte) d_pixl.d_w[1];
	  result.d_pixl.d_b[2] = (t_byte) d_pixl.d_w[2];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_RGBO:
	  result.d_pixl.d_w[0] = d_pixl.d_w[2];
	  result.d_pixl.d_w[1] = d_pixl.d_w[1];
	  result.d_pixl.d_w[2] = d_pixl.d_w[0];
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_BGRO:
	  result.d_pixl.d_w[0] = d_pixl.d_w[0];
	  result.d_pixl.d_w[1] = d_pixl.d_w[1];
	  result.d_pixl.d_w[2] = d_pixl.d_w[2];
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_RGBR:
	  result.d_pixl.d_r[0] = ((t_real) d_pixl.d_w[2]) / 65535.0;
	  result.d_pixl.d_r[1] = ((t_real) d_pixl.d_w[1]) / 65535.0;
	  result.d_pixl.d_r[2] = ((t_real) d_pixl.d_w[0]) / 65535.0;
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_RGBF:
	  result.d_pixl.d_f[0] = ((float) d_pixl.d_w[2]) / 65535.0F;
	  result.d_pixl.d_f[1] = ((float) d_pixl.d_w[1]) / 65535.0F;
	  result.d_pixl.d_f[2] = ((float) d_pixl.d_w[0]) / 65535.0F;
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	}
	break;
      case PFMT_RGBR:
	// select from target
	switch (pfmt) {
	case PFMT_NONE:
	  result.clear ();
	  break;
	case PFMT_BYTE:
	  result.d_pixl.d_b[0] = (t_byte) ((d_pixl.d_r[0] * 255.0 +
					    d_pixl.d_r[1] * 255.0 +
					    d_pixl.d_r[2] * 255.0) / 3.0);
	  result.d_pixl.d_b[1] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[2] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_WORD:
	  result.d_pixl.d_w[0] = (t_word) ((d_pixl.d_r[0] * 255.0 +
					    d_pixl.d_r[1] * 255.0 +
					    d_pixl.d_r[2] * 255.0) / 3.0);
	  result.d_pixl.d_w[1] = d_pixl.d_w[0];
	  result.d_pixl.d_w[2] = d_pixl.d_w[0];
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_REAL:
	  result.d_pixl.d_r[0] = (d_pixl.d_r[0] + d_pixl.d_r[1] +
				  d_pixl.d_r[2]) / 3.0;
	  result.d_pixl.d_r[1] = d_pixl.d_r[0];
	  result.d_pixl.d_r[2] = d_pixl.d_r[0];
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_FLOT:
	  result.d_pixl.d_f[0] = ((float) (d_pixl.d_r[0] + d_pixl.d_r[1] +
					   d_pixl.d_r[2]) / 3.0);
	  result.d_pixl.d_f[1] = d_pixl.d_f[0];
	  result.d_pixl.d_f[2] = d_pixl.d_f[0];
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	case PFMT_RGBA:
	  result.d_pixl.d_b[0] = (t_byte) (d_pixl.d_r[0] * 255.0);
	  result.d_pixl.d_b[1] = (t_byte) (d_pixl.d_r[1] * 255.0);
	  result.d_pixl.d_b[2] = (t_byte) (d_pixl.d_r[2] * 255.0);
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_BGRA:
	  result.d_pixl.d_b[0] = (t_byte) (d_pixl.d_r[2] * 255.0);
	  result.d_pixl.d_b[1] = (t_byte) (d_pixl.d_r[1] * 255.0);
	  result.d_pixl.d_b[2] = (t_byte) (d_pixl.d_r[0] * 255.0);
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_RGBO:
	  result.d_pixl.d_w[0] = (t_word) (d_pixl.d_r[0] * 255.0);
	  result.d_pixl.d_w[1] = (t_word) (d_pixl.d_r[1] * 255.0);
	  result.d_pixl.d_w[2] = (t_word) (d_pixl.d_r[2] * 255.0);
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_BGRO:
	  result.d_pixl.d_w[0] = (t_word) (d_pixl.d_r[2] * 255.0);
	  result.d_pixl.d_w[1] = (t_word) (d_pixl.d_r[1] * 255.0);
	  result.d_pixl.d_w[2] = (t_word) (d_pixl.d_r[0] * 255.0);
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_RGBR:
	  result.d_pixl.d_r[0] = d_pixl.d_r[0];
	  result.d_pixl.d_r[1] = d_pixl.d_r[1];
	  result.d_pixl.d_r[2] = d_pixl.d_r[2];
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_RGBF:
	  result.d_pixl.d_f[0] = (float) d_pixl.d_r[0];
	  result.d_pixl.d_f[1] = (float) d_pixl.d_r[1];
	  result.d_pixl.d_f[2] = (float) d_pixl.d_r[2];
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	}
	break;
      case PFMT_RGBF:
	// select from target
	switch (pfmt) {
	case PFMT_NONE:
	  result.clear ();
	  break;
	case PFMT_BYTE:
	  result.d_pixl.d_b[0] = (t_byte) ((d_pixl.d_f[0] * 255.0F +
					    d_pixl.d_f[1] * 255.0F +
					    d_pixl.d_f[2] * 255.0F) / 3.0F);
	  result.d_pixl.d_b[1] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[2] = result.d_pixl.d_b[0];
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_WORD:
	  result.d_pixl.d_w[0] = (t_word) ((d_pixl.d_f[0] * 255.0F +
					    d_pixl.d_f[1] * 255.0F +
					    d_pixl.d_f[2] * 255.0F) / 3.0F);
	  result.d_pixl.d_w[1] = d_pixl.d_w[0];
	  result.d_pixl.d_w[2] = d_pixl.d_w[0];
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_REAL:
	  result.d_pixl.d_r[0] = (d_pixl.d_f[0] + d_pixl.d_f[1] +
				  d_pixl.d_f[2]) / 3.0F;
	  result.d_pixl.d_r[1] = d_pixl.d_r[0];
	  result.d_pixl.d_r[2] = d_pixl.d_r[0];
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_FLOT:
	  result.d_pixl.d_f[0] = (d_pixl.d_f[0] + d_pixl.d_f[1] +
				  d_pixl.d_f[2]) / 3.0F;
	  result.d_pixl.d_f[1] = d_pixl.d_f[0];
	  result.d_pixl.d_f[2] = d_pixl.d_f[0];
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	case PFMT_RGBA:
	  result.d_pixl.d_b[0] = (t_byte) (d_pixl.d_f[0] * 255.0F);
	  result.d_pixl.d_b[1] = (t_byte) (d_pixl.d_f[1] * 255.0F);
	  result.d_pixl.d_b[2] = (t_byte) (d_pixl.d_f[2] * 255.0F);
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_BGRA:
	  result.d_pixl.d_b[0] = (t_byte) (d_pixl.d_f[2] * 255.0F);
	  result.d_pixl.d_b[1] = (t_byte) (d_pixl.d_f[1] * 255.0F);
	  result.d_pixl.d_b[2] = (t_byte) (d_pixl.d_f[0] * 255.0F);
	  result.d_pixl.d_b[3] = 0xFFU;
	  break;
	case PFMT_RGBO:
	  result.d_pixl.d_w[0] = (t_word) (d_pixl.d_f[0] * 255.0F);
	  result.d_pixl.d_w[1] = (t_word) (d_pixl.d_f[1] * 255.0F);
	  result.d_pixl.d_w[2] = (t_word) (d_pixl.d_f[2] * 255.0F);
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_BGRO:
	  result.d_pixl.d_w[0] = (t_word) (d_pixl.d_f[2] * 255.0F);
	  result.d_pixl.d_w[1] = (t_word) (d_pixl.d_f[1] * 255.0F);
	  result.d_pixl.d_w[2] = (t_word) (d_pixl.d_f[0] * 255.0F);
	  result.d_pixl.d_w[3] = 0xFFFFU;
	  break;
	case PFMT_RGBR:
	  result.d_pixl.d_r[0] = (t_real) d_pixl.d_f[0];
	  result.d_pixl.d_r[1] = (t_real) d_pixl.d_f[1];
	  result.d_pixl.d_r[2] = (t_real) d_pixl.d_f[2];
	  result.d_pixl.d_r[3] = 1.0;
	  break;
	case PFMT_RGBF:
	  result.d_pixl.d_f[0] = d_pixl.d_f[0];
	  result.d_pixl.d_f[1] = d_pixl.d_f[1];
	  result.d_pixl.d_f[2] = d_pixl.d_f[2];
	  result.d_pixl.d_f[3] = 1.0F;
	  break;
	}
	break;
      }
      result.d_pfmt = pfmt;
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
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_CLEAR   = zone.intern ("clear");
  static const long QUARK_CONVERT = zone.intern ("convert");
  static const long QUARK_SETPFMT = zone.intern ("set-format");
  static const long QUARK_GETPFMT = zone.intern ("get-format");
  static const long QUARK_SETCVAL = zone.intern ("set-component");
  static const long QUARK_GETCVAL = zone.intern ("get-component");

  // evaluate an object data member

  Object* Pixel::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (quark == QUARK_PFMTNONE) return new Item (QUARK_PIXEL, QUARK_PFMTNONE);
    if (quark == QUARK_PFMTBYTE) return new Item (QUARK_PIXEL, QUARK_PFMTBYTE);
    if (quark == QUARK_PFMTWORD) return new Item (QUARK_PIXEL, QUARK_PFMTWORD);
    if (quark == QUARK_PFMTREAL) return new Item (QUARK_PIXEL, QUARK_PFMTREAL);
    if (quark == QUARK_PFMTFLOT) return new Item (QUARK_PIXEL, QUARK_PFMTFLOT);
    if (quark == QUARK_PFMTRGBA) return new Item (QUARK_PIXEL, QUARK_PFMTRGBA);
    if (quark == QUARK_PFMTBGRA) return new Item (QUARK_PIXEL, QUARK_PFMTBGRA);
    if (quark == QUARK_PFMTRGBO) return new Item (QUARK_PIXEL, QUARK_PFMTRGBO);
    if (quark == QUARK_PFMTBGRO) return new Item (QUARK_PIXEL, QUARK_PFMTBGRO);
    if (quark == QUARK_PFMTRGBR) return new Item (QUARK_PIXEL, QUARK_PFMTRGBR);
    if (quark == QUARK_PFMTRGBF) return new Item (QUARK_PIXEL, QUARK_PFMTRGBF);
    // invalid quark
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }

  // create a new object in a generic way
  
  Object* Pixel::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Pixel;
    throw Exception ("argument-error", 
                     "invalid arguments with with pixmap constructor"); 
  }
  
  // return true if the given quark is defined
  
  bool Pixel::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark

  Object* Pixel::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETCVAL) return getcval ();
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_CLEAR) {
	clear ();
	return nullptr;
      }
      if (quark == QUARK_GETPFMT) {
	return pfmt_to_item (getpfmt ());
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETPFMT) {
	Object* obj = argv->get (0);
	auto item = dynamic_cast<Item*>(obj);
	if (item == nullptr) {
	  throw Exception ("type-error", "invalid object with set-format",
			   Object::repr (obj));
	}
	setpfmt(item_to_pfmt (*item));
	return nullptr;
      }
      if (quark == QUARK_SETCVAL) {
	Object* cobj = argv->get (0);
	setcval (cobj);
	return nullptr;
      }
      if (quark == QUARK_GETCVAL) {
	long cidx = argv->getlong (0);
	return getcval (cidx);
      }
      if (quark == QUARK_CONVERT) {
	Object* obj = argv->get (0);
	auto item = dynamic_cast<Item*>(obj);
	if (item == nullptr) {
	  throw Exception ("type-error", "invalid object with convert",
			   Object::repr (obj));
	}
	Pixel pixl = convert (item_to_pfmt (*item));
	return new Pixel(pixl);
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETCVAL) {
	long cidx = argv->getlong (0);
	Object* cobj = argv->get (1);
	setcval (cidx, cobj);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
