// ---------------------------------------------------------------------------
// - Basexx.cpp                                                              -
// - afnix:uri service - base codec class implementation                     -
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

#include "Item.hpp"
#include "Basexx.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the codec name
  static const String CDC_BASE_NAME = "BASE64/32/16";
  // the default split line mode
  static const bool   CDC_LINE_MODE = false;
  // the default split line length
  static const long   CDC_LINE_LGTH = 64L;
  
  // the reverse table evaluator
  constexpr t_byte*
  base_xxxx_rptr(t_byte* dst, const t_byte* src, const long size) {
    // initialize the table
    for (long k = 0L; k < 256; k++) dst[k] = 0xFFU;
    // set the reverse value
    for (long k = 0L; k < size; k++) {
      long idx = (long) src[k];
      dst[idx] = (t_byte) k;
    }
    return dst;
  }
  
  // the standard base[64] encoding table
  static const t_byte BASE_SC64_CODE[] =
    {
     'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
     'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
     'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
     'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
     'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
     'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
     'w', 'x', 'y', 'z', '0', '1', '2', '3',
     '4', '5', '6', '7', '8', '9', '+', '/'     
    };
  // the standard base[64] reverse encoding table
  static t_byte  BASE_SC64_RTBL[256];
  static t_byte* BASE_SC64_RPTR =
    base_xxxx_rptr(BASE_SC64_RTBL, BASE_SC64_CODE, 64);
  
  // the url base[64] encoding table
  static const t_byte BASE_UC64_CODE[] =
    {
     'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
     'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
     'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
     'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
     'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
     'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
     'w', 'x', 'y', 'z', '0', '1', '2', '3',
     '4', '5', '6', '7', '8', '9', '-', '_'     
    };
  // the url base[64] reverse encoding table
  static t_byte  BASE_UC64_RTBL[256];
  static t_byte* BASE_UC64_RPTR =
    base_xxxx_rptr(BASE_UC64_RTBL, BASE_UC64_CODE, 64);

  // the standard base[32] encoding table
  static const t_byte BASE_SC32_CODE[] =
    {
     'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
     'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
     'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
     'Y', 'Z', '2', '3', '4', '5', '6', '7'
    };
  // the standard base[32] reverse encoding table
  static t_byte  BASE_SC32_RTBL[256];
  static t_byte* BASE_SC32_RPTR =
    base_xxxx_rptr(BASE_SC32_RTBL, BASE_SC32_CODE, 32);

  // the triacontakaidecimal base[32] encoding table
  static const t_byte BASE_EC32_CODE[] =
    {
     '0', '1', '2', '3', '4', '5', '6', '7',
     '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
     'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
     'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
    };
  // the triacontakaidecimal base[32] reverse encoding table
  static t_byte  BASE_EC32_RTBL[256];
  static t_byte* BASE_EC32_RPTR =
    base_xxxx_rptr(BASE_EC32_RTBL, BASE_EC32_CODE, 32);
  
  // the standard base[16] encoding table
  static const t_byte BASE_SC16_CODE[] =
    {
     '0', '1', '2', '3', '4', '5', '6', '7',
     '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };
  // the standard base[16] reverse encoding table
  static t_byte  BASE_SC16_RTBL[256];
  static t_byte* BASE_SC16_RPTR =
    base_xxxx_rptr(BASE_SC16_RTBL, BASE_SC16_CODE, 16);

  // the standard padding byte
  static t_byte BASE_SPAD_BYTE = '=';
  
  // the private codec structure
  struct s_codc {
    // the block size
    long d_bsiz;
    // the code sied
    long d_csiz;
    // the residue size
    long d_rsiz;
    // the residue value
    t_octa d_rval;
    // the residue mask
    t_octa d_rmsk;
    // the codec table
    t_byte* p_ctbl;
    // create a default structure
    s_codc (void) {
      reset ();
    }
    // reset the codec
    void reset (void) {
      d_bsiz = 0L;
      d_csiz = 0L;
      d_rsiz = 0L;
      d_rval = nilo;
      d_rmsk = nilo;
      p_ctbl = nullptr;
    }
    // clear the codec
    void clear (void) {
      d_rval = nilo;
      d_rsiz = 0L;
    }      
    // bind a codec table
    void bind (const Basexx::t_base base, const bool rflg) {
      switch (base) {
      case Basexx::BASE_SC64:
	d_bsiz = rflg ? 32L : 24L;
	d_csiz = 6L;
	d_rmsk = 0x000000000000003FU;
	p_ctbl = rflg ? BASE_SC64_RPTR : const_cast<t_byte*>(BASE_SC64_CODE);
	break;
      case Basexx::BASE_UC64:
	d_bsiz = rflg ? 32L : 24L;
	d_csiz = 6L;
	d_rmsk = 0x000000000000003FU;
	p_ctbl = rflg ? BASE_UC64_RPTR : const_cast<t_byte*>(BASE_UC64_CODE);
	break;
      case Basexx::BASE_SC32:
	d_bsiz = rflg ? 64 : 40L;
	d_csiz = 5L;
	d_rmsk = 0x000000000000001FU;
	p_ctbl = rflg ? BASE_SC32_RPTR : const_cast<t_byte*>(BASE_SC32_CODE);
	break;
      case Basexx::BASE_EC32:
	d_bsiz = rflg ? 64 : 40L;
	d_csiz = 5L;
	d_rmsk = 0x000000000000001FU;
	p_ctbl = rflg ? BASE_EC32_RPTR : const_cast<t_byte*>(BASE_EC32_CODE);
	break;
      case Basexx::BASE_SC16:
	d_bsiz = rflg ? 16L : 8L;
	d_csiz = 4L;
	d_rmsk = 0x000000000000000FU;
	p_ctbl = rflg ? BASE_SC16_RPTR : const_cast<t_byte*>(BASE_SC16_CODE);
	break;
      }
    }
    // push a byte in the codec
    void push (const t_byte bval) {
      // check for valid residue
      if (d_rsiz >= d_bsiz ) {
	throw Exception ("codec-error", "residue is full in push");
      }
      // shift/add to the residue
      d_rval <<= 8; d_rval |= (t_word) bval;
      // update the residue size
      d_rsiz += 8;
    }
    // pop a byte from the codec
    t_byte pop (void) {
      // check for valid residue
      if (d_rsiz < 8) {
	throw Exception ("codec-error", "residue is too short in pop");
      }
      t_byte rval = (t_byte) (0x00000000000000FFU & (d_rval >> (d_rsiz - 8)));
      // update the residue size
      d_rsiz -= 8;
      return rval;
    }
    // check for a valid encode/decode state
    bool valid (const bool rflg) const {
      // check residue size consistency
      if (d_rsiz > d_bsiz) {
	throw Exception ("codec-error", "excessive residue size in encode");
      }
      return (d_rsiz == d_bsiz) ? true : false;
    }
    // encode the residue in a buffer
    long encode (t_byte* rbuf) {
      // check residue size consistency
      if (d_rsiz > d_bsiz) {
	throw Exception ("codec-error", "excessive residue size in encode");
      }
      // check for null encoding
      if (d_rsiz == 0L) return 0L;
      // compute residue shift and padding
      long sval = (d_rsiz < d_bsiz) ? (d_bsiz - d_rsiz) % d_csiz : 0L;
      long plen = (d_rsiz < d_bsiz) ? (d_bsiz - d_rsiz) / d_csiz : 0L;
      // fill the residue size
      if (sval > 0L) {
	d_rval <<= sval; d_rsiz += sval;
      }
      // validate the residue size
      if ((d_rsiz % d_csiz) != 0L) {
	throw Exception ("codec-error", "invalid residue size in encode");
      }
      // encode the residue
      long result = 0L;
      while (d_rsiz >= d_csiz) {
	// get the code index
	long bidx = (long) (d_rmsk & (d_rval >> (d_rsiz - d_csiz)));
	// update the residue buffer by code
	rbuf[result++] = p_ctbl[bidx];
	// update residue size
	d_rsiz-= d_csiz;
      }
      // validate the residue size
      if (d_rsiz != 0L) {
	throw Exception ("codec-error", "invalid residue size in encode");
      }
      // eventually add the padding
      for (long k = 0L; k < plen; k++) rbuf[result++] = BASE_SPAD_BYTE;
      return result;
    }
    // decode the residue in a buffer
    long decode (t_byte* rbuf) {
      // check for null encoding
      if (d_rsiz == 0L) return 0L;
      // check residue size consistency
      if (d_rsiz != d_bsiz) {
	throw Exception ("codec-error", "invalid residue size in decode");
      }
      // remove the padding
      long plen  = 0;
      while (d_rsiz >= 8L) {
	if ((t_byte) d_rval != BASE_SPAD_BYTE) break;
	d_rval >>= 8; d_rsiz -= 8; plen++;
      }
      if (d_rsiz < 8) {
	throw Exception ("codec-error", "invalid residue size in decode");
      }
      // decode the byte stream
      t_octa rval = nilo; long rsiz = 0L;
      while (d_rsiz >= 8) {
	t_byte bval = (t_byte) (0x00000000000000FFU & (d_rval >> (d_rsiz - 8)));
	t_byte dval = p_ctbl[bval];
	if (dval == 0xFF) {
	  throw Exception ("codec-error", "invalid byte in decode");
	}
	rval <<= d_csiz; rval |= (t_octa) dval; rsiz += d_csiz; d_rsiz -= 8;
      }
      if ((d_rsiz != 0L) || (rsiz == 0L)) {
	throw Exception ("codec-error", "invalid residue size in decode");
      }
      // remove the trailing bit in residue
      long slen = rsiz % 8L; rval >>= slen; rsiz -= slen;
      // rebuild the result array
      long result = 0L;
      while (rsiz >= 8) {
	rbuf[result++] = (t_byte) (0x00000000000000FFU & (rval >> (rsiz - 8)));
	rsiz -= 8;
      }
      if (rsiz != 0L) {
	throw Exception ("codec-error", "invalid residue size in decode");
      }
      return result;
    }
  };

  // -------------------------------------------------------------------------
  // - public section                                                         -
  // -------------------------------------------------------------------------

  // create default base codec

  Basexx::Basexx (void) : Codec (CDC_BASE_NAME) {
    d_base = BASE_SC64;
    d_slmd = CDC_LINE_MODE;
    p_codc = new s_codc;
    reset ();
  }

  // create a base codec by type

  Basexx::Basexx (const t_base base) : Codec (CDC_BASE_NAME) {
    d_base = base;
    d_slmd = CDC_LINE_MODE;
    p_codc = new s_codc;
    reset ();
  }

  // destroy this codec

  Basexx::~Basexx (void) {
    delete p_codc;
  }

  // return the class name

  String Basexx::repr (void) const {
    return "Basexx";
  }

  // reset this base codec

  void Basexx::reset (void) {
    wrlock ();
    try {
      // reset the codec
      Codec::reset ();
      // reset the private structure
      d_slmd = CDC_LINE_MODE;
      p_codc->reset (); p_codc->bind (d_base, d_rflg);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this base codec

  void Basexx::clear (void) {
    wrlock ();
    try {
      // clear the codec
      Codec::clear (); p_codc->bind (d_base, d_rflg);
      // clear the private structure
      p_codc->clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the split line mode

  void Basexx::setslmd (const bool slmd) {
    wrlock ();
    try {
      d_slmd = slmd;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the split line mode

  bool Basexx::getslmd (void) const {
    wrlock ();
    try {
      bool result = d_slmd;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // encode an input buffer into an output buffer

  long Basexx::encode (Buffer& ob, Buffer& ib) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
        throw Exception ("cipher-error", "calling encode in reverse mode");
      }
      // the residue buffer
      t_byte rbuf[8];
      // the result length
      long result = 0L;
      // loop in the input buffer
      long lcnt = 0L;
      while (ib.empty () == false) {
	// push a byte in the codec
	p_codc->push (ib.read ());
	// encode in the residue buffer
	if (p_codc->valid(d_rflg) == true) {
	  // encode the residue buffer
	  long rlen = p_codc->encode(&rbuf[0]);
	  // push in the output buffer
	  for (long k = 0L; k < rlen; k++) {
	    ob.add ((const char) rbuf[k]);
	    if ((d_slmd == true) &&
		((++lcnt % CDC_LINE_LGTH) == 0L)) ob.add (eolc);
	  }
	  // update result length
	  result += rlen;
	}
      }
      // finalize the buffer
      long rlen = p_codc->encode(&rbuf[0]);
      // push in the output buffer
      for (long k = 0L; k < rlen; k++) ob.add ((const char) rbuf[k]);
      // update result length
      result += rlen;
      d_encs[0] += result; d_encs[1] += result;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode an input stream into an output buffer

  long Basexx::encode (Buffer& ob, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
        throw Exception ("cipher-error", "calling encode in reverse mode");
      }
      // the residue buffer
      t_byte rbuf[8];
      // the result length
      long result = 0L;
      // loop in the input buffer
      long lcnt = 0L;
      while (is.valid () == true) {
	// push a byte in the codec
	p_codc->push (is.read ());
	// encode in the residue buffer
	if (p_codc->valid(d_rflg) == true) {
	  // encode the residue buffer
	  long rlen = p_codc->encode(&rbuf[0]);
	  // push in the output buffer
	  for (long k = 0L; k < rlen; k++) {
	    ob.add ((const char) rbuf[k]);
	    if ((d_slmd == true) &&
		((++lcnt % CDC_LINE_LGTH) == 0L)) ob.add (eolc);
	  }
	  // update result length
	  result += rlen;
	}
      }
      // finalize the buffer
      long rlen = p_codc->encode(&rbuf[0]);
      // push in the output buffer
      for (long k = 0L; k < rlen; k++) ob.add ((const char) rbuf[k]);
      // update result length
      result += rlen;
      d_encs[0] += result; d_encs[1] += result;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode an input stream into an output stream

  long Basexx::encode (OutputStream& os, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
        throw Exception ("cipher-error", "calling encode in reverse mode");
      }
      // the residue buffer
      t_byte rbuf[8];
      // the result length
      long result = 0L;
      // loop in the input buffer
      long lcnt = 0L;
      while (is.valid () == true) {
	// push a byte in the codec
	p_codc->push (is.read ());
	// encode in the residue buffer
	if (p_codc->valid(d_rflg) == true) {
	  // encode the residue buffer
	  long rlen = p_codc->encode(&rbuf[0]);
	  // push in the output buffer
	  for (long k = 0L; k < rlen; k++) {
	    os.write ((const char) rbuf[k]);
	    if ((d_slmd == true) &&
		((++lcnt % CDC_LINE_LGTH) == 0L)) os.write (eolc);
	  }
	  // update result length
	  result += rlen;
	}
      }
      // finalize the buffer
      long rlen = p_codc->encode(&rbuf[0]);
      // push in the output buffer
      for (long k = 0L; k < rlen; k++) os.write ((const char) rbuf[k]);
      // update result length
      result += rlen;
      d_encs[0] += result; d_encs[1] += result;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // decode an input buffer into an output buffer

  long Basexx::decode (Buffer& ob, Buffer& ib) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
        throw Exception ("cipher-error", "calling decode in non reverse mode");
      }
      // the residue buffer
      t_byte rbuf[8];
      // the result length
      long result = 0L;
      // loop in the input buffer
      while (ib.empty () == false) {
	// get the next byte and eventually remove
	t_byte bval = ib.read ();
	if ((bval == '\r') || (bval == '\n')) continue;
	// push the byte in the codec
	p_codc->push (bval);
	// encode in the residue buffer
	if (p_codc->valid(d_rflg) == true) {
	  // decode the residue buffer
	  long rlen = p_codc->decode(&rbuf[0]);
	  // push in the output buffer
	  for (long k = 0L; k < rlen; k++) ob.add ((const char) rbuf[k]);
	  // update result length
	  result += rlen;
	}
      }
      d_decs[0] += result; d_decs[1] += result;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an input stream into an output buffer

  long Basexx::decode (Buffer& ob, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
        throw Exception ("cipher-error", "calling decode in non reverse mode");
      }
      // the residue buffer
      t_byte rbuf[8];
      // the result length
      long result = 0L;
      // loop in the input buffer
      while (is.valid () == true) {
	// get the next byte and eventually remove
	t_byte bval = is.read ();
	if ((bval == '\r') || (bval == '\n')) continue;
	// push the byte in the codec
	p_codc->push (bval);
	// encode in the residue buffer
	if (p_codc->valid(d_rflg) == true) {
	  // decode the residue buffer
	  long rlen = p_codc->decode(&rbuf[0]);
	  // push in the output buffer
	  for (long k = 0L; k < rlen; k++) ob.add ((const char) rbuf[k]);
	  // update result length
	  result += rlen;
	}
      }
      d_decs[0] += result; d_decs[1] += result;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an input stream into an output stream

  long Basexx::decode (OutputStream& os, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
        throw Exception ("cipher-error", "calling decode in non reverse mode");
      }
      // the residue buffer
      t_byte rbuf[8];
      // the result length
      long result = 0L;
      // loop in the input buffer
      while (is.valid () == true) {
	// get the next byte and eventually remove
	t_byte bval = is.read ();
	if ((bval == '\r') || (bval == '\n')) continue;
	// push the byte in the codec
	p_codc->push (bval);
	// encode in the residue buffer
	if (p_codc->valid(d_rflg) == true) {
	  // decode the residue buffer
	  long rlen = p_codc->decode(&rbuf[0]);
	  // push in the output buffer
	  for (long k = 0L; k < rlen; k++) os.write ((const char) rbuf[k]);
	  // update result length
	  result += rlen;
	}
      }
      d_decs[0] += result; d_decs[1] += result;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // preset the stream processing

  long Basexx::preset (void) {
    wrlock ();
    try {
      // clear the private structure
      if (p_codc != nullptr) p_codc->clear ();
      long result = 0L;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // finish the stream processing

  long Basexx::finish (void) {
    wrlock ();
    try {
      long result = 0L;
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
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETSLMD = zone.intern ("set-split-mode");
  static const long QUARK_GETSLMD = zone.intern ("get-split-mode");

  // the object eval quarks
  static const long QUARK_BASEXX   = String::intern ("Basexx");
  static const long QUARK_BASESC64 = String::intern ("SC64");
  static const long QUARK_BASEUC64 = String::intern ("UC64");
  static const long QUARK_BASESC32 = String::intern ("SC32");
  static const long QUARK_BASEEC32 = String::intern ("EC32");
  static const long QUARK_BASESC16 = String::intern ("SC16");

  // map an item to a base type
  static inline Basexx::t_base item_to_base (const Item& item) {
    // check for a key item
    if (item.gettid () != QUARK_BASEXX)
      throw Exception ("item-error", "item is not a basexx item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_BASESC64) return Basexx::BASE_SC64;
    if (quark == QUARK_BASEUC64) return Basexx::BASE_UC64;
    if (quark == QUARK_BASESC32) return Basexx::BASE_SC32;
    if (quark == QUARK_BASEEC32) return Basexx::BASE_EC32;
    if (quark == QUARK_BASESC16) return Basexx::BASE_SC16;
    throw Exception ("item-error", "cannot map item to base type");
  }

  // evaluate an object data member

  Object* Basexx::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    // key type part
    if (quark == QUARK_BASESC64) return new Item (QUARK_BASEXX, QUARK_BASESC64);
    if (quark == QUARK_BASEUC64) return new Item (QUARK_BASEXX, QUARK_BASEUC64);
    if (quark == QUARK_BASESC32) return new Item (QUARK_BASEXX, QUARK_BASESC32);
    if (quark == QUARK_BASEEC32) return new Item (QUARK_BASEXX, QUARK_BASEEC32);
    if (quark == QUARK_BASESC16) return new Item (QUARK_BASEXX, QUARK_BASESC16);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }
  
  // create a new object in a generic way
  
  Object* Basexx::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Basexx;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      auto item = dynamic_cast<Item*> (obj);
      if (item == nullptr) {
	throw Exception ("type-error", "invalid object with basexx constructor",
			 Object::repr (obj));
      }
      t_base base = item_to_base (*item);
      return new Basexx (base);
    }
    throw Exception ("argument-error", "too many arguments with basexx");
  }

  // return true if the given quark is defined

  bool Basexx::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
        unlock ();
        return true;
      }
      bool result = hflg ? Codec::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Basexx::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETSLMD) return new Boolean (getslmd ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETSLMD) {
	bool slmd = argv->getbool (0);
	setslmd (slmd);
	return nullptr;
      }
    }
    // call the codec method
    return Codec::apply (zobj, nset, quark, argv);
  }
}
