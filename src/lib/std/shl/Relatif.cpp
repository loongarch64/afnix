// ---------------------------------------------------------------------------
// - Relatif.cpp                                                             -
// - standard object library - relatif big number class implementation       -
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

#include "Real.hpp"
#include "Vector.hpp"
#include "Stdsid.hxx"
#include "Buffer.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Relatif.hpp"
#include "Evaluable.hpp"
#include "Character.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"
#include "ccnv.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the 32 bits shift factor
  const long   BIT_QUAD_SHIX = 5;
  // the 32 bits unity
  const t_quad MPI_QUAD_PONE = 0x00000001UL;
  // the 32 bits maximum
  const t_quad MPI_QUAD_PMAX = 0xFFFFFFFFUL;
  // the maximum unsigned 32 bits integer
  const t_octa MAX_OCTA_UVAL = 0x00000000FFFFFFFFULL;
  // the mpi quad mask
  const t_octa MPI_QUAD_MASK = 0x00000000FFFFFFFFULL;
  // the 64 bits unity
  const t_octa MPI_OCTA_PONE = 0x0000000000000001ULL;
  // the 64 bits upper one
  const t_octa MPI_OCTA_HONE = 0x0000000100000000ULL;
  // the maximum positive 64 bits integer
  const t_octa MAX_OCTA_PVAL = 0x7FFFFFFFFFFFFFFFULL;

  // this function computes the maximum of two long integers
  static inline long max (const long x, const long y) {
    return x > y ? x : y;
  }

  // this function computes the byte size of a quad
  static inline long bsq (const t_quad x) {
    // check byte 3
    if ((x & 0xFF000000UL) != nilq) return 4;
    // check byte 2
    if ((x & 0x00FF0000UL) != nilq) return 3;
    // check byte 1
    if ((x & 0x0000FF00UL) != nilq) return 2;
    // only one byte
    return 1;
  }

  // this function sign extend a quad by byte - the number is assumed to be
  // negative with at least one byte with the msb set to 1
  static inline t_quad sext (const t_quad x) {
    t_quad result = x;
    if ((result & 0x80000000UL) == nilq) {
      result |= 0xFF000000UL;
      if ((result & 0x00800000UL) == nilq) {
	result |= 0x00FF0000UL;
	if ((result & 0x000008000UL) == nilq) {
	  result |= 0x0000FF00UL;
	  if ((result & 0x00000080UL) == nilq) {
	    throw Exception ("quad-error", "invalid byte in signed quad");
	  }
	}
      }
    }
    return result;
  }

  // this function sign extend a byte by bit
  static inline t_byte sext (const t_byte x) {
    // check for null first
    if (x == 0x00) return x;
    // check for extened already
    if ((x & 0x80) != 0x00) return x;
    // initialize bit position
    long   pos = 1;
    t_byte val = x;
    for (long i = 0; i < 8; i++) {
      val <<= 1;
      if ((val & 0x80) != 0x00) break;
      pos++;
    }
    // check null (should not happen)
    if (val == 0x00) return 0x00;
    // extend byte
    for (long i = 0; i < pos; i++) {
      val >>= 1;
      val |= 0x80;
    }
    return val;
  }

  // this function extracts a byte from a quad
  static inline t_byte getqb (const t_quad x, const long index) {
    // prepare working quad
    t_quad qval = x;
    // check for index 0
    if (index == 0) return (t_byte) (qval & 0x000000FFUL);
    // check for index 1
    if (index == 1) return (t_byte) ((qval >> 8) & 0x000000FFUL);
    // check for index 2
    if (index == 2) return (t_byte) ((qval >> 16) & 0x000000FFUL);
    // check for index 3
    if (index == 3) return (t_byte) ((qval >> 24) & 0x000000FFUL);
    // invalid index
    throw Exception ("mpi-error", "illegal quad byte index");
  }

  // this function make a quad from a byte by index
  static inline t_quad makqb (const t_byte x, const long index) {
    // prepare quad result
    t_quad result = (t_quad) x;
    // check for index 0
    switch (index) {
    case 0:
      break;
    case 1:
      result <<= 8;
      break;
    case 2:
      result <<= 16;
      break;
    case 3:
      result <<= 24;
      break;
    default:
      throw Exception ("mpi-error", "illegal byte quad index");
      break;
    }
    return result;
  }

  // this function initialize an array of quad
  static inline void rstdat (const long size, t_quad* data) {
    for (t_quad* p = &data[0]; p < &data[size]; p++) *p = nilq;
  }

  // this function compute the msb of a quad
  static inline long quadmsb (const t_quad x) {
    // check for null first
    if (x == 0) return 0;
    // find the msb
    long lsb = 0;
    long msb = 32;
    while ((lsb + 1) != msb) {
      long mid = (lsb + msb) >> 1;
      if ((x & (MPI_QUAD_PMAX << mid)) != 0)
	lsb = mid;
      else
	msb = mid;
    }
    return msb;
  }

  // this function computes the lsb of a quad
  static inline long quadlsb (const t_quad x) {
    // check for null first
    if (x == 0) return 0;
    // find the lsb
    for (long i = 0; i < 32; i++) {
      if (((x >> i) & MPI_QUAD_PONE) == MPI_QUAD_PONE) return (i+1);
    }
    return 0;
  }

  // this function convert a character according to a base
  static inline long ctol (const char c, const long base) {
    switch (base) {
    case 2:
      if (c == '0') return 0;
      if (c == '1') return 1;
      break;
    case 10:
      if ((c >= '0') && (c <= '9')) return (long) (c - '0');
      break;
    case 16:
      if ((c >= '0') && (c <= '9')) return  (long) (c - '0');
      if ((c >= 'a') && (c <= 'f')) return ((long) (c - 'a')) + 10;
      if ((c >= 'A') && (c <= 'F')) return ((long) (c - 'A')) + 10;
      break;
    }
    throw Exception ("format-error", "cannot convert character in base");
  }

  // this function converts a string to a relatif object
  static Relatif strtor (const String& s) {
    // initialize base
    long base   = 10;
    // initialise result
    Relatif basval = 1;
    Relatif result = 0;  
    
    // check for size first
    long len = s.length ();
    if (len == 0) return result;
    // process one character
    if (len == 1) {
      result = ctol (s[0], 10);
      return result;
    }

    // here we have at least two characters - it can be the sign, the format
    // or a normal number
    bool sign  = false;
    long index = 0;
    // check for the sign
    if (s[0] == '-') {
      index++;
      sign = true;
      goto format;
    }
    if (s[0] == '+') {
      index++;
      sign = false;
      goto format;
    }
    
    // check for the format
  format:
    if (s[index] != '0') goto number;
    index++;
    if (index >= len) return result;
    if ((s[index] == 'x') || (s[index] == 'X')) {
      index++;
      if (index >= len)
	throw Exception ("format-error", "cannot convert to relatif", s);
      base = 16;
      goto number;
    }
    if ((s[index] == 'b') || (s[index] == 'B')) {
      index++;
      if (index >= len)
	throw Exception ("format-error", "cannot convert to relatif", s);
      base = 2;
      goto number;
    }
    
    // compute the number value
  number:
    // check for the last index
    long max = len - 1;
    if ((s[max] == 'r') || (s[max] == 'R')) max--;
    // loop in the digits
    for (long i = max; i >= index ; i--) {
      result = result + (basval * ctol (s[i], base));
      basval = basval * base;
    };
    return (sign) ? -result : result;
  }

  // the mpi structure - the mpi array is organized in ascending order
  // quad[0] is the least significand quad while in byte mode rbuf[0] is the
  // most significand byte (like a string reading from left to right)
  struct s_mpi {
    // the mpi size
    long d_size;
    // the clamp flag
    bool d_cflg;
    // the mpi array
    t_quad* p_data;

    // the default mpi
    s_mpi (void) {
      d_size = 1;
      d_cflg = true;
      p_data = new t_quad (0);
    }

    // create a mpi by exponent and value
    s_mpi (const long size, const t_quad x) {
      if (x == nilq) {
	d_size = 1;
	p_data = new t_quad (nilq);
      } else {
	d_size = size;
	p_data = new t_quad[d_size];
	for (long i = 0; i < d_size; i++) p_data[i] = nilq;
	p_data[d_size - 1] = x;
      }
      // mark the clamp flag
      d_cflg = true;
    }

    // create a mpi by data and size
    s_mpi (const t_byte* rbuf, const long size) {
      // preset the buffer arreay
      d_size = 0;
      p_data = nullptr;
      d_cflg = false;
      // set as an unsigned buffer
      setubuf (rbuf, size);
    }

    // create a mpi by data and size
    s_mpi (t_quad* data, const long size) {
      d_size = size;
      d_cflg = false;
      p_data = data;
    }

    // create a mpi by value
    s_mpi (const t_octa x) {
      // find the size
      if (x > MAX_OCTA_UVAL) {
	d_size = 2;
	p_data = new t_quad[2];
	p_data[0] = x;
	p_data[1] = x >> 32;
      } else {
	d_size = 1;
	p_data = new t_quad[1];
	p_data[0] = x;
      }
      d_cflg = true;
    }

    // copy construct this mpi
    s_mpi (const s_mpi& that) {
      d_size = that.d_size;
      d_cflg = that.d_cflg;
      p_data = new t_quad[d_size];
      for (long i = 0; i < d_size; i++) p_data[i] = that.p_data[i];
    }
    
    // copy move this mpi
    s_mpi (s_mpi&& that) noexcept {
      d_size = that.d_size; that.d_size = 0L;
      d_cflg = that.d_cflg; that.d_cflg = true;
      p_data = that.p_data; that.p_data = nullptr;
    }
    
    // destroy this mpi
    ~s_mpi (void) {
      delete [] p_data;
    };

    // copy an mpi into this one
    s_mpi& operator = (const s_mpi& that) {
      // normal check as usual
      if (this == &that) return *this;
      // cleanup and copy
      delete [] p_data;
      d_size = that.d_size;
      d_cflg = that.d_cflg;
      p_data = new t_quad[d_size];
      for (long i = 0; i < d_size; i++) p_data[i] = that.p_data[i];
      return *this;
    }

    // move a mpi to this one
    s_mpi& operator = (s_mpi&& that) noexcept {
      // cleanup and move
      delete [] p_data;
      d_size = that.d_size; that.d_size = 0L;
      d_cflg = that.d_cflg; that.d_cflg = true;
      p_data = that.p_data; that.p_data = nullptr;
      return *this;
    }
    
    // grow this mpi by a certain amount
    void grow (const long size) {
      // trivial case first
      if ((size <= 0) || (size <= d_size)) return;
      // allocate the new array
      t_quad* data = new t_quad[size];
      // copy the array
      for (long i = 0; i < d_size; i++)    data[i] = p_data[i];
      for (long i = d_size; i < size; i++) data[i] = nilq;
      // fix old value
      delete [] p_data;
      d_size = size;
      p_data = data;
      d_cflg = false;
    }

    // serialize the mpi data
    void wrstream (OutputStream& os) {
      Serial::wrlong (d_size, os);
      // serialize the array
      for (long i = 0; i < d_size; i++) {
	t_byte bval[4];
	c_qhton (p_data[i], bval);
	os.write ((const char*) bval, 4);
      }
    }

    // get the mpi size quad size
    long vsize (void) const {
      // check for clamped
      if (d_cflg == true) return d_size;
      // initialize the size and check
      long size = d_size;
      for (long i = d_size - 1; i > 0; i--) {
	if (p_data[i] != nilq) break;
	size--;
      }
      return size;
    }

    // get the mpi byte clamped size
    long bsize (void) const {
      // initialize the quad size
      long size = vsize ();
      // get the last quad size
      long result = bsq (p_data[size-1]);
      // add the quad size
      result += (size-1) * 4;
      // here it is
      return result;
    }

    // clamp this mpi by fixing the size and return true if null
    bool clamp (void) {
      // fix the size if not already clamped
      if (d_cflg == false) {
	d_size = vsize ();
	d_cflg = true;
      }
      // check for null
      if ((d_size == 1) && (p_data[0] == nilq)) return true;
      return false;
    }

    // get a byte by index
    t_byte getbyte (const long index) const {
      // extract byte and quad indexes
      long qidx = index / 4;
      long bidx = index % 4;
      // check for index
      if (qidx >= d_size) {
	throw Exception ("mpi-error", "invalid quad index in byte access");
      }
      // get quad value
      t_quad qval = p_data[qidx];
      // extract byte value
      return getqb (qval, bidx);      
    }

    // get a byte by index
    void setbyte (const t_byte x, const long index) {
      // extract byte and quad indexes
      long qidx = index / 4;
      long bidx = index % 4;
      // check for index
      if (qidx >= d_size) {
	throw Exception ("mpi-error", "invalid quad index in byte access");
      }
      // get quad value
      t_quad qval = makqb (x, bidx);
      // set the quad value
      p_data[qidx] |= qval;
    }

    // fill an unsigned buffer representation by size
    long toubuf (t_byte* rbuf, const long size) const {
      // get the clamped byte size
      long cbsz = bsize ();
      // fill with size range
      if (size <= cbsz) {
	for (long i = 0, j = cbsz-1; i < size; i++, j--) rbuf[i] = getbyte (j);
	return size;
      }
      // process large size
      for (long i = 0, j = cbsz-1; i < cbsz; i++, j--) rbuf[i] = getbyte (j);
      for (long i = cbsz; i < size; i++) rbuf[i] = nilc;
      return cbsz;
    }

    // fill an unsigned buffer representation by size
    long tosbuf (t_byte* rbuf, const long size, const bool sgn) const {
      // get the clamped byte size
      long cbsz = bsize ();
      // prepare the signed buffer
      t_byte sbuf[cbsz];
      t_word cw = 0x0001U;
      for (long i = 0; i < cbsz; i++) {
	if (sgn == true) {
	  t_word bo = ((t_word) ~getbyte (i)) & 0x00FFU;
	  t_word ro = bo + cw;
	  sbuf[i] = (t_byte) ro;
	       cw = (ro >> 8) & 0x0001U;
	} else {
	  sbuf[i] = getbyte (i);
	}
      }
      // sign extend and clamp if needed
      if (sgn == true) {
	// sign extend the last byte
	sbuf[cbsz-1] = sext (sbuf[cbsz-1]);
	// clamp the signed buffer
	long csiz = cbsz;
	for (long i = cbsz - 1; i > 0; i--) {
	  if ((cbsz == 1) || (sbuf[i] != 0xFF)) break;
	  if ((sbuf[i] == 0xFF) && ((sbuf[i-1] & 0x80) == nilc)) break;
	  csiz--;
	}
	// adjust buffer size
	cbsz = csiz;
      }
      // fill with size range
      if (size <= cbsz) {
	for (long i = 0, j = cbsz-1; i < size; i++, j--) rbuf[i] = sbuf[j];
	return size;
      }
      // process large size
      for (long i = 0, j = cbsz-1; i < cbsz; i++, j--) rbuf[i] = sbuf[j];
      for (long i = cbsz; i < size; i++) rbuf[i] = nilc;
      return cbsz;
    }

    // fill a buffer representation by size
    long tobyte (t_byte* rbuf, const long size) const {
      // get the clamped byte size
      long cbsz = bsize ();
      // fill with size range
      if (size <= cbsz) {
	for (long i = 0, j = size-1; i < size; i++, j--) rbuf[i] = getbyte (j);
	return size;
      } 
      long dbsz = size - cbsz;
      for (long i = 0; i < dbsz; i++) rbuf[i] = nilc;
      for (long i = dbsz, j = cbsz-1; i < size; i++, j--) rbuf[i] = getbyte (j);
      return cbsz;
    }

    // return a hexadecimal string value without prefix
    String tohexa (void) const {
      // initialize the result string
      String result = Utility::tohexa (p_data[d_size-1]);
      // get the clamped size
      long size = vsize ();
      for (long i = size - 2; i >= 0; i--) {
	result += Utility::tohexa (p_data[i], true, false);
      }
      return result;
    }

    // convert this mpi to an octa
    t_octa toocta (void) const {
      // check for size
      if (d_size == 0) return 0;
      // prepare the result
      t_octa data = p_data[0];
      if (d_size > 1) data |= (((t_octa) p_data[1]) << 32);
      // here it is
      return data;
    }

    // return true if this mpi is zero
    bool iszero (void) const {
      // check for single digit clamped
      if (d_cflg == true) {
	if ((d_size == 1) && (p_data[0] == nilq)) return true;
      }
      // get the true size and loop
      long size = vsize ();
      for (long i = 0; i < size; i++) {
	if (p_data[i] != nilq) return false;
      }
      return true;
    }

    // return true if this mpi is odd
    bool isodd (void) const {
      return (p_data[0] & MPI_QUAD_PONE) == MPI_QUAD_PONE;
    }
    
    // return true if this mpi is odd
    bool iseven (void) const {
      return (p_data[0] & MPI_QUAD_PONE) == 0UL;
    }

    // return the msb of this mpi
    long getmsb (void) const {
      // start by clamping the size
      long size = vsize ();
      if ((size == 1) && (p_data[0] == nilq)) return 0;
      // find the quad msb
      long mmsb = quadmsb (p_data[size-1]);
      // here is the msb
      return ((size - 1) << BIT_QUAD_SHIX) + mmsb;
    }

    // return the lsb of this mpi
    long getlsb (void) const {
      // start by clamping the size
      long size = vsize ();
      if ((size == 1) && (p_data[0] == nilq)) return 0;
      // find the quad lsb
      long qlsb = 0;
      for (long i = 0; i < size; i++) {
	if (p_data[i] != nilq) break;
	qlsb++;
      }
      long mlsb = quadlsb (p_data[qlsb]);
      // here is the lsb
      return (qlsb << BIT_QUAD_SHIX) + mlsb;
    }

    // get a bit value by position
    bool getbit (const long bpos) const {
      // compute quad index
      long index = bpos >> BIT_QUAD_SHIX;
      if (index >= d_size) return false;
      // get the quad and bit mark
      t_quad data = p_data[index];
      long   mark = bpos % 32;
      // read bit value
      t_quad qval = ((mark == 0) ? data : (data >> mark)) & MPI_QUAD_PONE;
      // compute result
      return (qval == MPI_QUAD_PONE);
    }

    // set a bit by position
    void setbit (const long bpos, const bool bval) {
      // compute quad index
      long index = bpos >> BIT_QUAD_SHIX;
      if (index >= d_size) {
	throw Exception ("mpi-error", "invalid mpi bit position");
      }
      // get the quad and bit mark
      t_quad data = p_data[index];
      long   mark = bpos % 32;
      // prepare the mask
      t_quad mask = (mark == 0) ? MPI_QUAD_PONE : (MPI_QUAD_PONE << mark);
      // set the quad bit value
      p_data[index] = bval ? (data | mask) : (data & ~mask);
    }

    // set the mpi with an unsigned buffer
    void setubuf (const t_byte* ubuf, const long size) {
      // clear the old buffer
      delete [] p_data;
      // compute the number of quads
      d_size = size / 4;
      if ((size % 4) != 0) d_size++;
      // allocate the quad array
      p_data = new t_quad[d_size];
      // initialize the array
      rstdat (d_size, p_data);
      // fill the quad array
      for (long i = 0, j = size-1; i < size; i++, j--) setbyte (ubuf[i], j);
      // clear the clamp flag
      d_cflg = false;
    }

    // set the mpi with an signed buffer
    bool setsbuf (const t_byte* sbuf, const long size) {
      // consider an unsigned buffer
      setubuf (sbuf, size);
      // find the buffer sign
      bool sign = ((sbuf[0] & 0x80) == 0x80);
      // process the negative value
      if (sign == true) {
	// sign extend the last quad
	p_data[d_size-1] = sext (p_data[d_size-1]);
	// fix the complemented value
	t_octa co = MPI_OCTA_PONE;
	for (long i = 0; i < d_size; i++) {
	  t_octa ro = (((t_octa) ~p_data[i]) & MPI_QUAD_MASK) + co;
	  p_data[i] = (t_quad) ro;
	         co = (ro >> 32) & MPI_OCTA_PONE;
	}
      }
      return sign;
    }

    // radix shift right by a certain amount
    void rsr (const long rsa) {
      // get the mpi size
      long size = vsize ();
      // loop and shift
      for (long i = 0; i < size; i++) {
	long  idx = i + rsa;
	p_data[i] = (idx < size) ? p_data[idx] : nilq;
      }
      // clamp
      d_size = size - rsa; if (d_size <= 0) d_size = 1;
      d_cflg = true;
    }

    // get the viewable byte size
    long tovsize(void) const {
      return d_size * sizeof(t_quad);
    }

    // get the viewable byte array
    t_byte* tovbyte(void) {
      return reinterpret_cast<t_byte*>(p_data);
    }

    // get the viewable byte array
    const t_byte* tovbyte(void) const {
      return reinterpret_cast<const t_byte*>(p_data);
    }
  };

  // this function generates a random mpi upto n bits
  static s_mpi* mpi_rng (const long bits) {
    // check for null first
    if (bits == 0) return new s_mpi;
    // compute number of quads
    long    size = ((bits - 1) / 32) + 1;
    t_quad* data = new t_quad[size];
    for (long i = 0; i < size; i++) data[i] = Utility::quadrnd ();
    // build the mpi result
    s_mpi* result = new s_mpi (data, size);
    // fix upper bits
    long mbit = (((bits - 1) / 32) * 32) + 31;
    for (long i = bits; i <= mbit; i++) result->setbit (i, false);
    // clamp the result
    result->clamp ();
    // here is the result
    return result;
  }

  // this function generates a random n bits mpi
  static s_mpi* mpi_rnd (const long bits) {
    // check for null first
    if (bits == 0) return new s_mpi;
    // compute number of quads
    long    size = ((bits - 1) / 32) + 1;
    t_quad* data = new t_quad[size];
    for (long i = 0; i < size; i++) data[i] = Utility::quadrnd ();
    // build the mpi result
    s_mpi* result = new s_mpi (data, size);
    // fix upper bits
    long mbit = (((bits - 1) / 32) * 32) + 31;
    result->setbit (bits - 1, true);
    for (long i = bits; i <= mbit; i++) result->setbit (i, false);
    // clamp the result
    result->clamp ();
    // here is the result
    return result;
  }

  // this function deserialize a stream into a mpi
  static s_mpi* mpi_rds (InputStream& is) {
    // deserialize the size
    long size = Serial::rdlong (is);
    // deserialize the data
    t_quad* data = new t_quad[size];
    for (long i = 0; i < size; i++) {
      t_byte bval[4];
      for (long j = 0; j < 4; j++) bval[j] = is.read ();
      data[i] = c_qntoh (bval);
    }
    // build the mpi result
    return new s_mpi (data, size);
  }

  // this function compare a mpi with an octa
  static bool mpi_eql (const s_mpi& x, const t_octa y) {
    // get the maximum size
    long size = max (x.d_size, 2);
    // loop in the size
    for (long i = size - 1; i >= 0; i--) {
      t_quad xq = (i < x.d_size) ? x.p_data [i] : nilq;
      t_quad yq = (i == 0) ? (t_quad) y : (i == 1) ? (t_quad) (y >> 32) : 0ULL;
      if (xq != yq) return false;
    }
    // here the numbers are equal
    return true;
  }

  // this function compare two mpi values
  static bool mpi_eql (const s_mpi& x, const s_mpi& y) {
    // get the maximum size
    long size = max (x.vsize (), y.vsize ());
    // loop in the size
    for (long i = size - 1; i >= 0; i--) {
      t_quad xq = (i < x.d_size) ? x.p_data [i] : nilq;
      t_quad yq = (i < y.d_size) ? y.p_data [i] : nilq;
      if (xq != yq) return false;
    }
    // here the numbers are equal
    return true;
  }
  
  // this function compare a mpi with an octa
  static bool mpi_gth (const s_mpi& x, const t_octa y) {
    // get the maximum size
    long size = max (x.vsize (), 2);
    // loop in the size
    for (long i = size - 1; i >= 0; i--) {
      t_quad xq = (i < x.d_size) ? x.p_data [i] : nilq;
      t_quad yq = (i == 0) ? (t_quad) y : (i == 1) ? (t_quad) (y >> 32) : 0;
      if (xq > yq) return true;
      if (xq < yq) return false;
    }
    // here the numbers are equal
    return false;
  }

  // this function compare two mpi values
  static bool mpi_gth (const s_mpi& x, const s_mpi& y) {
    // get the maximum size
    long size = max (x.vsize (), y.vsize ());
    // loop in the size
    for (long i = size - 1; i >= 0; i--) {
      t_quad xq = (i < x.d_size) ? x.p_data [i] : nilq;
      t_quad yq = (i < y.d_size) ? y.p_data [i] : nilq;
      if (xq > yq) return true;
      if (xq < yq) return false;
    }
    // here the numbers are equal
    return false;
  }

  // this function compares a data array with an octa
  static bool mpi_geq (const s_mpi& x, const t_octa y) {
    // get the maximum size
    long size = max (x.vsize (), 2);
    // loop in the size
    for (long i = size - 1; i >= 0; i--) {
      t_quad xq = (i < x.d_size) ? x.p_data [i] : nilq;
      t_quad yq = (i == 0) ? (t_quad) y : (i == 1) ? (t_quad) (y >> 32) : 0;
      if (xq > yq) return true;
      if (xq < yq) return false;
    }
    // here the numbers are equal
    return true;
  }

  // this function compares a data array with an mpi value
  static bool mpi_geq (const s_mpi& x, const s_mpi& y) {
    // get the maximum size
    long size = max (x.vsize (), y.vsize ());
    // loop in the size
    for (long i = size - 1; i >= 0; i--) {
      t_quad xq = (i < x.d_size) ? x.p_data [i] : nilq;
      t_quad yq = (i < y.d_size) ? y.p_data [i] : nilq;
      if (xq > yq) return true;
      if (xq < yq) return false;
    }
    // here the numbers are equal
    return true;
  }

  // shift left in place a mpi by a certain amount
  static void mpi_leq (s_mpi& x, const long asl) {
    // fix trivial case
    if (asl == 0) return;
    // compute the amount of quad and bit shift
    long qsl = asl >> BIT_QUAD_SHIX;
    long bsl = asl % 32;
    // compute how much we must grow
    long msb = x.getmsb ();
    long max = x.d_size - 1;
    if (((msb + bsl) > 32) || (qsl > 0)) x.grow (x.d_size + qsl + 1);
    // shift the data
    for (long i = max; i >= 0; i--) {
      // shift octa
      t_octa xo = x.p_data[i];
      t_octa ro = (xo << bsl);
      // process upper quad
      t_quad uq = (t_quad) (ro >> 32);
      if (uq != nilq) x.p_data[i+qsl+1] |= uq;
      // process lower quad
      t_quad lq = (t_quad) ro;
      x.p_data[i+qsl] = lq;
    }
    // clean remaining quad
    for (long i = 0; i < qsl; i++) x.p_data[i] = nilq;
    // reset clamp flag
    x.d_cflg = false;
  }

  // shift right in place a mpi by a certain amount
  static void mpi_req (s_mpi& x, const long asr) {
    // fix trivial case
    if (asr == 0) return;
    // compute the amount of quad and bit shift
    long qsr = asr >> BIT_QUAD_SHIX;
    long bsr = asr % 32;
    // shift the data
    for (long i = 0; i < x.d_size; i++) {
      long index = i + qsr;
      // shift octa
      t_octa xo = (index < x.d_size) ? x.p_data[index] : 0;
      t_octa ro = xo << (32 - bsr);
      // process upper quad
      t_quad uq = (t_quad) (ro >> 32);
      x.p_data[i] = uq;
      // process lower quad
      t_quad lq = (t_quad) ro;
      if (i > 0) x.p_data[i-1] |= lq;
    }
    // reset clamp flag
    x.d_cflg = false;
  }

  // shift left a mpi by a certain amount
  static s_mpi* mpi_shl (const s_mpi& x, const long asl) {
    // fix trivial case
    if (asl == 0) return new s_mpi (x);
    // compute the amount of quad and bit shift
    long qsl = asl >> BIT_QUAD_SHIX;
    long bsl = asl % 32;
    // compute the new size and allocate
    long    size = x.d_size + qsl;
    t_quad* data = new t_quad[size+1]; rstdat (size+1, data);
    // initialize the carry
    t_octa cq = nilq;
    // shift the data
    for (long i = 0; i < x.d_size; i++) {
      // compute shift
      t_octa xo = x.p_data[i];
      t_octa ro = (xo << bsl) | cq;
      // set data and carry
      data[i+qsl] = (t_quad) ro;
      cq = ro >> 32;
    }
    // set the last quad
    data[size] = cq;
    // build result
    return new s_mpi (data, size+1);
  }

  // shift right a mpi by a certain amount
  static s_mpi* mpi_shr (const s_mpi& x, const long asr) {
    // fix trivial case
    if (asr == 0) return new s_mpi (x);
    // compute the amount of quad and bit shift
    long qsl = asr >> BIT_QUAD_SHIX;
    long bsl = asr % 32;
    t_quad* data = new t_quad[x.d_size]; rstdat (x.d_size, data);
    // initialize the borrow
    t_octa bq = nilq;
    // shift the data
    for (long i = x.d_size - 1; i >= 0; i--) {
      // compute shift
      t_octa xo = ((t_octa) x.p_data[i]) << 32;
      t_octa ro = (xo >> bsl) | (bq << 32);
      // set data and carry
      bq = ro & MAX_OCTA_UVAL;
      long j = i - qsl;
      if (j >= 0) data[j] = ro >> 32;
    }
    // build result
    return new s_mpi (data,x.d_size);
  }

  // forward definitions of in place operations
  static void mpi_aeq (s_mpi& x, const s_mpi& y);
  static void mpi_seq (s_mpi& x, const s_mpi& y);
  static void mpi_meq (s_mpi& x, const s_mpi& y);
  static void mpi_deq (s_mpi& x, const s_mpi& y, const bool rflg);

  // this function increments a mpi
  static s_mpi* mpi_inc (const s_mpi& x) {
    // compute result size and allocate
    long    size = x.d_size;
    long    dsiz = size + 1;
    t_quad* data = new t_quad[dsiz];
    t_quad    cq = nilq;
    // loop and add
    for (long i = 0; i < x.d_size; i++) {
      t_octa xo = (i < x.d_size) ? x.p_data[i] : 0;
      t_octa yo = (i == 0) ? MPI_OCTA_PONE : 0;
      t_octa ro = xo + yo + cq;
      data[i]   = (t_quad) ro;
      cq        = (t_quad) (ro >> 32);
    }
    // fix the carry
    data[size] = cq;
    // here is the result
    return new s_mpi (data, dsiz);
  }

  // this function decrements a mpi
  static s_mpi* mpi_dec (const s_mpi& x) {
    // compute result size and allocate
    t_quad* data = new t_quad[x.d_size];
    t_quad    cq = nilq;
    // loop and substract
    for (long i = 0; i < x.d_size; i++) {
      t_octa xo = x.p_data[i];
      t_octa yo = (i == 0) ? MPI_OCTA_PONE : 0;
      t_octa ro = xo - yo - cq;
      data[i]   = (t_quad) ro;
      cq        = (t_quad) (ro >> 63);
    }
    // here is the result
    return new s_mpi (data, x.d_size);
  }

  // this function adds a mpi with an octa 
  static s_mpi* mpi_add (const s_mpi& x, const t_octa y) {
    // compute result size and allocate
    long    size = max (x.d_size, 2);
    long    dsiz = size + 1;
    t_quad* data = new t_quad[dsiz];
    t_quad    cq = nilq;
    // loop and add
    for (long i = 0; i < size; i++) {
      t_octa xo = (i < x.d_size) ? x.p_data[i] : 0;
      t_octa yo = (i == 0) ? y & MPI_QUAD_MASK : (i == 1) ? y >> 32 : 0;
      t_octa ro = xo + yo + cq;
      data[i]   = (t_quad) ro;
      cq        = (t_quad) (ro >> 32);
    }
    // fix the carry
    data[size] = cq;
    // here is the result
    return new s_mpi (data,dsiz);
  }

  // this function adds two mpi values
  static s_mpi* mpi_add (const s_mpi& x, const s_mpi& y) {
    // compute result size and allocate
    long    size = max (x.d_size, y.d_size);
    long    dsiz = size + 1;
    t_quad* data = new t_quad[dsiz];
    t_quad    cq = nilq;
    // loop and add
    for (long i = 0; i < size; i++) {
      t_octa xo = (i < x.d_size) ? x.p_data[i] : 0;
      t_octa yo = (i < y.d_size) ? y.p_data[i] : 0;
      t_octa ro = xo + yo + cq;
      data[i]   = (t_quad) ro;
      cq        = (t_quad) (ro >> 32);
    }
    // fix the carry
    data[size] = cq;
    // here is the result
    return new s_mpi (data,dsiz);
  }

  // this function substracts a mpi value with an octa assuming x > y
  static s_mpi* mpi_sub (const s_mpi& x, const t_octa y) {
    // compute result size and allocate
    t_quad* data = new t_quad[x.d_size];
    t_quad    cq = nilq;
    // loop and substract
    for (long i = 0; i < x.d_size; i++) {
      t_octa xo = x.p_data[i];
      t_octa yo = (i == 0) ? y & MPI_QUAD_MASK : (i == 1) ? y >> 32 : 0ULL;
      t_octa ro = xo - yo - cq;
      data[i]   = (t_quad) ro;
      cq        = (t_quad) (ro >> 63);
    }
    // here is the result
    return new s_mpi (data, x.d_size);
  }

  // this function substracts an octa value with a mpi assuming x > y
  static s_mpi* mpi_sub (const t_octa x, const s_mpi& y) {
    // double check the size
    if (y.d_size > 2) {
      throw Exception ("internal-error", "mpi argument size bigger than 2");
    }
    // compute result size and allocate
    t_quad* data = new t_quad[2];
    // build octa argument
    t_octa yq = y.p_data[0];
    if (y.d_size > 1) yq += ((t_octa) y.p_data[1]) << 32;
    // evaluate result and set
    t_octa ro = x - yq;
    data[0] = ro;
    data[1] = ro >> 32;
    // here is the result
    return new s_mpi (data, 2);
  }

  // this function substracts two mpi values assuming x > y
  static s_mpi* mpi_sub (const s_mpi& x, const s_mpi& y) {
    // compute result size and allocate
    t_quad* data = new t_quad[x.d_size];
    t_quad    cq = nilq;
    // loop and substract
    for (long i = 0; i < x.d_size; i++) {
      t_octa xo = x.p_data[i];
      t_octa yo = (i < y.d_size) ? y.p_data[i] : 0;
      t_octa ro = xo - yo - cq;
      data[i]   = (t_quad) ro;
      cq        = (t_quad) (ro >> 63);
    }
    // here is the result
    return new s_mpi (data, x.d_size);
  }

  // this function multiply a mpi with a quad
  static s_mpi* mpi_mul (const s_mpi& x, const t_quad y) {
    // compute result size and allocate
    long    size = x.d_size + 1;
    t_quad* data = new t_quad[size];
    // reset carry
    t_quad cq = nilq;
    // loop in the first argument
    for (long j = 0; j < x.d_size; j++) {
      // compute local multiplication
      t_octa xo = x.p_data[j];
      t_octa ro = xo * y + cq;
      // adjust result
      data[j] = (t_quad) ro;
      // adjust carry
      cq = (t_quad) (ro >> 32);
    }
    data[x.d_size] = cq;
    // here is the result
    return new s_mpi (data,size);
  }

  // this function multiply a mpi with an octa
  static s_mpi* mpi_mul (const s_mpi& x, const t_octa y) {
    // compute result size and allocate
    long    size = x.d_size + 2;
    t_quad* data = new t_quad[size]; rstdat (size, data);
    // loop in the second argument
    for (long i = 0; i < 2; i++) {
      // reset carry
      t_quad cq = nilq;
      // loop in the first argument
      for (long j = 0; j < x.d_size; j++) {
	// initialize line arguments
	t_octa ro = data[i+j];
	t_octa xo = x.p_data[j];
	t_octa yo = (i == 0) ? y & MPI_QUAD_MASK : (i == 1) ? y >> 32 : 0;
	// compute local multiplication
	ro += xo * yo + cq;
	// adjust result
	data[i+j] = (t_quad) ro;
	// adjust carry
	cq = (t_quad) (ro >> 32);
      }
      data[i+x.d_size] = cq;
    }
    // here is the result
    return new s_mpi (data, size);
  }

  // this function multiply add equal a mpi with an octa
  static void mpi_mae (s_mpi& z, const s_mpi& x, const t_octa y) {
    // compute result size and allocate
    long size = max (z.d_size + 1, x.d_size + 2); z.grow(size);
    // loop in the second argument
    for (long i = 0; i < 2; i++) {
      // reset carry
      t_quad cq = nilq;
      // loop in the first argument
      for (long j = 0; j < size; j++) {
	// initialize line arguments
	t_octa ro = z.p_data[i+j];
	t_octa xo = (j < x.d_size) ? x.p_data[j] : nilo;
	t_octa yo = (i == 0) ? y & MPI_QUAD_MASK : (i == 1) ? y >> 32 : 0;
	// compute local multiplication
	ro += xo * yo + cq;
	// adjust result
	z.p_data[i+j] = (t_quad) ro;
	// adjust carry
	cq = (t_quad) (ro >> 32);
      }
      z.p_data[i+x.d_size] += cq;
    }
  }

  // this function multiply two mpi values
  static s_mpi* mpi_mul (const s_mpi& x, const s_mpi& y) {
    // compute result size and allocate
    long    size = x.d_size + y.d_size;
    t_quad* data = new t_quad[size]; rstdat (size, data);
    // loop in the second argument
    for (long i = 0; i < y.d_size; i++) {
      // reset carry
      t_quad cq = nilq;
      // loop in the first argument
      for (long j = 0; j < x.d_size; j++) {
	// initialize line arguments
	t_octa ro = data[i+j];
	t_octa xo = x.p_data[j];
	t_octa yo = y.p_data[i];
	// compute local multiplication
	ro += xo * yo + cq;
	// adjust result
	data[i+j] = (t_quad) ro;
	// adjust carry
	cq = (t_quad) (ro >> 32);
      }
      data[i+x.d_size] = cq;
    }
    // here is the result
    return new s_mpi (data, size);
  }

  // this function divide two mpi values and return the result or the
  // reminder depending on the reminder flag
  // this function is based on the algorithm described in HAC 14.20 and
  // incorporates numerous fixes to the sloppy description
  // this function also assumes that x is bigger than y and y is not null
  static s_mpi* mpi_div (const s_mpi& x, const s_mpi& y, const bool rflg) {
    // normalize x and y in order to have y > b/2
    long dbs = y.getmsb () % 32;
    if (dbs > 0) dbs = 32 - dbs;
    
    // allocate both dividend and divisor
    s_mpi* dx = (dbs == 0) ? new s_mpi (x) : mpi_shl (x, dbs);
    s_mpi* dy = (dbs == 0) ? new s_mpi (y) : mpi_shl (y, dbs);
    dx->clamp ();
    dy->clamp ();

    // allocate the quotient
    long delta = dx->d_size - dy->d_size;
    if (delta < 0) {
      delete dx;
      delete dy;
      throw Exception ("size-error", "invalid negative quotient size");
    }
    long qsize = delta + 1;    
    t_quad* qdata = new t_quad[qsize]; rstdat (qsize, qdata);
    
    // step 2:
    // py = y*b**n-t
    s_mpi* py = mpi_shl (*dy, delta*32);
    py->clamp ();
    // while (dx >= py) {q[n-t] += 1; dx-= py}
    while (mpi_geq (*dx, *py) == true) {
      qdata[delta]++;
      mpi_seq (*dx, *py);
    }
    dx->clamp ();
    delete py;

    // step 3: local variables
    // n = dx->d_size - 1;
    // t = dy->d_size - 1;
    long n = dx->d_size - 1;
    long t = dy->d_size - 1;
    // temporary tx
    t_quad* txd = new t_quad[3];
    s_mpi*   tx = new s_mpi (txd, 3);
    // ty = y[t]*b + y[t-1]
    t_quad* tyd = new t_quad[2];
    tyd[0] = (t == 0) ? nilq : dy->p_data[t-1];
    tyd[1] = dy->p_data[t];
    s_mpi* ty = new s_mpi (tyd, 2);

    // step 3: main loop
    for (long i = n; i >= dy->d_size; i--) {
      // step 3.1:
      // if (dx[i] == y[t]) (q[i-t-1] = b-1) (q[i-t-1] = x[i]*b+x[i-1]/y[t]
      if (dx->p_data[i] == dy->p_data[t]) {
	qdata[i-t-1] = MPI_QUAD_PMAX;
      } else {
	t_octa ox = dx->p_data[i]; ox <<= 32;
	ox |= dx->p_data[i-1];
	ox /= dy->p_data[t];
	qdata[i-t-1] = (t_quad) ox;
      }
      // step 3.2:
      // while(q[i-t-1]*ty) > (x[i]*b**2+x[i-1]*b+x[i-2])) (q[i-t-1]--)
      qdata[i-t-1]++;
      s_mpi* tq = nullptr;
      do {
	// fix back quotient
	qdata[i-t-1]--;
	// q[i-t-1]*ty
	delete tq; tq = nullptr;
	tq = mpi_mul (*ty, qdata[i-t-1]);
	// x[i]*b**2+x[i-1]*b+x[i-2]
	tx->p_data[2] = dx->p_data[i];
	tx->p_data[1] = ((i - 1) < 0) ? nilq : dx->p_data[i-1];
	tx->p_data[0] = ((i - 2) < 0) ? nilq : dx->p_data[i-2];
      } while (mpi_gth (*tq, *tx) == true);
      // step 3.3/3.4:
      // x -= q[i-t-1]*y*b**(i-t-1)
      // if (x < 0) {x += y*b**(i-t-1); q[i-t-1]--}
      long  ybs = (i - t - 1) * 32;
      s_mpi* tb = mpi_shl (*dy, ybs);
      delete tq; tq = mpi_mul (*tb, qdata[i-t-1]);
      if (mpi_geq (*dx, *tq) == true) {
	mpi_seq (*dx, *tq);
      } else {
	mpi_aeq (*dx, *tb);
	mpi_seq (*dx, *tq);
	qdata[i-t-1]--;
      }
      delete tb; tb = nullptr;
      delete tq; tq = nullptr;
    }
    // prepare result based on flag
    // eventually normalize back the reminder
    s_mpi* result = rflg ? mpi_shr (*dx, dbs) : new s_mpi (qdata, qsize);
    // clean temporary
    delete tx;
    delete ty;
    delete dx;
    delete dy;
    if (rflg == true) delete [] qdata;
    // finally here it is
    return result;
  }

  // this function computes the exponentiation of a mpi
  static s_mpi* mpi_pow (const s_mpi& x, const t_octa e) {
    // trivial case first
    if (e == nilq) return new s_mpi (1);
    // initialize temporaries
    s_mpi* ta = new s_mpi (1);
    s_mpi  ts = x;
    t_octa te = e;
    // main loop
    while (te != nilq) {
      if ((te & MPI_OCTA_PONE) == MPI_OCTA_PONE) mpi_meq (*ta, ts);
      te >>= 1;
      if (te != nilq) mpi_meq (ts, ts);
    }
    return ta;
  }

  // this function computes the exponentiation of a mpi
  static s_mpi* mpi_pow (const s_mpi& x, const s_mpi& e) {
    // trivial case first
    if (e.iszero () == true) return new s_mpi (1);
    // initialize temporaries
    s_mpi* ta = new s_mpi (1);
    s_mpi  ts = x;
    s_mpi  te = e;
    // main loop
    while (te.iszero () == false) {
      if (te.isodd () == true) mpi_meq (*ta, ts);
      mpi_req (te, 1);
      if (te.iszero () == false) mpi_meq (ts, ts);
    }
    return ta;
  }

  // this function computes the modular exponentiation of a mpi
  static s_mpi* mpi_pow (const s_mpi& x, const t_octa e, const s_mpi& m) {
    // trivial case first
    if (e == nilq) return new s_mpi (1);
    // initialize temporaries
    s_mpi* ta = new s_mpi (1);
    s_mpi  ts = x;
    t_octa te = e;
    // main loop
    while (te != nilq) {
      if ((te & MPI_OCTA_PONE) == MPI_OCTA_PONE) {
	mpi_meq (*ta, ts); ta->clamp ();
	if (mpi_geq (*ta, m) == true) {
	  mpi_deq (*ta, m, true); 
	  ta->clamp ();
	}
      }
      te >>= 1;
      if (te != nilq) {
	mpi_meq (ts, ts); ts.clamp ();
	if (mpi_geq (ts, m) == true) {
	  mpi_deq (ts, m, true); 
	  ts.clamp ();
	}
      }
    }
    return ta;
  }

  // this function computes the modular exponentiation of a mpi
  static s_mpi* mpi_pow (const s_mpi& x, const s_mpi& e, const s_mpi& m) {
    // trivial case first
    if (e.iszero () == true) return new s_mpi (1);
    // initialize temporaries
    s_mpi* ta = new s_mpi (1);
    s_mpi  ts = x;
    s_mpi  te = e;
    // main loop
    while (te.iszero () == false) {
      if (te.isodd () == true) {
	mpi_meq (*ta, ts); ta->clamp ();
	if (mpi_geq (*ta, m) == true) {
	  mpi_deq (*ta, m, true);
	  ta->clamp ();
	}
      }
      mpi_req (te, 1);
      if (te.iszero () == false) {
	mpi_meq (ts, ts); ts.clamp ();
	if (mpi_geq (ts, m) == true) {
	  mpi_deq (ts, m, true); 
	  ts.clamp ();
	}
      }
    }
    return ta;
  }

  // this function adds in place two mpi values
  static void mpi_aeq (s_mpi& x, const s_mpi& y) {
    // compute size and eventually grow
    long size = max (x.d_size, y.d_size); x.grow (size);
    // loop and add
    t_quad cq = nilq;
    for (long i = 0; i < size; i++) {
      t_octa xo   = (i < x.d_size) ? x.p_data[i] : 0;
      t_octa yo   = (i < y.d_size) ? y.p_data[i] : 0;
      t_octa ro   = xo + yo + cq;
      x.p_data[i] = (t_quad) ro;
      cq          = (t_quad) (ro >> 32);
    }
    // fix the carry
    if (cq != nilq) {
      x.grow (x.d_size + 1);
      x.p_data[x.d_size-1] = cq;
    }
    // reset clamp flag
    x.d_cflg = false;
  }

  // this function substracts in place two mpi assuming that x > y
  static void mpi_seq (s_mpi& x, const s_mpi& y) {
    // preset the carry
    t_quad cq = nilq;
    for (long i = 0; i < x.d_size; i++) {
      t_octa xo   = x.p_data[i];
      t_octa yo   = (i < y.d_size) ? y.p_data[i] : nilo;
      t_octa ro   = xo - yo - cq;
      x.p_data[i] = (t_quad) ro;
      cq = ((ro & 0x8000000000000000) == nilo) ? nilq : 0x00000001U;
    }
    // reset clamp flag
    x.d_cflg = false;
  }

  // this function multiply in place two mpi
  static void mpi_meq (s_mpi& x, const s_mpi& y) {
    // compute multiplication
    s_mpi* mpi = mpi_mul (x, y);
    // clean old data
    delete [] x.p_data;
    x.d_size = mpi->d_size;
    x.p_data = mpi->p_data;
    x.d_cflg = false;
    // clean temporary mpi
    mpi->p_data = nullptr;
    delete mpi;
  }

  // this function divides in place two mpi
  static void mpi_deq (s_mpi& x, const s_mpi& y, const bool rflg) {
    // compute multiplication
    s_mpi* mpi = mpi_div (x, y, rflg);
    // clean old data
    delete [] x.p_data;
    x.d_size = mpi->d_size;
    x.p_data = mpi->p_data;
    x.d_cflg = false;
    // clean temporary mpi
    mpi->p_data = nullptr;
    delete mpi;
  }

  // this function bitwise negate a mpi
  static s_mpi* mpi_neg (const s_mpi& x) {
    // build the result data
    long    size = x.d_size;
    t_quad* data = new t_quad[size];
    // negate the data
    for (long i = 0; i < size; i++) data[i] = ~x.p_data[i];
    // build the result
    return new s_mpi (data, size);
  }

  // this function bitwise and two mpi
  static s_mpi* mpi_and (const s_mpi& x, const s_mpi& y) {
    // build the result data
    long    size = max (x.d_size, y.d_size);
    t_quad* data = new t_quad[size];
    // and the data
    for (long i = 0; i < size; i++) {
      t_quad xq = (i < x.d_size) ? x.p_data[i] : nilq;
      t_quad yq = (i < y.d_size) ? y.p_data[i] : nilq;
      data[i] = xq & yq;
    }
    // build the result
    return new s_mpi (data, size);
  }

  // this function bitwise or two mpi
  static s_mpi* mpi_bor (const s_mpi& x, const s_mpi& y) {
    // build the result data
    long    size = max (x.d_size, y.d_size);
    t_quad* data = new t_quad[size];
    // and the data
    for (long i = 0; i < size; i++) {
      t_quad xq = (i < x.d_size) ? x.p_data[i] : nilq;
      t_quad yq = (i < y.d_size) ? y.p_data[i] : nilq;
      data[i] = xq | yq;
    }
    // build the result
    return new s_mpi (data, size);
  }

  // this function bitwise xor two mpi
  static s_mpi* mpi_xor (const s_mpi& x, const s_mpi& y) {
    // build the result data
    long    size = max (x.d_size, y.d_size);
    t_quad* data = new t_quad[size];
    // and the data
    for (long i = 0; i < size; i++) {
      t_quad xq = (i < x.d_size) ? x.p_data[i] : nilq;
      t_quad yq = (i < y.d_size) ? y.p_data[i] : nilq;
      data[i] = xq ^ yq;
    }
    // build the result
    return new s_mpi (data, size);
  }

  // this function computes the binary gcd of two mpi
  static s_mpi* mpi_gcd (const s_mpi& x, const s_mpi& y) {
    // initialize temporaries
    s_mpi a = x;
    s_mpi b = y;
    s_mpi g = 1ULL;
    // simplify by two and update even coefficient
    while ((a.iseven () == true) && (b.iseven () == true)) {
      mpi_req (a, 1);
      mpi_req (b, 1);
      mpi_leq (g, 1);
    }
    // clamp before looping
    a.clamp ();
    b.clamp ();
    // main loop
    while (a.iszero () == false) {
      while (a.iseven () == true) mpi_req (a, 1);
      while (b.iseven () == true) mpi_req (b, 1);
      if (mpi_geq (a, b) == true) {
	s_mpi t = a; 
	mpi_seq (t, b);
	mpi_req (t, 1);
	a = t;
      } else {
	s_mpi t = b; 
	mpi_seq (t, a);
	mpi_req (t, 1);
	b = t;
      }
    }
    // here it is
    return mpi_mul (b, g);
  }

  // this procedure compute the initial rho factor (also called m')
  // suck like m'=-1/m (mod b) - where b is the base radix
  // the fast algorithm here is from Tom Saint-Denis
  static t_quad mpi_rho (const s_mpi& m) {
    // get first digit
    t_quad b = m.p_data[0];
    // x*a==1 (mod 2**4)
    t_quad x = (((b + 2) & 4) << 1) + b;
    // x*a==1 (mod 2**8)
    x *=  2 - (b * x);
    // x*a==1 (mod 2**16)
    x *=  2 - (b * x);
    // x*a==1 (mod 2**32)
    x *=  2 - (b * x);
    // rho = -1/m (mod b)
    return (t_quad) (MPI_OCTA_HONE - x);
  }

  // this procedure compute a radix-based montgomery multiplication
  // of two mpi without any verification (HAC 14.36)
  // carefull: it is assumed here that m is odd and x,y < m
  static s_mpi* mpi_mmm (const s_mpi& x, const s_mpi& y, const s_mpi& m,
			 const t_quad rho) {
    // initialize result
    s_mpi* z = new s_mpi;
    // get modular size and loop
    long k = m.vsize ();
    for (long i = 0; i < k; i++) {
      // ui = ((zo + xi.y0)*rho) mod b
      t_octa z0 = z->p_data[0];
      t_quad xi = (i < x.d_size) ? x.p_data[i] : nilq;
      t_octa y0 = y.p_data[0];
      t_quad ui = (z0 + (y0 * xi)) * rho;
      // z += xi.y
      mpi_mae (*z, y, xi);
      // z += ui.m
      mpi_mae (*z, m, ui);
      // divide one radix and clamp
      z->rsr (1);
    }
    if (mpi_geq (*z, m) == true) mpi_seq (*z, m);
    z->clamp ();
    return z;
  }

  // this procedure computes a montgomery modular transformation of a mpi
  // without any verification
  // carefull: it is assumed here that m is odd and x < m
  static s_mpi* mpi_mmt (const s_mpi& x, const s_mpi& m, const t_quad rho) {
    // get the modular size
    long k = m.vsize (); 
    // compute the exponent r**2 mod m
    s_mpi* y = new s_mpi (2*k+1, 1UL);
    mpi_deq (*y, m, true);
    // compute the montgomery multiplication
    s_mpi* z = mpi_mmm (x, *y, m, rho);
    delete y;
    return z;
  }

  // this procedure computes a montgomery inverse transformation of a mpi
  // without any verification
  // carefull: it is assumed here that m is odd and x < m
  static s_mpi* mpi_mit (const s_mpi& x, const s_mpi& m, const t_quad rho) {
    return mpi_mmm (x, 1, m, rho);
  }

  // this procedure computes a montgomery modular exponentiation of a mpi
  // without any verification
  // carefull: it is assumed here that m is odd and x < m
  static s_mpi* mpi_mmp (const s_mpi& x, const s_mpi& e, const s_mpi& m, 
			 const t_quad rho) {
    // initialize temporaries
    s_mpi* tr = mpi_mmt (1, m, rho);
    long   em = e.getmsb () - 1;
    for (long i = em; i >= 0; i--) {
      // compute tr * tr
      s_mpi* tz = mpi_mmm (*tr, *tr, m, rho);
      *tr = *tz; delete tz;
      // check for bit set
      if (e.getbit (i) == true) {
	tz = mpi_mmm (*tr, x, m, rho);
	*tr = *tz; delete tz;
      }
    }
    tr->clamp();
    return tr;
  }

  // this procedure computes a montgomery modular exponentiation of a mpi
  // but unlike the other montgomery procedures, this one does the required 
  // verifications
  static s_mpi* mpi_mme (const s_mpi& x, const s_mpi& e, const s_mpi& m) {
    // verify first that m is odd
    if (m.isodd () == false) {
      throw Exception ("internal-error", 
		       "montgomery exponentiation called with even modulus");
    }
    // get the modulus rho factor
    t_quad rho = mpi_rho (m);
    // initialize and normalize x
    s_mpi tx = x;
    if (mpi_geq (tx, m) == true) mpi_deq (tx, m, true);
    // transform tx in the montgomery space
    s_mpi* mx = mpi_mmt (tx, m, rho);
    // compute the exponentiation
    s_mpi* mz = mpi_mmp (*mx, e, m, rho);
    // reverse the result
    s_mpi*  z = mpi_mit (*mz, m, rho);
    // clean stuff and return
    delete mx;
    delete mz;
    return z;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // compute the gcd of two relatifs

  Relatif Relatif::gcd (const Relatif& x, const Relatif& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      // compute the gcd
      s_mpi* mpi = mpi_gcd (*x.p_mpi, *y.p_mpi);
      mpi->clamp ();
      // build the result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      result.p_mpi = mpi;
      // unlock and return;
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // compute the lcm of two relatifs

  Relatif Relatif::lcm (const Relatif& x, const Relatif& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      // compute result
      Relatif result = (x * y) / Relatif::gcd (x, y);
      result.d_sgn = false;
      result.p_mpi->clamp ();
      // unlock and return;
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // compute the mmi of a relatif

  Relatif Relatif::mmi (const Relatif& x, const Relatif& m) {
    // check for existence
    if ((x.iseven () == true) && (m.iseven () == true)) return 0;
    // initialize temporaries
    Relatif u = x;
    Relatif v = m;
    Relatif a = 1;
    Relatif b = 0;
    Relatif c = 0;
    Relatif d = 1;
    // main loop
    while (u.iszero () == false) {
      // process u division
      while (u.iseven () == true) {
	u >>= 1;
	if ((a.iseven () == true) && (b.iseven () == true)) {
	  a >>= 1;
	  b >>= 1;
	} else {
	  a = (a + m) >> 1;
	  b = (b - x) >> 1;
	}
      }
      // process v division
      while (v.iseven () == true) {
	v >>= 1;
	if ((c.iseven () == true) && (d.iseven () == true)) {
	  c >>= 1;
	  d >>= 1;
	} else {
	  c = (c + m) >> 1;
	  d = (d - x) >> 1;
	}
      }
      // adjust results
      if (u >= v) {
	u = u - v;
	a = a - c;
	b = b - d;
      } else {
	v = v - u;
	c = c - a;
	d = d - b;
      }
    }
    if (u.iszero () == true) {
      a = c;
      b = d;
    }
    // check for gcd
    if (v != 1) return 0;
    // process negative
    if (a < 0) return m + (a % m);
    return a % m;
  }

  // compute an optimized modular exponentiation

  Relatif Relatif::mme (const Relatif& x, const Relatif& e, const Relatif& m) {
    // check for negative exponent
    e.rdlock ();
    if (e.d_sgn == true) {
      e.unlock ();
      throw Exception ("number-error", "invalid negative exponent in pow");
    }
    // check for negative modulo
    m.rdlock ();
    if (m.d_sgn == true) {
      e.unlock ();
      m.unlock ();
      throw Exception ("number-error", "invalid negative modulo in pow");
    }
    // process trivial case
    if (e == 1) {
      e.unlock ();
      m.unlock ();
      return x % m;
    }
    // lock argument and prepare result
    x.rdlock ();
    Relatif result;
    try {
      // use the montgomery exponentiation if m is odd
      if (m.isodd () == true) {
	// compute result
	delete result.p_mpi; result.p_mpi = nullptr;
	result.p_mpi = mpi_mme (*x.p_mpi, *e.p_mpi, *m.p_mpi);
	// compute sign
	result.d_sgn = (e.iseven () == true) ? false : x.d_sgn;
	// clamp the result
	if (result.p_mpi->clamp () == true) result.d_sgn = false;
      } else {
	result = x.pow (e, m);
      }
      x.unlock ();
      e.unlock ();
      m.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      e.unlock ();
      m.unlock ();
      throw;
    }
  }

  // generate a random n bits relatif number

  Relatif Relatif::random (const long bits) {
    // check for valid size
    if (bits < 0) {
      throw Exception ("size-error", "invalid random number size");
    }
    // build mpi result
    Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
    result.p_mpi = mpi_rnd (bits);
    // here is the number
    return result;
  }

  // generate a random n bits odd or even relatif number

  Relatif Relatif::random (const long bits, const bool oddf) {
    // generate a random number
    Relatif result = Relatif::random (bits);
    // fix parity
    result.setbit (0, oddf);
    // here is the number
    return result;
  }

  // generate a random number upto a maximum

  Relatif Relatif::random (const Relatif& mrn) {
    // check for sign
    mrn.rdlock ();
    try {
      if (mrn.d_sgn == true) {
	throw Exception ("sign-error", "invalid maximum negative number");
      }
      // get the max number of bits
      long bits = mrn.p_mpi->getmsb ();
      // build mpi result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      result.p_mpi = mpi_rng (bits);
      if (mpi_geq (*result.p_mpi, *mrn.p_mpi) == true) {
	mpi_req (*result.p_mpi, 1);
	result.p_mpi->clamp ();
      }
      mrn.unlock ();
      // here is the number
      return result;
    } catch (...) {
      mrn.unlock ();
      throw;
    }
  }
  
  // change the sign of a relatif

  Relatif operator - (const Relatif& x) {
    x.rdlock ();
    try {
      // create an empty result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      // compute the result
      result.d_sgn = !x.d_sgn;
      result.p_mpi = new s_mpi (*x.p_mpi);
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      // unlock and return
      x.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      throw;
    }
  }

  // add a relatif with an integer

  Relatif operator + (const Relatif& x, const t_long y) {
    x.rdlock ();
    try {
      // create an empty result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      // prepare y data
      bool   ysgn = (y < 0);
      t_octa yval = ysgn ? -y : y;
      // compute the type of operation to do
      bool sflg = x.d_sgn ^ ysgn;
      // compute the result
      if (sflg == false) {
	result.p_mpi = mpi_add (*x.p_mpi, yval);
	result.d_sgn = x.d_sgn;
      } else {
	bool xflg = mpi_geq (*x.p_mpi, yval);
	if (xflg == true) {
	  result.p_mpi = mpi_sub (*x.p_mpi, yval);
	  result.d_sgn = x.d_sgn;
	} else {
	  result.p_mpi = mpi_sub (yval, *x.p_mpi);
	  result.d_sgn = ysgn;
	}
      }
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      // unlock and return
      x.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      throw;
    }
  }

  // add two relatifs

  Relatif operator + (const Relatif& x, const Relatif& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      // create an empty result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      // compute the type of operation to do
      bool sflg = x.d_sgn ^ y.d_sgn;
      // compute the result
      if (sflg == false) {
	result.p_mpi = mpi_add (*x.p_mpi, *y.p_mpi);
	result.d_sgn = x.d_sgn;
      } else {
	bool xflg = mpi_geq (*x.p_mpi, *y.p_mpi);
	if (xflg == true) {
	  result.p_mpi = mpi_sub (*x.p_mpi, *y.p_mpi);
	  result.d_sgn = x.d_sgn;
	} else {
	  result.p_mpi = mpi_sub (*y.p_mpi, *x.p_mpi);
	  result.d_sgn = y.d_sgn;
	}
      }
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      // unlock and return
      y.unlock ();
      x.unlock ();
      return result;
    } catch (...) {
      y.unlock ();
      x.unlock ();
      throw;
    }
  }

  // substract a relatif with an integer

  Relatif operator - (const Relatif& x, const t_long y) {
    x.rdlock ();
    try {
      // create an empty result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      // prepare y data
      bool   ysgn = (y < 0);
      t_octa yval = ysgn ? -y : y;
      // compute the type of operation to do
      bool sflg = x.d_sgn ^ ysgn;
      // compute the result
      if (sflg == true) {
	result.p_mpi = mpi_add (*x.p_mpi, yval);
	result.d_sgn = x.d_sgn;
      } else {
	bool xflg = mpi_geq (*x.p_mpi, yval);
	if (xflg == true) {
	  result.p_mpi = mpi_sub (*x.p_mpi, yval);
	  result.d_sgn = x.d_sgn;
	} else {
	  result.p_mpi = mpi_sub (yval, *x.p_mpi);
	  result.d_sgn = !x.d_sgn;
	}
      }
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      // unlock and return
      x.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      throw;
    }
  }

  // substract two relatifs

  Relatif operator - (const Relatif& x, const Relatif& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      // create an empty result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      // compute the type of operation to do
      bool sflg = x.d_sgn ^ y.d_sgn;
      // compute the result
      if (sflg == true) {
	result.p_mpi = mpi_add (*x.p_mpi, *y.p_mpi);
	result.d_sgn = x.d_sgn;
      } else {
	bool xflg = mpi_geq (*x.p_mpi, *y.p_mpi);
	if (xflg == true) {
	  result.p_mpi = mpi_sub (*x.p_mpi, *y.p_mpi);
	  result.d_sgn = x.d_sgn;
	} else {
	  result.p_mpi = mpi_sub (*y.p_mpi, *x.p_mpi);
	  result.d_sgn = !x.d_sgn;
	}
      }
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      // unlock and return
      y.unlock ();
      x.unlock ();
      return result;
    } catch (...) {
      y.unlock ();
      x.unlock ();
      throw;
    }
  }

  // multiply a relatif with an integer

  Relatif operator * (const Relatif& x, const t_long y) {
    x.rdlock ();
    try {
      // create an empty result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      // prepare y data
      bool   ysgn = (y < 0);
      t_octa yval = ysgn ? -y : y;
      // compute the result
      result.d_sgn = x.d_sgn ^ ysgn;
      result.p_mpi = mpi_mul (*x.p_mpi, yval);
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      // unlock and return
      x.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      throw;
    }
  }

  // multiply two relatifs

  Relatif operator * (const Relatif& x, const Relatif& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      // create an empty result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      // compute the result
      result.d_sgn = x.d_sgn ^ y.d_sgn;
      result.p_mpi = mpi_mul (*x.p_mpi, *y.p_mpi);
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      // unlock and return
      y.unlock ();
      x.unlock ();
      return result;
    } catch (...) {
      y.unlock ();
      x.unlock ();
      throw;
    }
  }

  // divide a relatif with an integer

  Relatif operator / (const Relatif& x, const t_long y) {
    x.rdlock ();
    try {
      // check y value
      if (y == 0) {
	throw Exception ("relatif-error", "division by zero");
      }
      // prepare y data
      bool   ysgn = (y < 0);
      t_octa yval = ysgn ? -y : y;
      // create an empty result
      Relatif result; 
      // check for trivial case
      if (mpi_geq (*x.p_mpi, yval) == false) {
	x.unlock ();
	return result;
      }
      // clean result data
      delete result.p_mpi; result.p_mpi = nullptr;
      // compute the result
      result.d_sgn = x.d_sgn ^ ysgn;
      result.p_mpi = mpi_div (*x.p_mpi, yval, false);
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      // unlock and return
      x.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      throw;
    }
  }
  
  // divide two relatifs

  Relatif operator / (const Relatif& x, const Relatif& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      // check y value
      if (y == 0) {
	throw Exception ("relatif-error", "division by zero");
      }
      // create an empty result
      Relatif result;
      // check trivial case
      if (mpi_geq (*x.p_mpi, *y.p_mpi) == false) {
	y.unlock ();
	x.unlock ();
	return result;
      }
      // clean result data
      delete result.p_mpi; result.p_mpi = nullptr;
      // compute the result
      result.d_sgn = x.d_sgn ^ y.d_sgn;
      result.p_mpi = mpi_div (*x.p_mpi, *y.p_mpi, false);
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      // unlock and return
      y.unlock ();
      x.unlock ();
      return result;
    } catch (...) {
      y.unlock ();
      x.unlock ();
      throw;
    }
  }

  // compute the reminder of a relatif with an integer

  Relatif operator % (const Relatif& x, const t_long y) {
    x.rdlock ();
    try {
      // check y value
      if (y == 0) {
	throw Exception ("relatif-error", "division by zero");
      }
      // prepare y data
      bool   ysgn = (y < 0);
      t_octa yval = ysgn ? -y : y;
      // create an empty result
      Relatif result; 
      // check trivial case
      if (mpi_geq (*x.p_mpi, yval) == false) {
	result = x;
	x.unlock ();
	return result;
      }
      // clean result data;
      delete result.p_mpi; result.p_mpi = nullptr;
      // compute the result
      result.d_sgn = x.d_sgn;
      result.p_mpi = mpi_div (*x.p_mpi, yval, true);
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      // unlock and return
      x.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      throw;
    }
  }

  // compute the reminder of two relatifs

  Relatif operator % (const Relatif& x, const Relatif& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      // check y value
      if (y == 0) {
	throw Exception ("relatif-error", "division by zero");
      }
      // create an empty result
      Relatif result;
      // check trivial case
      if (mpi_geq (*x.p_mpi, *y.p_mpi) == false) {
	result = x;
	y.unlock ();
	x.unlock ();
	return result;
      }
      // clean result data
      delete result.p_mpi; result.p_mpi = nullptr;
      // compute the result
      result.d_sgn = x.d_sgn;
      result.p_mpi = mpi_div (*x.p_mpi, *y.p_mpi, true);
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      // unlock and return
      y.unlock ();
      x.unlock ();
      return result;
    } catch (...) {
      y.unlock ();
      x.unlock ();
      throw;
    }
  }

  // bitwise negate a relatif

  Relatif operator ~ (const Relatif& x) {
    x.rdlock ();
    try {
      // create an empty result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      // bind the sign
      result.d_sgn = x.d_sgn;
      // bind the mpi
      result.p_mpi = mpi_neg (*x.p_mpi);
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      // unlock and return
      x.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      throw;
    }
  }

  // bitwise and two relatifs

  Relatif operator & (const Relatif& x, const Relatif& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      // create an empty result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      // bind the sign
      result.d_sgn = x.d_sgn && y.d_sgn;
      // bind the mpi
      result.p_mpi = mpi_and (*x.p_mpi, *y.p_mpi);
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      // unlock and return
      y.unlock ();
      x.unlock ();
      return result;
    } catch (...) {
      y.unlock ();
      x.unlock ();
      throw;
    }
  }

  // bitwise or two relatifs

  Relatif operator | (const Relatif& x, const Relatif& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      // create an empty result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      // bind the sign
      result.d_sgn = x.d_sgn || y.d_sgn;
      // bind the mpi
      result.p_mpi = mpi_bor (*x.p_mpi, *y.p_mpi);
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      // unlock and return
      y.unlock ();
      x.unlock ();
      return result;
    } catch (...) {
      y.unlock ();
      x.unlock ();
      throw;
    }
  }

  // bitwise xor two relatifs

  Relatif operator ^ (const Relatif& x, const Relatif& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      // create an empty result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      // bind the sign
      result.d_sgn = x.d_sgn ^ y.d_sgn;
      // bind the mpi
      result.p_mpi = mpi_xor (*x.p_mpi, *y.p_mpi);
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      // unlock and return
      y.unlock ();
      x.unlock ();
      return result;
    } catch (...) {
      y.unlock ();
      x.unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default relatif

  Relatif::Relatif (void) {
    d_sgn = false;
    p_mpi = new s_mpi;
  }

  // create a relatif by value

  Relatif::Relatif (const t_long x) {
    // compute sign
    d_sgn = x < 0;
    // convert to unsigned
    t_octa data = d_sgn ? -x : x;
    // build mpi data
    p_mpi = new s_mpi (data);
  }

  // create a relatif from a string

  Relatif::Relatif (const String& s) {
    d_sgn = false;
    p_mpi = nullptr;
    *this = strtor (s);
  }

  // create a relatif from a byte array

  Relatif::Relatif (const t_byte* rbuf, const long size) {
    if (size < 0) {
      throw Exception ("size-error", "invalid relatif negative byte size");
    }
    if (size == 0) {
      d_sgn = false;
      p_mpi = new s_mpi;
    } else {
      d_sgn = false;
      p_mpi = new s_mpi (rbuf, size); p_mpi->clamp ();
    }
  }

  // create a relatif from a byte array in complemented form

  Relatif::Relatif (const t_byte* rbuf, const long size, const bool cflg) {
    if (size < 0) {
      throw Exception ("size-error", "invalid relatif negative byte size");
    }
    if (size == 0) {
      d_sgn = false;
      p_mpi = new s_mpi;
    } else {
      if (cflg == false) {
	d_sgn = false;
	p_mpi = new s_mpi (rbuf, size); p_mpi->clamp ();
      } else { 
	p_mpi = new s_mpi;
	d_sgn = p_mpi->setsbuf (rbuf, size); p_mpi->clamp ();
      }
    }
  }

  // copy construct this relatif

  Relatif::Relatif (const Relatif& that) {
    that.rdlock ();
    try {
      d_sgn = that.d_sgn;
      p_mpi = new s_mpi (*that.p_mpi);
      if (p_mpi->clamp () == true) d_sgn = false;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this relatif

  Relatif::Relatif (Relatif&& that) noexcept {
    that.wrlock ();
    try {
      // move base number
      Number::operator = (static_cast<Number&&>(that));
      // copy move locally
      d_sgn = that.d_sgn; that.d_sgn = false;
      p_mpi = that.p_mpi; that.p_mpi = nullptr;
      that.unlock ();
    } catch (...) {
      d_sgn = false;
      p_mpi = nullptr;
      that.unlock ();
    }
  }

  // destroy this relatif

  Relatif::~Relatif (void) {
    delete p_mpi;
  }

  // assign a relatif to this one

  Relatif& Relatif::operator = (const Relatif& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and check
    wrlock ();
    that.rdlock ();
    try {
      // clean old mpi
      delete p_mpi;
      // bin new mpi
      d_sgn = that.d_sgn;
      p_mpi = new s_mpi (*that.p_mpi);
      if (p_mpi->clamp () == true) d_sgn = false;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }

  //  move a relatif into this one

  Relatif& Relatif::operator = (Relatif&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and check
    wrlock ();
    that.wrlock ();
    try {
      // assign base object
      Number::operator = (static_cast<Number&&>(that));
      // assign locally
      d_sgn = that.d_sgn; that.d_sgn = false;
      p_mpi = that.p_mpi; that.p_mpi = nullptr;
      unlock ();
      that.unlock ();
    } catch (...) {
      d_sgn = false;
      p_mpi = nullptr;
      unlock ();
      that.unlock ();
    }
    return *this;
  }

  // add a relatif to this one

  Relatif& Relatif::operator += (const Relatif& x) {
    wrlock   ();
    x.rdlock ();
    try {
      // compute the type of operation to do
      bool sflg = d_sgn ^ x.d_sgn;
      // compute the result
      if (sflg == false) {
	s_mpi* mpi = mpi_add (*p_mpi, *x.p_mpi);
	delete p_mpi; p_mpi = mpi;
      } else {
	bool xflg = mpi_geq (*p_mpi, *x.p_mpi);
	if (xflg == true) {
	  s_mpi* mpi = mpi_sub (*p_mpi, *x.p_mpi);
	  delete p_mpi; p_mpi = mpi;
	} else {
	  s_mpi* mpi = mpi_sub (*x.p_mpi, *p_mpi);
	  delete p_mpi; p_mpi = mpi;
	  d_sgn = x.d_sgn;
	}
      }
      // clamp the result
      if (p_mpi->clamp () == true) d_sgn = false;
      // unlock and return
      unlock   ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock   ();
      x.unlock ();
      throw;
    }
  }

  // substract a relatif to this one

  Relatif& Relatif::operator -= (const Relatif& x) {
    wrlock   ();
    x.rdlock ();
    try {
      // compute the type of operation to do
      bool sflg = d_sgn ^ x.d_sgn;
      // compute the result
      if (sflg == true) {
	s_mpi* mpi = mpi_add (*p_mpi, *x.p_mpi);
	delete p_mpi; p_mpi = mpi;
      } else {
	bool xflg = mpi_geq (*p_mpi, *x.p_mpi);
	if (xflg == true) {
	  s_mpi* mpi = mpi_sub (*p_mpi, *x.p_mpi);
	  delete p_mpi; p_mpi = mpi;
	} else {
	  s_mpi* mpi = mpi_sub (*x.p_mpi, *p_mpi);
	  delete p_mpi; p_mpi = mpi;
	  d_sgn = !d_sgn;
	}
      }
      // clamp the result
      if (p_mpi->clamp () == true) d_sgn = false;
      // unlock and return
      unlock   ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock   ();
      x.unlock ();
      throw;
    }
  }

  // multiply a relatif with this one

  Relatif& Relatif::operator *= (const Relatif& x) {
    wrlock   ();
    x.rdlock ();
    try {
      // compute the result
      d_sgn = x.d_sgn ^ x.d_sgn;
      s_mpi* mpi = mpi_mul (*p_mpi, *x.p_mpi);
      delete p_mpi; p_mpi = mpi;
      // clamp the result
      if (p_mpi->clamp () == true) d_sgn = false;
      // unlock and return
      unlock ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock   ();
      x.unlock ();
      throw;
    }
  }

  // divide a relatif with this one

  Relatif& Relatif::operator /= (const Relatif& x) {
    wrlock   ();
    x.rdlock ();
    try {
      // compute the result
      d_sgn = x.d_sgn ^ x.d_sgn;
      s_mpi* mpi = mpi_div (*p_mpi, *x.p_mpi, false);
      delete p_mpi; p_mpi = mpi;
      // clamp the result
      if (p_mpi->clamp () == true) d_sgn = false;
      // unlock and return
      unlock ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock   ();
      x.unlock ();
      throw;
    }
  }

  // prefix increment this relatif

  Relatif& Relatif::operator ++ (void) {
    wrlock ();
    try {
      // check the sign first
      if (d_sgn == false) {
	s_mpi* mpi = mpi_inc (*p_mpi);
	delete p_mpi; p_mpi = mpi;
      } else {
	if (mpi_gth (*p_mpi, 1) == true) {
	  s_mpi* mpi = mpi_dec (*p_mpi);
	  delete p_mpi; p_mpi = mpi;
	} else {
	  delete p_mpi; p_mpi = new s_mpi;
	  d_sgn = false;
	}
      }
      // clamp the result
      if (p_mpi->clamp () == true) d_sgn = false;
      // unlock and return
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // prefix decrements this relatif

  Relatif& Relatif::operator -- (void) {
    wrlock ();
    try {
      // check the sign first
      if (d_sgn == true) {
	s_mpi* mpi = mpi_inc (*p_mpi);
	delete p_mpi; p_mpi = mpi;
      } else {
	if (mpi_gth (*p_mpi, 1) == true) {
	  s_mpi* mpi = mpi_dec (*p_mpi);
	  delete p_mpi; p_mpi = mpi;
	} else {
	  delete p_mpi; p_mpi = new s_mpi;
	  d_sgn = false;
	}
      }
      // clamp the result
      if (p_mpi->clamp () == true) d_sgn = false;
      // unlock and return
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // postfix increments this relatif

  Relatif Relatif::operator ++ (int) {
    wrlock ();
    try {
      // save the result
      Relatif result = *this;
      // check the sign first
      if (d_sgn == false) {
	s_mpi* mpi = mpi_inc (*p_mpi);
	delete p_mpi; p_mpi = mpi;
      } else {
	if (mpi_gth (*p_mpi, 1) == true) {
	  s_mpi* mpi = mpi_dec (*p_mpi);
	  delete p_mpi; p_mpi = mpi;
	} else {
	  delete p_mpi; p_mpi = new s_mpi;
	  d_sgn = false;
	}
      }
      // clamp the result
      if (p_mpi->clamp () == true) d_sgn = false;
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // postfix decrements this relatif

  Relatif Relatif::operator -- (int) {
    wrlock ();
    try {
      // save the result
      Relatif result = *this;
      // check the sign first
      if (d_sgn == true) {
	s_mpi* mpi = mpi_inc (*p_mpi);
	delete p_mpi; p_mpi = mpi;
      } else {
	if (mpi_gth (*p_mpi, 1) == true) {
	  s_mpi* mpi = mpi_dec (*p_mpi);
	  delete p_mpi; p_mpi = mpi;
	} else {
	  delete p_mpi; p_mpi = new s_mpi;
	  d_sgn = false;
	}
      }
      // clamp the result
      if (p_mpi->clamp () == true) d_sgn = false;
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // shift left this relatif by a certain amount

  Relatif Relatif::operator << (const long asl) const {
    // lock and compute
    rdlock ();
    try {
      // create an empty result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      // compute the shifted mpi
      result.d_sgn = d_sgn;
      if (asl >= 0) {
	result.p_mpi = mpi_shl (*p_mpi, asl);
      } else {
	result.p_mpi = mpi_shr (*p_mpi, -asl);
      }
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      //unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // shift right this relatif by a certain amount

  Relatif Relatif::operator >> (const long asr) const {
    // lock and compute
    rdlock ();
    try {
      // create an empty result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      // compute the shifted mpi
      result.d_sgn = d_sgn;
      if (asr >= 0) {
	result.p_mpi = mpi_shr (*p_mpi, asr);
      } else {
	result.p_mpi = mpi_shl (*p_mpi, -asr);
      }
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      //unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // shift left this relatif by a certain amount

  Relatif& Relatif::operator <<= (const long asl) {
    // lock and compute
    wrlock ();
    try {
      if (asl >= 0) {
	mpi_leq (*p_mpi, asl);
      } else {
	mpi_req (*p_mpi, -asl);
      }
      // clamp the result
      if (p_mpi->clamp () == true) d_sgn = false;
      //unlock and return
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // shift right this relatif by a certain amount

  Relatif& Relatif::operator >>= (const long asr) {
    // lock and compute
    wrlock ();
    try {
      if (asr >= 0) {
	mpi_req (*p_mpi, asr);
      } else {
	mpi_leq (*p_mpi, -asr);
      }
      // clamp the result
      if (p_mpi->clamp () == true) d_sgn = false;
      //unlock and return
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // compare a relatif with an integer

  bool Relatif::operator == (const t_long x) const {
    rdlock ();
    try {
      // prepare x data
      bool   xsgn = (x < 0);
      t_octa xval = xsgn ? -x : x;
      // check for sign
      bool result = (d_sgn == xsgn);
      // check for data equality
      if (result == true) result = mpi_eql (*p_mpi, xval);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare two relatifs

  bool Relatif::operator == (const Relatif& x) const {
    rdlock ();
    x.rdlock ();
    try {
      // check for sign
      bool result = (d_sgn == x.d_sgn);
      // check for data equality
      if (result == true) result = mpi_eql (*p_mpi, *x.p_mpi);
      // unlock and return
      x.unlock ();
      unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      unlock ();
      throw;
    }
  }

  // compare two relatifs

  bool Relatif::operator != (const t_long x) const {
    rdlock ();
    try {
      // prepare x data
      bool   xsgn = (x < 0);
      t_octa xval = xsgn ? -x : x;
      // check for sign
      bool result = (d_sgn == xsgn);
      // check for data equality
      if (result == true) result = mpi_eql (*p_mpi, xval);
      // unlock and return
      unlock ();
      return !result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare two relatifs

  bool Relatif::operator != (const Relatif& x) const {
    rdlock ();
    x.rdlock ();
    try {
      // check for sign
      bool result = (d_sgn == x.d_sgn);
      // check for data equality
      if (result == true) result = mpi_eql (*p_mpi, *x.p_mpi);
      // unlock and return
      x.unlock ();
      unlock ();
      return !result;
    } catch (...) {
      x.unlock ();
      unlock ();
      throw;
    }
  }

  // compare a relatif with an integer

  bool Relatif::operator < (const t_long x) const {
    rdlock ();
    try {
      // prepare x data
      bool   xsgn = (x < 0);
      t_octa xval = xsgn ? -x : x;
      // check against sign first
      if ((d_sgn == true)  && (xsgn == false)) {
	unlock ();
	return true;
      }
      if ((d_sgn == false) && (xsgn == true)){
	unlock ();
	return false;
      }
      // the number have the same sign
      bool result = !mpi_geq (*p_mpi, xval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare two relatifs

  bool Relatif::operator < (const Relatif& x) const {
    rdlock ();
    x.rdlock ();
    try {
      // check against sign first
      if ((d_sgn == true)  && (x.d_sgn == false)) {
	x.unlock ();
	unlock ();
	return true;
      }
      if ((d_sgn == false) && (x.d_sgn == true)){
	x.unlock ();
	unlock ();
	return false;
      }
      // the number have the same sign
      bool result = !mpi_geq (*p_mpi, *x.p_mpi);
      x.unlock ();
      unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      unlock ();
      throw;
    }
  }

  // compare a relatif with an integer

  bool Relatif::operator <= (const t_long x) const {
    rdlock ();
    try {
      // prepare x data
      bool   xsgn = (x < 0);
      t_octa xval = xsgn ? -x : x;
      // check against sign first
      if ((d_sgn == true)  && (xsgn == false)) {
	unlock ();
	return true;
      }
      if ((d_sgn == false) && (xsgn == true)){
	unlock ();
	return false;
      }
      // the number have the same sign
      bool result = !mpi_gth (*p_mpi, xval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare two relatifs

  bool Relatif::operator <= (const Relatif& x) const {
    rdlock ();
    x.rdlock ();
    try {
      // check against sign first
      if ((d_sgn == true)  && (x.d_sgn == false)) {
	x.unlock ();
	unlock ();
	return true;
      }
      if ((d_sgn == false) && (x.d_sgn == true)){
	x.unlock ();
	unlock ();
	return false;
      }
      // the number have the same sign
      bool result = !mpi_gth (*p_mpi, *x.p_mpi);
      x.unlock ();
      unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      unlock ();
      throw;
    }
  }

  // compare a relatif with an integer

  bool Relatif::operator > (const t_long x) const {
    rdlock ();
    try {
      // prepare x data
      bool   xsgn = (x < 0);
      t_octa xval = xsgn ? -x : x;
      // check against sign first
      if ((d_sgn == true)  && (xsgn == false)) {
	unlock ();
	return false;
      }
      if ((d_sgn == false) && (xsgn == true)){
	unlock ();
	return true;
      }
      // the number have the same sign
      bool result = mpi_gth (*p_mpi, xval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare two relatifs

  bool Relatif::operator > (const Relatif& x) const {
    rdlock ();
    x.rdlock ();
    try {
      // check against sign first
      if ((d_sgn == true)  && (x.d_sgn == false)) {
	x.unlock ();
	unlock ();
	return false;
      }
      if ((d_sgn == false) && (x.d_sgn == true)){
	x.unlock ();
	unlock ();
	return true;
      }
      // the number have the same sign
      bool result = mpi_gth (*p_mpi, *x.p_mpi);
      x.unlock ();
      unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      unlock ();
      throw;
    }
  }

  // compare  a relatif with an integer

  bool Relatif::operator >= (const t_long x) const {
    rdlock ();
    try {
      // prepare x data
      bool   xsgn = (x < 0);
      t_octa xval = xsgn ? -x : x;
      // check against sign first
      if ((d_sgn == true)  && (xsgn == false)) {
	unlock ();
	return false;
      }
      if ((d_sgn == false) && (xsgn == true)){
	unlock ();
	return true;
      }
      // the number have the same sign
      bool result = mpi_geq (*p_mpi, xval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare two relatifs

  bool Relatif::operator >= (const Relatif& x) const {
    rdlock ();
    x.rdlock ();
    try {
      // check against sign first
      if ((d_sgn == true)  && (x.d_sgn == false)) {
	x.unlock ();
	unlock ();
	return false;
      }
      if ((d_sgn == false) && (x.d_sgn == true)){
	x.unlock ();
	unlock ();
	return true;
      }
      // the number have the same sign
      bool result = mpi_geq (*p_mpi, *x.p_mpi);
      x.unlock ();
      unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      unlock ();
      throw;
    }
  }

  // return the class name

  String Relatif::repr (void) const {
    return "Relatif";
  }

  // return a clone of this object

  Object* Relatif::clone (void) const {
    return new Relatif (*this);
  }

  // clear this relatif

  void Relatif::clear (void) {
    wrlock ();
    try {
      d_sgn = false;
      delete p_mpi; p_mpi = new s_mpi;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation of this relatif

  String Relatif::toliteral (void) const {
    rdlock ();
    String result = tostring () + 'R';
    unlock ();
    return result;
  }

  // get a string representation on this relatif

  String Relatif::tostring (void) const {
    rdlock ();
    try {
      Relatif bval = 10;
      Relatif dval = d_sgn ? -(*this) : (*this);
      Buffer buffer;
      // loop until we reach 0
      while (dval != 0) {
	Relatif rval = dval % bval;
	buffer.pushback ((char) ('0' + rval.tolong ()));
	dval = dval / bval;
      }
      // readjust for sign and null value
      if (buffer.length () == 0) {
	unlock ();
	return '0';
      }
      String result;
      if (d_sgn == true) result = result + '-';
      result = result + buffer.tostring ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the relatif did

  t_word Relatif::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the relatif sid

  t_word Relatif::getsid (void) const {
    return SRL_RELT_SID;
  }

  // serialize this relatif

  void Relatif::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the sign
      Serial::wrbool (d_sgn, os);
      // serialize the mpi
      p_mpi->wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this relatif

  void Relatif::rdstream (InputStream& is) {
    wrlock ();
    try {
      // clean the old value
      delete p_mpi; p_mpi = nullptr;
      // deserialize the sign
      d_sgn = Serial::rdbool (is);
      // deserialize the mpi
      p_mpi = mpi_rds (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if this relatif is zero
  
  bool Relatif::iszero (void) const {
    rdlock ();
    try {
      // compute result
      bool result = p_mpi->iszero ();
      // check consistency
      if ((d_sgn == true) && (result == true)) {
	throw Exception ("internal-error", "inconsistent negative null sign");
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if this relatif is even
  
  bool Relatif::iseven (void) const {
    rdlock ();
    try {
      bool result = p_mpi->iseven ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if this relatif is odd
  
  bool Relatif::isodd (void) const {
    rdlock ();
    try {
      bool result = p_mpi->isodd ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the absolute value of this relatif

  Relatif Relatif::abs (void) const {
    rdlock ();
    try {
      // prepare the result
      Relatif result = *this;
      // fix the sign
      result.d_sgn = false;
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compute the power of a relatif

  Relatif Relatif::pow (const t_long e) const {
    // check for negative exponent
    if (e < 0) {
      throw Exception ("exponent-error", "invalid negative exponent in pow");
    }
    // process trivial case
    rdlock ();
    if (e == 0) {
      unlock ();
      return 1;
    }
    if (e == 1) {
      Relatif result = *this;
      unlock ();
      return result;
    }
    // compute power
    try {
      // compute result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      result.p_mpi = mpi_pow (*p_mpi, e);
      // compute sign
      result.d_sgn = ((e & 1) == 0) ? false : d_sgn;
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }


  // compute the power of a relatif

  Relatif Relatif::pow (const Relatif& e) const {
    // check for negative exponent
    e.rdlock ();
    if (e.d_sgn == true) {
      e.unlock ();
      throw Exception ("exponent-error", "invalid negative exponent in pow");
    }
    // process trivial case
    rdlock ();
    if (e == 0) {
      unlock ();
      return 1;
    }
    if (e == 1) {
      Relatif result = *this;
      unlock ();
      return result;
    }
    // compute the power			  
    try {
      // compute result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      result.p_mpi = mpi_pow (*p_mpi, *e.p_mpi);
      // compute sign
      result.d_sgn = (e.iseven () == true) ? false : d_sgn;
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      unlock   ();
      e.unlock ();
      return result;
    } catch (...) {
      unlock   ();
      e.unlock ();
      throw;
    }
  }

  // compute the modular exponentiation of a relatif

  Relatif Relatif::pow (const t_long e, const Relatif& m) const {
    // check for negative exponent
    if (e < 0) {
      throw Exception ("exponent-error", "invalid negative exponent in pow");
    }
    // check the modulus
    m.rdlock ();
    if (m.d_sgn == true) {
      m.unlock ();
      throw Exception ("modulus-error", "invalid negative modulus in pow");
    }
    // process trivial case
    rdlock ();
    if (e == 0) {
      unlock   ();
      m.unlock ();
      return 1;
    }
    if (e == 1) {
      Relatif result = *this % m;
      unlock   ();
      m.unlock ();
      return result;
    }
    // compute the modular exponentiation
    try {
      // compute result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      result.p_mpi = mpi_pow (*p_mpi, e, *m.p_mpi);
      // compute sign
      result.d_sgn = ((e & 1) == 0) ? false : d_sgn;
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      unlock ();
      m.unlock ();
      return result;
    } catch (...) {
      unlock ();
      m.unlock ();
      throw;
    }
  }

  // compute the modular exponentiation of a relatif

  Relatif Relatif::pow (const Relatif& e, const Relatif& m) const {
    // check for negative exponent
    e.rdlock ();
    if (e.d_sgn == true) {
      e.unlock ();
      throw Exception ("exponent-error", "invalid negative exponent in pow");
    }
    // check the modulus
    m.rdlock ();
    if (m.d_sgn == true) {
      e.unlock ();
      m.unlock ();
      throw Exception ("modulus-error", "invalid negative modulus in pow");
    }
    // process trivial case
    rdlock ();
    if (e == 0) {
      unlock   ();
      e.unlock ();
      m.unlock ();
      return 1;
    }
    if (e == 1) {
      Relatif result = *this % m;
      unlock   ();
      e.unlock ();
      m.unlock ();
      return result;
    }
    // compute the modular exponentiation
    try {
      // compute result
      Relatif result; delete result.p_mpi; result.p_mpi = nullptr;
      result.p_mpi = mpi_pow (*p_mpi, *e.p_mpi, *m.p_mpi);
      // compute sign
      result.d_sgn = ((e & 1) == 0) ? false : d_sgn;
      // clamp the result
      if (result.p_mpi->clamp () == true) result.d_sgn = false;
      unlock   ();
      e.unlock ();
      m.unlock ();
      return result;
    } catch (...) {
      unlock   ();
      e.unlock ();
      m.unlock ();
      throw;
    }
  }

  // reverse a relatif by size

  Relatif Relatif::reverse (const long size) const {
    rdlock ();
    try {
      // check for valid size
      if (size <= 1L) {
	throw Exception ("relatif-error", "invalid size in reverse");
      }
      // check for null
      if (iszero () == true) {
	unlock ();
	return 0;
      }
      // build reverse sequence
      Relatif result; Relatif one = 1; one = one << (size - 1L);
      for (long k = 0L; k < size; k++) {
	if (getbit (k) == true) result += one;
	one = one >> 1L;
      }
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
	
  // return the lsb of this relatif

  long Relatif::getlsb (void) const {
    rdlock ();
    try {
      long result = p_mpi->getlsb ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the msb of this relatif

  long Relatif::getmsb (void) const {
    rdlock ();
    try {
      long result = p_mpi->getmsb ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the relatif bit value by position

  bool Relatif::getbit (const long bpos) const {
    // check valid position and lock
    if (bpos < 0) {
      throw Exception ("position-error", "invalid negative bit position");
    }
    // lock and compute
    rdlock ();
    try {
      bool result = p_mpi->getbit (bpos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the relatif bit value by position

  void Relatif::setbit (const long bpos, const bool bval) {
    // check valid position and lock
    if (bpos < 0) {
      throw Exception ("position-error", "invalid negative bit position");
    }
    // lock and compute
    wrlock ();
    try {
      p_mpi->setbit (bpos, bval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the relatif byte buffer size

  long Relatif::getbbs (void) const {
    rdlock ();
    try {
      long result = p_mpi->bsize ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // fill an unsigned buffer representation by size

  long Relatif::toubuf (t_byte* rbuf, const long size) const {
    rdlock ();
    try {
      // bind the mpi value
      long result = p_mpi->toubuf (rbuf, size);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // fill a signed buffer representation by size

  long Relatif::tosbuf (t_byte* rbuf, const long size) const {
    rdlock ();
    try {
      // bind the mpi value
      long result = p_mpi->tosbuf (rbuf, size, d_sgn);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // fill a large buffer with an unsigned representation

  long Relatif::tobyte (t_byte* rbuf, const long size) const {
    rdlock ();
    try {
      // bind the mpi value
      long result = p_mpi->tobyte (rbuf, size);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert this relatif into a hexadecimal representation

  String Relatif::tohexa (void) const {
    rdlock ();
    try {
      // prepare result string
      String result = d_sgn ? "-0x" : "0x";
      // bind the mpi value
      result += p_mpi->tohexa ();
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert this relatif into a hexadecimal string

  String Relatif::tohstr (void) const {
    rdlock ();
    try {
      // bind the mpi value
      String result = p_mpi->tohexa ();
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert this relatif to an integer

  t_long Relatif::tolong (void) const {
    rdlock ();
    try {
      // get native value
      t_octa data = p_mpi->toocta ();
      // check for overflow
      if ((d_sgn == true) && (data > MAX_OCTA_PVAL)) {
	throw Exception ("overflow-error", "cannot convert negative number");
      }
      // adjust with the sign
      t_long result = d_sgn ? - (t_long) data : (t_long) data;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the viewable size
  
  long Relatif::tosize (void) const {
    rdlock ();
    try {
      long result = p_mpi->tovsize ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable data
  
  t_byte* Relatif::tobyte (void) {
    wrlock ();
    try {
      t_byte* result = p_mpi->tovbyte ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the viewable data
  
  const t_byte* Relatif::tobyte (void) const {
    rdlock ();
    try {
      const t_byte* result = p_mpi->tovbyte ();
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
  static const long QUARK_ZONE_LENGTH = 33;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_OR      = zone.intern ("or");
  static const long QUARK_OPP     = zone.intern ("++");
  static const long QUARK_OMM     = zone.intern ("--");
  static const long QUARK_ADD     = zone.intern ("+");
  static const long QUARK_SUB     = zone.intern ("-");
  static const long QUARK_MUL     = zone.intern ("*");
  static const long QUARK_DIV     = zone.intern ("/");
  static const long QUARK_EQL     = zone.intern ("==");
  static const long QUARK_NEQ     = zone.intern ("!=");
  static const long QUARK_LTH     = zone.intern ("<");
  static const long QUARK_LEQ     = zone.intern ("<=");
  static const long QUARK_GTH     = zone.intern (">");
  static const long QUARK_GEQ     = zone.intern (">=");
  static const long QUARK_AEQ     = zone.intern ("+=");
  static const long QUARK_SEQ     = zone.intern ("-=");
  static const long QUARK_MEQ     = zone.intern ("*=");
  static const long QUARK_DEQ     = zone.intern ("/=");
  static const long QUARK_ABS     = zone.intern ("abs");
  static const long QUARK_AND     = zone.intern ("and");
  static const long QUARK_SHL     = zone.intern ("shl");
  static const long QUARK_SHR     = zone.intern ("shr");
  static const long QUARK_XOR     = zone.intern ("xor");
  static const long QUARK_MOD     = zone.intern ("mod");
  static const long QUARK_NOT     = zone.intern ("not");
  static const long QUARK_POW     = zone.intern ("pow");
  static const long QUARK_MMI     = zone.intern ("mmi");
  static const long QUARK_ODDP    = zone.intern ("odd-p");
  static const long QUARK_EVENP   = zone.intern ("even-p");
  static const long QUARK_ZEROP   = zone.intern ("zero-p");
  static const long QUARK_GETMSB  = zone.intern ("get-msb");
  static const long QUARK_GETBIT  = zone.intern ("get-bit");
  static const long QUARK_SETBIT  = zone.intern ("set-bit");
  static const long QUARK_REVERSE = zone.intern ("reverse");

  // create a new object in a generic way

  Object* Relatif::mknew (Vector* argv) {
    if ((argv == nullptr) || (argv->length () == 0)) return new Relatif;
    if (argv->length () != 1) 
      throw Exception ("argument-error", 
		       "too many argument with relatif constructor");
    // try to map the relatif argument
    Object* obj = argv->get (0);
    if (obj == nullptr) return new Relatif;

    // try an integer object
    Integer* ival = dynamic_cast <Integer*> (obj);
    if (ival != nullptr) return new Relatif (ival->tolong ());

    // try a relatif object
    Relatif* xval = dynamic_cast <Relatif*> (obj);
    if (xval != nullptr) return new Relatif (*xval);

    // try a real object
    Real* rval = dynamic_cast <Real*> (obj);
    if (rval != nullptr) return new Relatif (rval->tolong ());

    // try a character object
    Character* cval = dynamic_cast <Character*> (obj);
    if (cval != nullptr) return new Relatif (cval->toquad ());

    // try a string object
    String* sval = dynamic_cast <String*> (obj);
    if (sval != nullptr) return new Relatif (*sval);

    // illegal object
    throw Exception ("type-error", "illegal object with relatif constructor",
		     obj->repr ());
  }

  // return true if the given quark is defined

  bool Relatif::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Number::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // operate this object with another object

  Object* Relatif::oper (t_oper type, Object* object) {
    Integer* iobj = dynamic_cast <Integer*> (object);
    Relatif* zobj = dynamic_cast <Relatif*> (object);
    switch (type) {
    case Object::OPER_ADD:
      if (iobj != nullptr) return new Relatif (*this + iobj->tolong ());
      if (zobj != nullptr) return new Relatif (*this + *zobj);
      break;
    case Object::OPER_SUB:
      if (iobj != nullptr) return new Relatif (*this - iobj->tolong ());
      if (zobj != nullptr) return new Relatif (*this - *zobj);
      break;
    case Object::OPER_MUL:
      if (iobj != nullptr) return new Relatif (*this * iobj->tolong ());
      if (zobj != nullptr) return new Relatif (*this * *zobj);
      break;
    case Object::OPER_DIV:
      if (iobj != nullptr) return new Relatif (*this / iobj->tolong ());
      if (zobj != nullptr) return new Relatif (*this / *zobj);
      break;
    case Object::OPER_UMN:
      return new Relatif (-(*this));
      break;
    case Object::OPER_EQL:
    case Object::OPER_QEQ:
      if (iobj != nullptr) return new Boolean (*this == iobj->tolong ());
      if (zobj != nullptr) return new Boolean (*this == *zobj);
      break;
    case Object::OPER_NEQ:
      if (iobj != nullptr) return new Boolean (*this != iobj->tolong ());
      if (zobj != nullptr) return new Boolean (*this != *zobj);
      break;
    case Object::OPER_GEQ:
      if (iobj != nullptr) return new Boolean (*this >= iobj->tolong ());
      if (zobj != nullptr) return new Boolean (*this >= *zobj);
      break;
    case Object::OPER_GTH:
      if (iobj != nullptr) return new Boolean (*this >  iobj->tolong ());
      if (zobj != nullptr) return new Boolean (*this >  *zobj);
      break;
    case Object::OPER_LEQ:
      if (iobj != nullptr) return new Boolean (*this <= iobj->tolong ());
      if (zobj != nullptr) return new Boolean (*this <= *zobj);
      break;
    case Object::OPER_LTH:
      if (iobj != nullptr) return new Boolean (*this <  iobj->tolong ());
      if (zobj != nullptr) return new Boolean (*this <  *zobj);
      break;
    }
    throw Exception ("type-error", "invalid operand with relatif",
		     Object::repr (object));
  }
  
  // set an object to this relatif
  
  Object* Relatif::vdef (Evaluable* zobj, Nameset* nset, Object* object) {
    Integer* iobj = dynamic_cast <Integer*> (object);
    if (iobj != nullptr) {
      *this = iobj->tolong ();
      return this;
    }
    Real* fobj = dynamic_cast <Real*> (object);
    if (fobj != nullptr) {
      *this = fobj->tolong ();
      return this;
    }
    Relatif* mobj = dynamic_cast <Relatif*> (object);
    if (mobj != nullptr) {
      *this = *mobj;
      return this;
    }
    throw Exception ("type-error", "invalid object with relatif vdef",
		     Object::repr (object));
  }

  // apply this object with a set of arguments and a quark

  Object* Relatif::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_ABS)    return new Relatif (abs ());
      if (quark == QUARK_EVENP)  return new Boolean (iseven ());
      if (quark == QUARK_ODDP)   return new Boolean (isodd  ());
      if (quark == QUARK_ZEROP)  return new Boolean (iszero ());
      if (quark == QUARK_GETMSB) return new Integer (getmsb ());
      if (quark == QUARK_OPP) {
	++(*this);
	zobj->post (this);
	return this;
      }
      if (quark == QUARK_OMM) {
	--(*this);
	zobj->post (this);
	return this;
      }
      if (quark == QUARK_NOT) {
	return new Relatif (~(*this));
      }
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) return oper (Object::OPER_ADD, argv->get (0));
      if (quark == QUARK_SUB) return oper (Object::OPER_SUB, argv->get (0));
      if (quark == QUARK_MUL) return oper (Object::OPER_MUL, argv->get (0));
      if (quark == QUARK_DIV) return oper (Object::OPER_DIV, argv->get (0));
      if (quark == QUARK_EQL) return oper (Object::OPER_EQL, argv->get (0));
      if (quark == QUARK_NEQ) return oper (Object::OPER_NEQ, argv->get (0));
      if (quark == QUARK_LTH) return oper (Object::OPER_LTH, argv->get (0));
      if (quark == QUARK_LEQ) return oper (Object::OPER_LEQ, argv->get (0));
      if (quark == QUARK_GTH) return oper (Object::OPER_GTH, argv->get (0));
      if (quark == QUARK_GEQ) return oper (Object::OPER_GEQ, argv->get (0));
      if (quark == QUARK_REVERSE) {
	long size = argv->getlong (0);
	return new Relatif (reverse (size));
      }
      if (quark == QUARK_AEQ) {
	t_long val = argv->getlong (0);
	*this += val;
	zobj->post (this);
	return this;
      }
      if (quark == QUARK_SEQ) {
	t_long val = argv->getlong (0);
	*this -= val;
	zobj->post (this);
	return this;
      }
      if (quark == QUARK_MEQ) {
	t_long val = argv->getlong (0);
	*this *= val;
	zobj->post (this);
	return this;
      }
      if (quark == QUARK_DEQ) {
	t_long val = argv->getlong (0);
	*this /= val;
	zobj->post (this);
	return this;
      }
      if (quark == QUARK_MOD) {
	Object* obj = argv->get (0);
	// check for relatif
	Relatif* mobj = dynamic_cast <Relatif*> (obj);
	if (mobj != nullptr) {
	  Object* result = new Relatif (*this % *mobj);
	  return result;
	}
	// check for integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  Object* result = new Relatif (*this % iobj->tolong ());
	  return result;
	}
	throw Exception ("type-error", "invalid object with mod operator",
			 Object::repr (obj));
      }
      if (quark == QUARK_POW) {
	Object* obj = argv->get (0);
	// check for relatif
	Relatif* mobj = dynamic_cast <Relatif*> (obj);
	if (mobj != nullptr) {
	  Object* result = new Relatif (pow (*mobj));
	  return result;
	}
	// check for integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  Object* result = new Relatif (pow (iobj->tolong ()));
	  return result;
	}
	throw Exception ("type-error", "invalid object with pow operator",
			 Object::repr (obj));
      }
      if (quark == QUARK_MMI) {
	Object* obj = argv->get (0);
	// check for relatif
	Relatif* mobj = dynamic_cast <Relatif*> (obj);
	if (mobj != nullptr) {
	  Object* result = new Relatif (Relatif::mmi (*this, *mobj));
	  return result;
	}
	// check for integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  t_long ival = iobj->tolong ();
	  Object* result = new Relatif (Relatif::mmi (*this, ival));
	  return result;
	}
	throw Exception ("type-error", "invalid object with mmi operator",
			 Object::repr (obj));
      }
      if (quark == QUARK_SHL) {
	t_long asl = argv->getlong (0);
	Object* result = new Relatif (*this << asl);
	return result;
      }
      if (quark == QUARK_SHR) {
	t_long asr = argv->getlong (0);
	Object* result = new Relatif (*this >> asr);
	return result;
      }
      if (quark == QUARK_XOR) {
	Object* obj = argv->get (0);
	// check for relatif
	Relatif* mobj = dynamic_cast <Relatif*> (obj);
	if (mobj != nullptr) {
	  Object* result = new Relatif (*this ^ *mobj);
	  return result;
	}
	// check for integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  Object* result = new Relatif (*this ^ iobj->tolong ());
	  return result;
	}
	throw Exception ("type-error", "invalid object with xor operator",
			 Object::repr (obj));
      }
      if (quark == QUARK_AND) {
	Object* obj = argv->get (0);
	// check for relatif
	Relatif* mobj = dynamic_cast <Relatif*> (obj);
	if (mobj != nullptr) {
	  Object* result = new Relatif (*this & *mobj);
	  return result;
	}
	// check for integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  Object* result = new Relatif (*this & iobj->tolong ());
	  return result;
	}
	throw Exception ("type-error", "invalid object with and operator",
			 Object::repr (obj));
      }
      if (quark == QUARK_OR) {
	Object* obj = argv->get (0);
	// check for relatif
	Relatif* mobj = dynamic_cast <Relatif*> (obj);
	if (mobj != nullptr) {
	  Object* result = new Relatif (*this | *mobj);
	  return result;
	}
	// check for integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  Object* result = new Relatif (*this | iobj->tolong ());
	  return result;
	}
	throw Exception ("type-error", "invalid object with or operator",
			 Object::repr (obj));
      }
      if (quark == QUARK_GETBIT) {
	long bpos = argv->getlong (0);
	return new Boolean (getbit (bpos));
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_POW) {
	Object* e = argv->get (0);
	Object* m = argv->get (1);
	Relatif* mmo = dynamic_cast <Relatif*> (m);
	if (mmo == nullptr) {
	  throw Exception ("type-error", "invalid modulus with pow operator",
			   Object::repr (m));
	}
	// check for relatif
	Relatif* emo = dynamic_cast <Relatif*> (e);
	if (emo != nullptr) {
	  Object* result = new Relatif (pow (*emo, *mmo));
	  return result;
	}
	// check for integer
	Integer* eio = dynamic_cast <Integer*> (e);
	if (eio != nullptr){
	  t_long ie = eio->tolong ();
	  Object* result = new Relatif (pow (ie, *mmo));
	  return result;
	}
	throw Exception ("type-error", "invalid exponent with pow operator",
			 Object::repr (e));
      }
      if (quark == QUARK_SETBIT) {
	long bpos = argv->getlong (0);
	bool bval = argv->getbool (1);
	setbit (bpos, bval);
	return nullptr;
      }
    }
    // call the number method
    return Number::apply (zobj, nset, quark, argv);
  }
}
