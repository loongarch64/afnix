// ---------------------------------------------------------------------------
// - Mixmap.cpp                                                              -
// - afnix:dip service - mixmap class implementation                         -
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
#include "Dipsid.hxx"
#include "Vector.hpp"
#include "Mixmap.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure computes the derived band format
  static inline Pixel::t_pfmt mixm_to_bfmt (const Pixel::t_pfmt pfmt) {
    Pixel::t_pfmt result = Pixel::PFMT_NONE;
    switch (pfmt) {
    case Pixel::PFMT_BYTE:
    case Pixel::PFMT_WORD:
    case Pixel::PFMT_REAL:
    case Pixel::PFMT_FLOT:
      result = pfmt;
      break;
    case Pixel::PFMT_RGBA:
    case Pixel::PFMT_BGRA:
      result = Pixel::PFMT_BYTE;
      break;
    case Pixel::PFMT_RGBO:
    case Pixel::PFMT_BGRO:
      result = Pixel::PFMT_WORD;
      break;
    case Pixel::PFMT_RGBR:
      result = Pixel::PFMT_REAL;
      break;
    case Pixel::PFMT_RGBF:
      result = Pixel::PFMT_FLOT;
      break;
    default:
      result = Pixel::PFMT_NONE;
      break;
    }
    return result;
  }
  
  // this procedure computes the mixmap stride by format
  static inline long mixm_to_strd (const Pixel::t_pfmt pfmt, const long wdth) {
    // check for valid argument
    if ((wdth <= 0L) || (pfmt == Pixel::PFMT_NONE)) return 0L;
    // compute stride by format
    long result = 0L;
    switch (pfmt) {
    case Pixel::PFMT_BYTE:
      result = ((wdth % 64) == 0L) ? wdth : ((wdth / 64) + 1) * 64;
      break;
    case Pixel::PFMT_WORD:
      result = ((wdth % 64) == 0L) ? (wdth * 2) : ((wdth / 64) + 1) * 64 * 2;
      break;
    case Pixel::PFMT_REAL:
      result = wdth * sizeof (t_real);
      break;
    case Pixel::PFMT_FLOT:
      result = wdth * sizeof (float);
      break;
    case Pixel::PFMT_RGBA:
    case Pixel::PFMT_BGRA:
      result = ((wdth % 64) == 0L) ? (wdth * 4) : ((wdth / 64) + 1) * 64 * 4;
      break;
    case Pixel::PFMT_RGBO:
    case Pixel::PFMT_BGRO:
      result = ((wdth % 64) == 0L) ? (wdth * 8) : ((wdth / 64) + 1) * 64 * 8;
      break;
    case Pixel::PFMT_RGBR:
      result = wdth * 4 * sizeof (t_real);
      break;
    case Pixel::PFMT_RGBF:
      result = wdth * 4 * sizeof (float);
      break;
    default:
      result = 0L;
      break;
    }
    return result;
  }
  
  // this procedure compute the mixmap byte size by format and geometry
  static inline long mixm_to_size (const long rtrd, const long btrd,
				   const long hght, const long dpth) {
    // check for valid argument;
    if ((rtrd <= 0L) || (hght <= 0L) || (dpth <= 0L)) return 0L;
    // compute result
    long result = (rtrd * hght) + (btrd * hght * (dpth - 1L));
    return result;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default mixmap

  Mixmap::Mixmap (void) {
    d_pfmt = Pixel::PFMT_NONE;
    p_data = nullptr;
    reset ();
  }

  // create a mixmap by width and height

  Mixmap::Mixmap (const Pixel::t_pfmt pfmt, const long wdth, const long hght,
		  const long dpth) {
    d_pfmt = pfmt;
    d_rtrd = mixm_to_strd (d_pfmt, wdth);
    d_btrd = mixm_to_strd (mixm_to_bfmt (d_pfmt), wdth);
    d_size = mixm_to_size (d_rtrd, d_btrd, hght, dpth);
    if (d_size > 0L) {
      d_wdth = wdth;
      d_hght = hght;
      d_dpth = dpth;
      p_data = new t_byte[d_size];
    } else {
      d_pfmt = Pixel::PFMT_NONE;
      d_rtrd = 0L;
      d_btrd = 0L;
      d_size = 0L;
      p_data = nullptr;
    }
    clear ();
  }
  
  // copy construct this mixmap

  Mixmap::Mixmap (const Mixmap& that) {
    that.rdlock ();
    try {
      // copy the base tranche
      Tranche::operator = (that);
      // copy locally
      d_rtrd = that.d_rtrd;
      d_btrd = that.d_btrd;
      d_size = that.d_size;
      p_data = nullptr;
      if ((d_size > 0L) && (that.p_data != nullptr)) {
	p_data = new t_byte[d_size];
	Utility::tobcpy(p_data, d_size, that.p_data);
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }  
  }

  // move construct this mixmap

  Mixmap::Mixmap (Mixmap&& that) noexcept {
    that.wrlock ();
    try {
      // copy move the base tranche
      Tranche::operator = (static_cast<Tranche&&>(that));
      // copu move locally
      d_rtrd = that.d_rtrd; that.d_rtrd = 0L;
      d_btrd = that.d_btrd; that.d_btrd = 0L;
      d_size = that.d_size; that.d_size = 0L;
      p_data = that.p_data; that.p_data = nullptr;
      that.unlock ();
    } catch (...) {
      that.unlock ();
    }
  }

  // destroy this mixmap

  Mixmap::~Mixmap (void) {
    delete [] p_data;
  }
  
  // assign an mixmap to this one

  Mixmap& Mixmap::operator = (const Mixmap& that) {
    // check for self-move
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign the base tranche
      Tranche::operator = (that);
      // assign locally
      d_rtrd = that.d_rtrd;
      d_btrd = that.d_btrd;
      d_size = that.d_size;
      p_data = nullptr;
      if ((d_size > 0L) && (that.p_data != nullptr)) {
	p_data = new t_byte[d_size];
	Utility::tobcpy(p_data, d_size, that.p_data);
      }
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // move an mixmap to this one

  Mixmap& Mixmap::operator = (Mixmap&& that) noexcept {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // move the base tranche
      Tranche::operator = (static_cast<Tranche&&>(that));
      // move locally
      d_rtrd = that.d_rtrd; that.d_rtrd = 0L;
      d_btrd = that.d_btrd; that.d_btrd = 0L;
      d_size = that.d_size; that.d_size = 0L;
      p_data = that.p_data; that.p_data = nullptr;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      return *this;
    }
  }

  // return the class name

  String Mixmap::repr (void) const {
    return "Mixmap";
  }

  // return a clone of this object

  Object* Mixmap::clone (void) const {
    return new Mixmap (*this);
  }

  // return the serial did
  
  t_word Mixmap::getdid (void) const {
    return SRL_DEOD_DIP;
  }
  
  // return the serial sid
  
  t_word Mixmap::getsid (void) const {
    return SRL_MIXM_SID;
  }
  
  // serialize this mixmap

  void Mixmap::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the tranche
      Tranche::wrstream (os);
      // serialize locally
      Serial::wrlong (d_rtrd, os);
      Serial::wrlong (d_btrd, os);
      Serial::wrlong (d_size, os);
      if (p_data == nullptr) {
	Serial::wrnilid (os);
      } else {
	os.write (reinterpret_cast<const char*> (p_data), d_size);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // deserialize this mixmap

  void Mixmap::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the tranche
      Tranche::rdstream (is);
      // deserialize locally
      d_rtrd = Serial::rdlong (is);
      d_btrd = Serial::rdlong (is);
      d_size = Serial::rdlong (is);
      p_data = nullptr;
      if (d_size != 0L) {
	p_data = new t_byte[d_size];
	is.copy (reinterpret_cast<char*>(p_data), d_size);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // reset this mixmap

  void Mixmap::reset (void) {
    wrlock ();
    try {
      // reset the base tranche
      Tranche::reset ();
      // reset locally
      d_rtrd = 0L;
      d_btrd = 0L;
      d_size = 0L;
      delete [] p_data; p_data = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this mixmap

  void Mixmap::clear (void) {
    wrlock ();
    try {
      Utility::tonull (p_data, d_size);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // set the mixmap by position and pixel

  void Mixmap::setpixl (const long x, const long y, const long d,
			const Pixel& pixl) {
    wrlock ();
    try {
      // check valid position
      if ((x < 0L) || (x >= d_wdth) || (y < 0L) || (y >= d_hght) ||
	  (d < 0L) || (d >= d_dpth)) {
	throw Exception ("mixmap-error", "invalid mixmap position in set");
      }
      // check for valid format
      if (d_pfmt == Pixel::PFMT_NONE) d_pfmt = pixl.getpfmt ();
      if (d_pfmt != pixl.getpfmt ()) {
	throw Exception ("mixmap-error", "inconsistent format in set");
      }
      // get the pixel component
      Pixel::t_pixl pc = pixl.getpixl ();
      // get the depth offset and format
      long doff = 0L;
      Pixel::t_pfmt pfmt = d_pfmt;
      if (d > 0L) {
	doff = d_rtrd * d_hght + d_btrd * d_hght * (d - 1L);
	pfmt = mixm_to_bfmt (pfmt);
      }
      // set by format
      switch (pfmt) {
      case Pixel::PFMT_BYTE:
	{
	  // compute the byte position
	  long bpos = doff + y * d_btrd + x;
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pp->d_b[0] = pc.d_b[0];
	  pp->d_b[1] = pc.d_b[1];
	  pp->d_b[2] = pc.d_b[2];
	  pp->d_b[3] = pc.d_b[3];
	}
	break;
      case Pixel::PFMT_WORD:
	{
	  // compute the byte position
	  long bpos = doff + y * d_btrd + (x * 2);
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pp->d_w[0] = pc.d_w[0];
	  pp->d_w[1] = pc.d_w[1];
	  pp->d_w[2] = pc.d_w[2];
	  pp->d_w[3] = pc.d_w[3];
	}
	break;
      case Pixel::PFMT_REAL:
	{
	  // compute the byte position
	  long bpos = doff + y * d_btrd + (x * sizeof(t_real));
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pp->d_r[0] = pc.d_r[0];
	  pp->d_r[1] = pc.d_r[1];
	  pp->d_r[2] = pc.d_r[2];
	  pp->d_r[3] = pc.d_r[3];
	}
	break;
      case Pixel::PFMT_FLOT:
	{
	  // compute the byte position
	  long bpos = doff + y * d_btrd + (x * sizeof(float));
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pp->d_f[0] = pc.d_f[0];
	  pp->d_f[1] = pc.d_f[1];
	  pp->d_f[2] = pc.d_f[2];
	  pp->d_f[3] = pc.d_f[3];
	}
	break;
      case Pixel::PFMT_RGBA:
      case Pixel::PFMT_BGRA:
	{
	  // check offset
	  if (doff != 0L) {
	    throw Exception ("mixmap-error", "invalid offset in setpixl");
	  }
	  // compute the byte position
	  long bpos = y * d_rtrd + (x * 4);
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pp->d_b[0] = pc.d_b[0];
	  pp->d_b[1] = pc.d_b[1];
	  pp->d_b[2] = pc.d_b[2];
	  pp->d_b[3] = pc.d_b[3];
	}
	break;
      case Pixel::PFMT_RGBO:
      case Pixel::PFMT_BGRO:
	{
	  // check offset
	  if (doff != 0L) {
	    throw Exception ("mixmap-error", "invalid offset in setpixl");
	  }
	  // compute the byte position
	  long bpos = y * d_rtrd + (x * 8);
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pp->d_w[0] = pc.d_w[0];
	  pp->d_w[1] = pc.d_w[1];
	  pp->d_w[2] = pc.d_w[2];
	  pp->d_w[3] = pc.d_w[3];
	}
	break;
      case Pixel::PFMT_RGBR:
	{
	  // check offset
	  if (doff != 0L) {
	    throw Exception ("mixmap-error", "invalid offset in setpixl");
	  }
	  // compute the byte position
	  long bpos = y * d_rtrd + (x * sizeof(t_real) * 4);
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pp->d_r[0] = pc.d_r[0];
	  pp->d_r[1] = pc.d_r[1];
	  pp->d_r[2] = pc.d_r[2];
	  pp->d_r[3] = pc.d_r[3];
	}
	break;
      case Pixel::PFMT_RGBF:
	{
	  // check offset
	  if (doff != 0L) {
	    throw Exception ("mixmap-error", "invalid offset in setpixl");
	  }
	  // compute the byte position
	  long bpos = y * d_rtrd + (x * sizeof(float) * 4);
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pp->d_f[0] = pc.d_f[0];
	  pp->d_f[1] = pc.d_f[1];
	  pp->d_f[2] = pc.d_f[2];
	  pp->d_f[3] = pc.d_f[3];
	}
	break;
      default:
	break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a pixel by position

  Pixel Mixmap::getpixl (const long x, const long y, const long d) const {
    rdlock ();
    try {
      // check valid position
      if ((x < 0L) || (x >= d_wdth) || (y < 0L) || (y >= d_hght) ||
	  (d < 0L) || (d >= d_dpth)) {
	throw Exception ("mixmap-error", "invalid pixel position");
      }
      // preset pixel result
      Pixel::t_pixl pixl;
      Utility::tonull (&pixl, sizeof (pixl));
      // get the depth offset and format
      long doff = 0L;
      Pixel::t_pfmt pfmt = d_pfmt;
      if (d > 0L) {
	doff = d_rtrd * d_hght + d_btrd * d_hght * (d - 1L);
	pfmt = mixm_to_bfmt (pfmt);
      }
      // get by format
      switch (pfmt) {
      case Pixel::PFMT_BYTE:
	{
	  // compute the byte position
	  long bpos = doff + y * d_btrd + x;
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pixl.d_b[0] = pp->d_b[0];
	  pixl.d_b[1] = pp->d_b[1];
	  pixl.d_b[2] = pp->d_b[2];
	  pixl.d_b[3] = pp->d_b[3];
	}
	break;
      case Pixel::PFMT_WORD:
	{
	  // compute the byte position
	  long bpos = doff + y * d_btrd + (x * 2);
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pixl.d_w[0] = pp->d_w[0];
	  pixl.d_w[1] = pp->d_w[1];
	  pixl.d_w[2] = pp->d_w[2];
	  pixl.d_w[3] = pp->d_w[3];
	}
	break;
      case Pixel::PFMT_REAL:
	{
	  // compute the byte position
	  long bpos = doff + y * d_btrd + (x * sizeof(t_real));
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pixl.d_r[0] = pp->d_r[0];
	  pixl.d_r[1] = pp->d_r[1];
	  pixl.d_r[2] = pp->d_r[2];
	  pixl.d_r[3] = pp->d_r[3];
	}
	break;
      case Pixel::PFMT_FLOT:
	{
	  // compute the byte position
	  long bpos = doff + y * d_btrd + (x * sizeof(float));
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pixl.d_f[0] = pp->d_f[0];
	  pixl.d_f[1] = pp->d_f[1];
	  pixl.d_f[2] = pp->d_f[2];
	  pixl.d_f[3] = pp->d_f[3];
	}
	break;
      case Pixel::PFMT_RGBA:
      case Pixel::PFMT_BGRA:
	{
	  // check offset
	  if (doff != 0L) {
	    throw Exception ("mixmap-error", "invalid offset in getpixl");
	  }
	  // compute the byte position
	  long bpos = y * d_rtrd + (x * 4);
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pixl.d_b[0] = pp->d_b[0];
	  pixl.d_b[1] = pp->d_b[1];
	  pixl.d_b[2] = pp->d_b[2];
	  pixl.d_b[3] = pp->d_b[3];
	}
	break;
      case Pixel::PFMT_RGBO:
      case Pixel::PFMT_BGRO:
	{
	  // check offset
	  if (doff != 0L) {
	    throw Exception ("mixmap-error", "invalid offset in getpixl");
	  }
	  // compute the byte position
	  long bpos = y * d_rtrd + (x * 8);
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pixl.d_w[0] = pp->d_w[0];
	  pixl.d_w[1] = pp->d_w[1];
	  pixl.d_w[2] = pp->d_w[2];
	  pixl.d_w[3] = pp->d_w[3];
	}
	break;
      case Pixel::PFMT_RGBR:
	{
	  // check offset
	  if (doff != 0L) {
	    throw Exception ("mixmap-error", "invalid offset in getpixl");
	  }
	  // compute the byte position
	  long bpos = y * d_rtrd + (x * sizeof(t_real) * 4);
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pixl.d_r[0] = pp->d_r[0];
	  pixl.d_r[1] = pp->d_r[1];
	  pixl.d_r[2] = pp->d_r[2];
	  pixl.d_r[3] = pp->d_r[3];
	}
	break;
      case Pixel::PFMT_RGBF:
	{
	  // check offset
	  if (doff != 0L) {
	    throw Exception ("mixmap-error", "invalid offset in getpixl");
	  }
	  // compute the byte position
	  long bpos = y * d_rtrd + (x * sizeof(float) * 4);
	  // get pixel equivalent position
	  auto pp = reinterpret_cast<Pixel::t_pixl*>(&p_data[bpos]);
	  pixl.d_f[0] = pp->d_f[0];
	  pixl.d_f[1] = pp->d_f[1];
	  pixl.d_f[2] = pp->d_f[2];
	  pixl.d_f[3] = pp->d_f[3];
	}
	break;
      default:
	break;
      }
      // create the pixel result
      Pixel result (d_pfmt, pixl);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the mixmap byte size

  long Mixmap::tosize (void) const {
    rdlock ();
    try {
      long result = d_size;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the mixmap byte data

  t_byte* Mixmap::tobyte (void) {
    rdlock ();
    try {
      t_byte* result = p_data;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the mixmap byte data

  const t_byte* Mixmap::tobyte (void) const {
    rdlock ();
    try {
      t_byte* result = p_data;
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

  // create a new object in a generic way
  
  Object* Mixmap::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Mixmap;
    // check for 4 arguments
    if (argc == 4) {
      Object* obj = argv->get(0);
      auto item = dynamic_cast<Item*>(obj);
      if (item == nullptr) {
	throw Exception ("type-error", "invalid object as item",
			 Object::repr (obj));
      }
      Pixel::t_pfmt pfmt = item_to_pfmt (*item);
      long wdth = argv->getlong (1);
      long hght = argv->getlong (2);
      long dpth = argv->getlong (3);
      return new Mixmap (pfmt, wdth, hght, dpth);
    }
    // invalid arguments
    throw Exception ("argument-error", 
                     "invalid arguments with with mixmap constructor"); 
  }
}
