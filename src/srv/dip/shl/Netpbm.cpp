// ---------------------------------------------------------------------------
// - Netpbm.cpp                                                              -
// - afnix:dip service - netpbm class implementation                         -
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

#include "Vector.hpp"
#include "Netpbm.hpp"
#include "Pixmap.hpp"
#include "System.hpp"
#include "Utility.hpp"
#include "TcpClient.hpp"
#include "UriStream.hpp"
#include "Exception.hpp"

namespace afnix {

  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure removes the comments if any
  static void pnm_rmeol (InputStream* is) {
    // check for nil
    if (is == nullptr) return;
    // remove until end of line
    while (is->valid () == true) {
      if (is->read () == '\n') break;
    } 
  }

  // this procedure reads a token
  static String pnm_token (InputStream* is) {
    // prepare for accumulation
    String token;
    // check for nil
    if (is == nullptr) return token;
    // loop until separator
    while (is->valid () == true) {
      // read next character
      char c = is->read ();
      // check for comment
      if (c == '#') {
	pnm_rmeol (is);
	continue;
      }
      // discard separator
      if (c == '\r') continue;
      if ((c == ' ') || (c == '\n')) return token;
      // accumulate
      token += c;
    }
    return token;
  }

  // get the ppm format by maximum value
  static inline Pixel::t_pfmt pnm_ppm_topfmt (const long maxv) {
    // check for byte range
    if ((maxv > 0L)   && (maxv < 256))   return Pixel::PFMT_RGBA;
    // check for word range
    if ((maxv > 255L) && (maxv < 65536)) return Pixel::PFMT_RGBO;
    // unsupported image format
    throw Exception ("netpbm-error", "unsupported image format");
  }

  // get the pgm format by maximum value
  static inline Pixel::t_pfmt pnm_pgm_topfmt (const long maxv) {
    // check for byte range
    if ((maxv > 0L)   && (maxv < 256))   return Pixel::PFMT_BYTE;
    // check for word range
    if ((maxv > 255L) && (maxv < 65536)) return Pixel::PFMT_WORD;
    // unsupported image format
    throw Exception ("netpbm-error", "unsupported image format");
  }

  // get the component byte size by maximum value
  static inline long pnm_max_tocbsz (const long maxv) {
    // check for byte range
    if ((maxv > 0L)   && (maxv < 256))   return 1L;
    // check for word range
    if ((maxv > 255L) && (maxv < 65536)) return 2L;
    // unsupported component format
    throw Exception ("netpbm-error", "unsupported image component size");
  }
  
  // get a pixel byte value by byte size
  static Pixel pnm_pgm_topixl (InputStream* is, const long cbsz) {
    Pixel::t_pixl pixl;
    if (cbsz == 1L) {
      pixl.d_b[0] = Utility::tolong (pnm_token (is));
      pixl.d_b[1] = pixl.d_b[0];
      pixl.d_b[2] = pixl.d_b[0];
      pixl.d_b[3] = 0xFFU;
      return Pixel(Pixel::PFMT_BYTE, pixl);
    }
    if (cbsz == 2L) {
      pixl.d_w[0]  = Utility::tolong (pnm_token (is)); pixl.d_w[0] <<= 8;
      pixl.d_w[0] |= Utility::tolong (pnm_token (is));
      pixl.d_w[1]  = pixl.d_w[0];
      pixl.d_w[2]  = pixl.d_w[0];
      pixl.d_w[3]  = 0xFFFFU;
      return Pixel(Pixel::PFMT_WORD, pixl);
    }
    throw Exception("netpbm-error", "invalid pixel size to read");
  }
  
  // get a pixel byte value by byte size
  static Pixel pnm_ppm_topixl (InputStream* is, const long cbsz) {
    Pixel::t_pixl pixl;
    if (cbsz == 1L) {
      pixl.d_b[0] = Utility::tolong (pnm_token (is));
      pixl.d_b[1] = Utility::tolong (pnm_token (is));
      pixl.d_b[2] = Utility::tolong (pnm_token (is));
      pixl.d_b[3] = 0xFFU;
      return Pixel(Pixel::PFMT_RGBA, pixl);
    }
    if (cbsz == 2L) {
      pixl.d_w[0]  = Utility::tolong (pnm_token (is)); pixl.d_w[0] <<= 8;
      pixl.d_w[0] |= Utility::tolong (pnm_token (is));
      pixl.d_w[1]  = Utility::tolong (pnm_token (is)); pixl.d_w[1] <<= 8;
      pixl.d_w[1] |= Utility::tolong (pnm_token (is));
      pixl.d_w[2]  = Utility::tolong (pnm_token (is)); pixl.d_w[2] <<= 8;
      pixl.d_w[2] |= Utility::tolong (pnm_token (is));
      pixl.d_w[3]  = 0xFFFFU;
      return Pixel(Pixel::PFMT_RGBO, pixl);
    }
    throw Exception("netpbm-error", "invalid pixel size to read");
  }
  
  // read a pbm from an input stream
  static Image* pnm_pbm_img (InputStream* is, const bool bflg) {
    throw Exception ("netpbm-error", "unimplemented pbm reader");
  }

  // read a pgm from an input stream
  static Image* pnm_pgm_img (InputStream* is, const bool bflg) {
     // check for nil
    if (is == nullptr) return nullptr;
    // get the width and height
    long wdth = Utility::tolong (pnm_token (is));
    long hght = Utility::tolong (pnm_token (is));
    // get the maximum value / pixel format
    long maxv = Utility::tolong (pnm_token (is));
    long cbsz = pnm_max_tocbsz (maxv);
    auto pfmt = pnm_pgm_topfmt (maxv);
    if (pfmt != Pixel::PFMT_BYTE) return nullptr;
    // check for valid image
    if ((wdth <= 0L) || (hght <= 0L)) return nullptr;
    // create a target pixmap as an image
    Pixmap* pixm = new Pixmap (pfmt, wdth, hght);
    try {
      // parse in text mode
      if (bflg == false) {
	long row = 0L; long col = 0L;
	while (is->valid () == true) {
	  // read the component values
	  Pixel pixl = pnm_pgm_topixl(is, cbsz);
	  // set the pixmap component value
	  pixm->setpixl (col, row, pixl);
	  // move to the next column
	  if (++col >= wdth) {
	    col = 0L;
	    row++;
	  }
	  if (row >= hght) break;
	}
      } else {
	// compute the image planar size in byte
	long size = wdth * hght * cbsz;
	// allocate and read the image block
	char* blok = new char[size];
	if (is->copy (blok, size) != size) {
	  delete pixm;
	  throw Exception ("netpbm-error", "cannot read pixmap data");
	}
	// map the color components
	long bidx = 0L; Pixel::t_pixl pixl;
	for (long row = 0L; row < hght; row++) {
	  for (long col = 0L; col < wdth; col++) {
	    if (cbsz == 1L) {
	      pixl.d_b[0] = blok[bidx++];
	      pixl.d_b[1] = pixl.d_b[0];
	      pixl.d_b[2] = pixl.d_b[0];
	      pixl.d_b[3] = 0xFFU;
	      pixm->setpixl (col, row, Pixel (Pixel::PFMT_BYTE, pixl));
	    } else if (cbsz == 2L) {
	      t_word w = blok[bidx++]; w <<= 8; w |= blok[bidx++];
	      pixl.d_w[0] = System::wswap(w);
	      pixl.d_w[1] = pixl.d_w[0];
	      pixl.d_w[2] = pixl.d_w[0];
	      pixl.d_w[3] = 0xFFFFU;
	      pixm->setpixl (col, row, Pixel (Pixel::PFMT_WORD, pixl));
	    } else {
	      throw Exception ("netpbm-error", "invalid pixel size");
	    }
	  }
	}
	delete [] blok;
      }
      return pixm;
    } catch (...) {
      delete pixm;
      throw;
    }
  }
  
  // read a ppm from an input stream
  static Image* pnm_ppm_img (InputStream* is, const bool bflg) {
    // check for nil
    if (is == nullptr) return nullptr;
    // get the width and height
    long wdth = Utility::tolong (pnm_token (is));
    long hght = Utility::tolong (pnm_token (is));
    // get the maximum value / pixel format
    long maxv = Utility::tolong (pnm_token (is));
    long cbsz = pnm_max_tocbsz (maxv);
    auto pfmt = pnm_ppm_topfmt (maxv);
    // check for valid image
    if ((wdth <= 0L) || (hght <= 0L)) return nullptr;
    // create a target pixmap as an image
    Pixmap* pixm = new Pixmap (pfmt, wdth, hght);
    try {
      // parse in text mode
      if (bflg == false) {
	long row = 0L; long col = 0L;
	while (is->valid () == true) {
	  // read the component values
	  Pixel pixl = pnm_ppm_topixl(is, cbsz);
	  // set the pixmap component value
	  pixm->setpixl (col, row, pixl);
	  // move to the next column
	  if (++col >= wdth) {
	    col = 0L;
	    row++;
	  }
	  if (row >= hght) break;
	}
      } else {
	// compute the image planar size in byte
	long size = wdth * hght * 3 * cbsz;
	// allocate and read the image block
	char* blok = new char[size];
	if (is->copy (blok, size) != size) {
	  delete pixm;
	  throw Exception ("netpbm-error", "cannot read pixmap data");
	}
	// map the color components
	long bidx = 0L; 
	for (long row = 0L; row < hght; row++) {
	  for (long col = 0L; col < wdth; col++) {
	    Pixel::t_pixl pixl;
	    if (cbsz == 1L) {
	      for (long k = 0L; k < 3L; k++) pixl.d_b[k] = blok[bidx++];
	      pixl.d_b[3] = 0xFFU;
	    } else if (cbsz == 2L) {
	      for (long k = 0L; k < 3L; k++) {
		t_word w = blok[bidx++]; w <<= 8; w |= blok[bidx++];
		pixl.d_w[k] = System::wswap(w);
	      }
	      pixl.d_w[3] = 0xFFFFU;
	    } else {
	      throw Exception ("netpbm-error", "invalid pixel size");
	    }
	    // set the pixmap component value
	    pixm->setpixl (col, row, Pixel(pfmt, pixl));
	  }
	}
	delete [] blok;
      }
      return pixm;
    } catch (...) {
      delete pixm;
      throw;
    }
  }
  
  // read an input stream to an image
  static Image* pnm_xxx_img (InputStream* is) {
    // check for nil
    if (is == nullptr) return nullptr;
    // extract file format
    String fmt = pnm_token (is);
    if (fmt.isnil () == true) return nullptr;
    // dispatch file format
    if (fmt.toupper () == "P1") return pnm_pbm_img (is, false);
    if (fmt.toupper () == "P4") return pnm_pbm_img (is, true);
    if (fmt.toupper () == "P2") return pnm_pgm_img (is, false);
    if (fmt.toupper () == "P5") return pnm_pgm_img (is, true);
    if (fmt.toupper () == "P3") return pnm_ppm_img (is, false);
    if (fmt.toupper () == "P6") return pnm_ppm_img (is, true);
    // invalid format
    throw Exception ("netpbm-error", "invalid pnm file format", fmt);
  }

  // write a text pgm slice to an output stream
  static bool slc_pgm_txt (OutputStream& os, const Slice& slc) {
    // collect width and height
    long wdth = slc.getwdth ();
    long hght = slc.gethght ();
    if ((wdth <= 0L) || (hght <= 0L)) return false;
    // write the header
    os << "P2" << eolc;
    os << wdth << ' ' << hght << eolc;
    os << "255" << eolc;
    // loop inside the slice
    for (long y = 0L; y < hght; y++) {
      for (long x = 0L; x < wdth; x++) {
	// get the source pixel
	Pixel spix = slc.getpixl (x, y);
	// convert to a rgba pixel
	Pixel tpix = spix.convert (Pixel::PFMT_RGBA);
	// get the pixel value
	Pixel::t_pixl pixl = tpix.getpixl ();
	// get the pixel value
	long grey = (long) pixl.d_b[0];
	// write the pixel
	os << grey << eolc;
      }
    }
    return true;
  }
  
  // write a text ppm slice to an output stream
  static bool slc_ppm_txt (OutputStream& os, const Slice& slc) {
    // collect width and height
    long wdth = slc.getwdth ();
    long hght = slc.gethght ();
    if ((wdth <= 0L) || (hght <= 0L)) return false;
    // write the header
    os << "P3" << eolc;
    os << wdth << ' ' << hght << eolc;
    os << "255" << eolc;
    // loop inside the slice
    for (long y = 0L; y < hght; y++) {
      for (long x = 0L; x < wdth; x++) {
	// get the source pixel
	Pixel spix = slc.getpixl (x, y);
	// convert to a rgba pixel
	Pixel tpix = spix.convert (Pixel::PFMT_RGBA);
	// get the pixel value
	Pixel::t_pixl pixl = tpix.getpixl ();
	// prepare the write block
	long rgb[3];
	rgb[0] = (long) pixl.d_b[0];
	rgb[1] = (long) pixl.d_b[1];
	rgb[2] = (long) pixl.d_b[2];
	// write the block
	os << rgb[0] << ' ' << rgb[1] << ' ' << rgb[2] << eolc;
      }
    }
    return true;
  }

  // write a raw pgm slice to an output stream
  static bool slc_pgm_raw (OutputStream& os, const Slice& slc) {
    // collect width and height
    long wdth = slc.getwdth ();
    long hght = slc.gethght ();
    long size = wdth * hght;
    if ((wdth <= 0L) || (hght <= 0L)) return false;
    // write the header
    os << "P5" << eolc;
    os << wdth << ' ' << hght << eolc;
    os << "255" << eolc;
    // allocate a temporay conversion buffer
    char* data = new char[size];
    long  bidx = 0L;
    // loop inside the slice for pixel conversion
    for (long y = 0L; y < hght; y++) {
      for (long x = 0L; x < wdth; x++) {
	// get the source pixel
	Pixel spix = slc.getpixl (x, y);
	// convert to a byte pixel
	Pixel tpix = spix.convert (Pixel::PFMT_BYTE);
	// get the pixel value
	Pixel::t_pixl pixl = tpix.getpixl ();
	// set the block value
	data[bidx++] = pixl.d_b[0];
      }
    }
    // write the data buffer
    bool result = (os.write(data, size) == size) ? true : false;
    delete [] data;
    return result;
  }
  
  // write a raw ppm slice to an output stream
  static bool slc_ppm_raw (OutputStream& os, const Slice& slc) {
    // collect width and height
    long wdth = slc.getwdth ();
    long hght = slc.gethght ();
    long size = wdth * hght * 3;
    if ((wdth <= 0L) || (hght <= 0L)) return false;
    // write the header
    os << "P6" << eolc;
    os << wdth << ' ' << hght << eolc;
    os << "255" << eolc;
    // allocate a temporay conversion buffer
    char* data = new char[size];
    long  bidx = 0L;
    // loop inside the slice
    for (long y = 0L; y < hght; y++) {
      for (long x = 0L; x < wdth; x++) {
	// get the source pixel
	Pixel spix = slc.getpixl (x, y);
	// convert to a rgba pixel
	Pixel tpix = spix.convert (Pixel::PFMT_RGBA);
	// get the pixel value
	Pixel::t_pixl pixl = tpix.getpixl ();
	// set the blobk value
	data[bidx++] = pixl.d_b[0];
	data[bidx++] = pixl.d_b[1];
	data[bidx++] = pixl.d_b[2];
      }
    }
    // write the data buffer
    bool result = (os.write(data, size) == size) ? true : false;
    delete [] data;
    return result;
  }
  
  // write a raw slice to an output stream
  static bool slc_xxx_raw (OutputStream& os, const Slice& slc) {
    // get pixel format
    Pixel::t_pfmt pfmt = slc.getpfmt ();
    if (pfmt == Pixel::PFMT_NONE) return false;
    // select between pgm and ppm
    bool ppm = false;
    switch (pfmt) {
    case Pixel::PFMT_BYTE:
    case Pixel::PFMT_WORD:
    case Pixel::PFMT_REAL:
    case Pixel::PFMT_FLOT:
      ppm = false;
      break;
    case Pixel::PFMT_RGBA:
    case Pixel::PFMT_BGRA:
    case Pixel::PFMT_RGBR:
    case Pixel::PFMT_RGBF:
      ppm = true;
      break;
    default:
      return false;
      break;
    }
    // write pgm/ppm slice
    return ppm ? slc_ppm_raw (os, slc) : slc_pgm_raw (os, slc);
  }

  // write a text slice to an output stream
  static bool slc_xxx_txt (OutputStream& os, const Slice& slc) {
    // get pixel format
    Pixel::t_pfmt pfmt = slc.getpfmt ();
    if (pfmt == Pixel::PFMT_NONE) return false;
    // select between pgm and ppm
    bool ppm = false;
    switch (pfmt) {
    case Pixel::PFMT_BYTE:
    case Pixel::PFMT_WORD:
    case Pixel::PFMT_REAL:
    case Pixel::PFMT_FLOT:
      ppm = false;
      break;
    case Pixel::PFMT_RGBA:
    case Pixel::PFMT_BGRA:
    case Pixel::PFMT_RGBR:
    case Pixel::PFMT_RGBF:
      ppm = true;
      break;
    default:
      return false;
      break;
    }
    // write pgm/ppm slice
    return ppm ? slc_ppm_txt (os, slc) : slc_pgm_txt (os, slc);
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // read an image from an input stream

  Image* Netpbm::read (InputStream* is) {
    return pnm_xxx_img (is);
  }

  // read an image by uri

  Image* Netpbm::read (const Uri& uri) {
    // create a uri stream with a tcp client
    auto iosm = [] (const String& host, const long port) -> Object* {
      return new TcpClient (host, port);
    };
    UriStream uris (iosm);
    // get the input stream
    InputStream* is = uris.istream (uri);
    // get the image
    return pnm_xxx_img (is);
  }

  // read an image by string uri

  Image* Netpbm::read (const String& suri) {
    // create a uri stream with a tcp client
    auto iosm = [] (const String& host, const long port) -> Object* {
      return new TcpClient (host, port);
    };
    UriStream uris (iosm);
    // get the input stream
    InputStream* is = uris.istream (suri);
    // get the image
    return pnm_xxx_img (is);
  }

  // write an image to an output stream

  bool Netpbm::write (OutputStream* os, Image* img, const bool raw) {
    // check first for a stream or image
    if ((os == nullptr) || (img == nullptr)) return false;
    // check for slice
    auto slc = dynamic_cast<Slice*>(img);
    if (slc == nullptr) return false;
    // write in raw or text mode
    return raw ? slc_xxx_raw (*os, *slc) : slc_xxx_txt (*os, *slc);
  }

  // write an image by uri

  bool Netpbm::write (const Uri& uri, Image* img, const bool raw) {
    // check for slice
    auto slc = dynamic_cast<Slice*>(img);
    if (slc == nullptr) return false;
    // create a uri stream with a tcp client
    auto iosm = [] (const String& host, const long port) -> Object* {
      return new TcpClient (host, port);
    };
    UriStream uris (iosm);
    // get the output stream
    OutputStream* os = uris.ostream (uri);
    // write in raw or text mode
    return raw ? slc_xxx_raw (*os, *slc) : slc_xxx_txt (*os, *slc);
  }

  // write an image by string uri

  bool Netpbm::write (const String& suri, Image* img, const bool raw) {
    // check for slice
    auto slc = dynamic_cast<Slice*>(img);
    if (slc == nullptr) return false;
    // create a uri stream with a tcp client
    auto iosm = [] (const String& host, const long port) -> Object* {
      return new TcpClient (host, port);
    };
    UriStream uris (iosm);
     // get the output stream
    OutputStream* os = uris.ostream (suri);
    // write in raw or text mode
    return raw ? slc_xxx_raw (*os, *slc) : slc_xxx_txt (*os, *slc);
  }
}
