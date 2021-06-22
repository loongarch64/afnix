// ---------------------------------------------------------------------------
// - Pixmap.cpp                                                              -
// - afnix:dip service - pixmap class implementation                         -
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
#include "Pixmap.hpp"
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

  // this procedure compute the pixmap stride by format
  static inline long pixm_to_strd (const Pixel::t_pfmt pfmt, const long wdth) {
    // check for valid argument
    if ((wdth <= 0L) || (pfmt == Pixel::PFMT_NONE)) return 0L;
    // compute stride by format
    long result = 0L;
    switch (pfmt) {
    case Pixel::PFMT_BYTE:
      result = wdth;
      break;
    case Pixel::PFMT_WORD:
      result = wdth * 2;
      break;
    case Pixel::PFMT_REAL:
      result = wdth * sizeof (t_real);
      break;
    case Pixel::PFMT_FLOT:
      result = wdth * sizeof (float);
      break;
    case Pixel::PFMT_RGBA:
    case Pixel::PFMT_BGRA:
      result = wdth * 4;
      break;
    case Pixel::PFMT_RGBO:
    case Pixel::PFMT_BGRO:
      result = wdth * 8;
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

  // this procedure compute the pixmap byte size by format and geometry
  static inline long pixm_to_size (const long strd, const long hght) {
    // check for valid argument;
    if ((strd <= 0L) || (hght <= 0L)) return 0L;
    // compute result
    long result = strd * hght;
    return result;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default pixmap

  Pixmap::Pixmap (void) {
    d_pfmt = Pixel::PFMT_NONE;
    p_data = nullptr;
    reset ();
  }

  // create a pixmap by width and height

  Pixmap::Pixmap (const Pixel::t_pfmt pfmt, const long wdth, const long hght) {
    d_pfmt = pfmt;
    d_strd = pixm_to_strd (d_pfmt, wdth);
    d_size = pixm_to_size (d_strd, hght);
    if (d_size > 0L) {
      d_wdth = wdth;
      d_hght = hght;
      p_data = new t_byte[d_size];
    } else {
      d_pfmt = Pixel::PFMT_NONE;
      d_strd = 0L;
      d_size = 0L;
      p_data = nullptr;
    }
    clear ();
  }
  
  // copy construct this pixmap

  Pixmap::Pixmap (const Pixmap& that) {
    that.rdlock ();
    try {
      // copy the base slice
      Slice::operator = (that);
      // copy locally
      d_strd = that.d_strd;
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

  // move construct this pixmap

  Pixmap::Pixmap (Pixmap&& that) noexcept {
    that.wrlock ();
    try {
      // copy move the base slice
      Slice::operator = (static_cast<Slice&&>(that));
      // copu move locally
      d_strd = that.d_strd; that.d_strd = 0L;
      d_size = that.d_size; that.d_size = 0L;
      p_data = that.p_data; that.p_data = nullptr;
      that.unlock ();
    } catch (...) {
      that.unlock ();
    }
  }

  // destroy this pixmap

  Pixmap::~Pixmap (void) {
    delete [] p_data;
  }
  
  // assign an pixmap to this one

  Pixmap& Pixmap::operator = (const Pixmap& that) {
    // check for self-move
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign the base slice
      Slice::operator = (that);
      // assign locally
      d_strd = that.d_strd;
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

  // move an pixmap to this one

  Pixmap& Pixmap::operator = (Pixmap&& that) noexcept {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // move the base slice
      Slice::operator = (static_cast<Slice&&>(that));
      // move locally
      d_strd = that.d_strd; that.d_strd = 0L;
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

  String Pixmap::repr (void) const {
    return "Pixmap";
  }

  // return a clone of this object

  Object* Pixmap::clone (void) const {
    return new Pixmap (*this);
  }
  
  // return the serial did
  
  t_word Pixmap::getdid (void) const {
    return SRL_DEOD_DIP;
  }

  // return the serial sid
  
  t_word Pixmap::getsid (void) const {
    return SRL_PIXM_SID;
  }

  // serialize this pixmap

  void Pixmap::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the slice
      Slice::wrstream (os);
      // serialize locally
      Serial::wrlong (d_strd, os);
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
  
  // deserialize this pixmap

  void Pixmap::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the slice
      Slice::rdstream (is);
      // deserialize locally
      d_strd = Serial::rdlong (is);
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
  
  // reset this pixmap

  void Pixmap::reset (void) {
    wrlock ();
    try {
      // reset the base slice
      Slice::reset ();
      // reset locally
      d_strd = 0L;
      d_size = 0L;
      delete [] p_data; p_data = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this pixmap

  void Pixmap::clear (void) {
    wrlock ();
    try {
      Utility::tonull (p_data, d_size);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a pixmap by buffer and format

  void Pixmap::setpixl (const Buffer& pbuf, const Pixel::t_pfmt pfmt) {
    wrlock ();
    try {
      // check for nil data
      if ((pbuf.empty () == true) || (pfmt == Pixel::PFMT_NONE)) {
	unlock ();
	return;
      }
      // compute image size by format
      long strd = pixm_to_strd (pfmt, d_wdth);
      long size = strd * d_hght;
      if (pbuf.tosize () != size) {
	throw Exception ("pixmap-error", "invalid buffer size in setpixl");
      }
      // clear locally
      delete [] p_data; p_data = new t_byte[size];
      // copy and update
      Utility::tobcpy (p_data, size, pbuf.tobyte ());
      d_pfmt = pfmt;
      d_size = size;
      d_strd = strd;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the pixmap by position and pixel

  void Pixmap::setpixl (const long x, const long y, const Pixel& pixl) {
    wrlock ();
    try {
      // check valid position
      if ((x < 0L) || (x >= d_wdth) || (y < 0L) || (y >= d_hght)) {
	throw Exception ("pixmap-error", "invalid pixmap position in set");
      }
      // check for valid format
      if (d_pfmt == Pixel::PFMT_NONE) d_pfmt = pixl.getpfmt ();
      if (d_pfmt != pixl.getpfmt ()) {
	throw Exception ("pixmap-error", "inconsistent format in set");
      }
      // get the pixel component
      Pixel::t_pixl pc = pixl.getpixl ();
      // set by format
      switch (d_pfmt) {
      case Pixel::PFMT_BYTE:
	{
	  // compute the byte position
	  long bpos = y * d_strd + x;
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
	  long bpos = y * d_strd + (x * 2);
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
	  long bpos = y * d_strd + (x * sizeof(t_real));
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
	  long bpos = y * d_strd + (x * sizeof(float));
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
	  // compute the byte position
	  long bpos = y * d_strd + (x * 4);
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
	  // compute the byte position
	  long bpos = y * d_strd + (x * 8);
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
	  // compute the byte position
	  long bpos = y * d_strd + (x * sizeof(t_real) * 4);
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
	  // compute the byte position
	  long bpos = y * d_strd + (x * sizeof(float) * 4);
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

  Pixel Pixmap::getpixl (const long x, const long y) const {
    rdlock ();
    try {
      // check valid position
      if ((x < 0L) || (x >= d_wdth) || (y < 0L) || (y >= d_hght)) {
	throw Exception ("pixmap-error", "invalid pixel position");
      }
      // preset pixel result
      Pixel::t_pixl pixl;
      Utility::tonull (&pixl, sizeof (pixl));
      // get by format
      switch (d_pfmt) {
      case Pixel::PFMT_BYTE:
	{
	  // compute the byte position
	  long bpos = y * d_strd + x;
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
	  long bpos = y * d_strd + (x * 2);
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
	  long bpos = y * d_strd + (x * sizeof(t_real));
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
	  long bpos = y * d_strd + (x * sizeof(float));
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
	  // compute the byte position
	  long bpos = y * d_strd + (x * 4);
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
	  // compute the byte position
	  long bpos = y * d_strd + (x * 8);
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
	  // compute the byte position
	  long bpos = y * d_strd + (x * sizeof(t_real) * 4);
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
	  // compute the byte position
	  long bpos = y * d_strd + (x * sizeof(float) * 4);
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

  // convert a pixmap to another format

  Slice* Pixmap::convert (const Pixel::t_pfmt pfmt) const {
    rdlock ();
    Pixmap* result = nullptr;
    try {
      // create the result pixmap
      result = new Pixmap (pfmt, d_wdth, d_hght);
      for (long y = 0L; y < d_hght; y++) {
	for (long x = 0L; x < d_wdth; x++) {
	  // get the initial pixel
	  Pixel pixl = getpixl (x, y);
	  // convert it to the new format
	  Pixel pixt = pixl.convert (pfmt);
	  // set the result pixel
	  result->setpixl (x, y, pixt);
	}
      }
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }
  
  // get the pixmap byte size

  long Pixmap::tosize (void) const {
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

  // get the pixmap byte data

  t_byte* Pixmap::tobyte (void) {
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

  // get the pixmap byte data

  const t_byte* Pixmap::tobyte (void) const {
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
  
  Object* Pixmap::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Pixmap;
    // check for 3 arguments
    if (argc == 3) {
      Object* obj = argv->get(0);
      auto item = dynamic_cast<Item*>(obj);
      if (item == nullptr) {
	throw Exception ("type-error", "invalid object as item",
			 Object::repr (obj));
      }
      Pixel::t_pfmt pfmt = item_to_pfmt (*item);
      long wdth = argv->getlong (1);
      long hght = argv->getlong (2);
      return new Pixmap (pfmt, wdth, hght);
    }
    // invalid arguments
    throw Exception ("argument-error", 
                     "invalid arguments with with pixmap constructor"); 
  }
}
