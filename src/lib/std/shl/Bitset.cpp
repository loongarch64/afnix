// ---------------------------------------------------------------------------
// - Bitset.cpp                                                              -
// - standard object library - bitset class implementation                   -
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
#include "Vector.hpp"
#include "Bitset.hpp"
#include "Utility.hpp"
#include "Unicode.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
    
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the bitset octa size in bytes
  static const long   BSET_OCTA_BSZ = sizeof (t_octa);
  // the bitset octa size in bits
  static const long   BSET_OCTA_BIT = BSET_OCTA_BSZ << 3;
  // the bitset octa mask
  static const t_octa BSET_OCTA_MSK = 0x0000000000000001ULL;

  // this procedure return the number of bytes needed to create a bitset
  static long bset_byte_size (const long size) {
    if (size <= 0) return 0;
    long result = size / 8;
    if ((size % 8) == 0) return result;
    return (result + 1);
  }

  // this procedure parse a string and returns the number of bits
  static long bset_bits_size (const String& s) {
    // the string length
    long len = s.length ();
    if (len == 0) return 0;
    // check for starting index
    long si = 0;
    bool hb = false;
    if ((len > 2) && (s[0] == Unicode::toquad ('0')) && 
	((s[1] == Unicode::toquad ('b')) || 
	 (s[1] == Unicode::toquad ('B')))) si = 2;
    if ((len > 2) && (s[0] == Unicode::toquad ('0')) && 
	((s[1] == Unicode::toquad ('x')) || 
	 (s[1] == Unicode::toquad ('X')))) hb = true;
    if (hb == true) si = 2;
    // look for valid characters
    long result = 0;
    for (long i = si; i < len; i++) {
      t_quad c = s[i];
      if (c == Unicode::toquad ('_')) continue;
      if ((hb == false) && (Unicode::isbit (c) == true)) {
	result++;
	continue;
      }
      if ((hb == true) && (Unicode::ishexa (c) == true)) {
	result += 4;
	continue;
      }
      throw Exception ("bitset-error", "invalid bit string to parse", s);
    }
    return result;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // generate a random bitset

  Bitset Bitset::random (const long bits) {
    // check for valid size
    if (bits < 0) {
      throw Exception ("size-error", "invalid random number size");
    }
    // create a bitset by size
    Bitset result (bits);
    // fill it with random bytes
    for (long i = 0; i < result.d_bsiz; i++) {
      result.p_byte[i] = Utility::byternd ();
    }
    // here it is
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default bitset

  Bitset::Bitset (void) {
    d_size   = 0;
    d_bsiz   = 0;
    p_byte   = nullptr;
  }

  // create a new bitset by size

  Bitset::Bitset (const long size) {
    d_size = size;
    d_bsiz = bset_byte_size (d_size); 
    p_byte = nullptr;
    if (d_bsiz > 0) {
      p_byte = new t_byte[d_bsiz];
      for (long i = 0; i < d_bsiz; i++) p_byte[i] = nilc;
    }
  }

  // create a new bitset by string

  Bitset::Bitset (const String& sval) {
    d_size = 0;
    d_bsiz = 0;
    p_byte = nullptr;
    set (sval);
  }

  // create a bitset by buffer

  Bitset::Bitset (const Buffer& buf, const bool abf) {
    // lock the buffer
    buf.rdlock ();
    try {
      // get the buffer size
      d_bsiz = buf.length ();
      d_size = d_bsiz * 8;
      // allocate the buffer
      p_byte = new t_byte[d_bsiz];
      // copy in ascending or not
      for (long i = 0; i < d_bsiz; i++) {
	p_byte[i] = abf ? buf.get (i) : buf.get (d_bsiz-i-1);
      }
      // unlock and return
      buf.unlock ();
    } catch (...) {
      buf.unlock ();
      throw;
    }
  }

  // copy constructor for this bitset

  Bitset::Bitset (const Bitset& that) {
    that.rdlock ();
    try {
      d_size = that.d_size;
      d_bsiz = that.d_bsiz;
      p_byte = new t_byte[d_bsiz];
      for (long i = 0; i < d_bsiz; i++) p_byte[i] = that.p_byte[i];
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // copy move a bitset

  Bitset::Bitset (Bitset&& that) noexcept {
    that.wrlock ();
    try {
      // move base object
      Object::operator = (static_cast<Object&&>(that));
      // copy move locally
      d_size = that.d_size; that.d_size = 0L;
      d_bsiz = that.d_bsiz; that.d_bsiz = 0L;
      p_byte = that.p_byte; that.p_byte = nullptr;
      that.unlock ();
    } catch (...) {
      d_size = 0L;
      d_bsiz = 0L;
      p_byte = nullptr;
      that.unlock ();
    }
  }
  
  // destroy this bitset

  Bitset::~Bitset (void) {
    delete [] p_byte;
  }

  // assign a bitset to this one

  Bitset& Bitset::operator = (const Bitset& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // destroy the old bitset
      delete [] p_byte;
      // copy the size and array
      d_size = that.d_size;
      d_bsiz = that.d_bsiz;
      p_byte = new t_byte[d_bsiz];
      for (long i = 0; i < d_bsiz; i++) p_byte[i] = that.p_byte[i];
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // move a bitset to this one

  Bitset& Bitset::operator = (Bitset&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      // clean locally
      delete [] p_byte;
      // assign base object
      Object::operator = (static_cast<Object&&>(that));
      // move locally
      d_size = that.d_size; that.d_size = 0L;
      d_bsiz = that.d_bsiz; that.d_bsiz = 0L;
      p_byte = that.p_byte; that.p_byte = nullptr;
      unlock ();
      that.unlock ();
    } catch (...) {
      d_size = 0L;
      d_bsiz = 0L;
      p_byte = nullptr;
      unlock ();
      that.unlock ();
    }
    return *this;
  }

  // return the class name

  String Bitset::repr (void) const {
    return "Bitset";
  }

  // return a clone of this object

  Object* Bitset::clone (void) const {
    return new Bitset (*this);
  }

  // check that two bitsets are equal

  bool Bitset::operator == (const Bitset& that) const {
    rdlock ();
    that.rdlock ();
    try {
      if (d_size != that.d_size) {
	unlock ();
	that.unlock ();
	return false;
      }
      for (long i = 0; i < d_bsiz; i++) {
	if (p_byte[i] != that.p_byte[i]) {
	  unlock ();
	  that.unlock ();
	  return false;
	}
      }
      unlock ();
      that.unlock ();
      return true;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // add a bitset to this one

  Bitset Bitset::operator + (const Bitset& bset) const {
    rdlock ();
    bset.rdlock ();
    try {
      // get the total size
      long size = d_size + bset.d_size;
      // create the final biset
      Bitset result (size);
      // add the argument bitset first
      for (long i = 0; i < bset.d_size; i++) {
	result.mark (i, bset.ismark (i));
      }
      // add the calling bitset
      for (long i = 0; i < d_size; i++) {
	result.mark (bset.d_size + i, ismark (i));
      }
      unlock ();
      bset.unlock ();
      return result;
    } catch (...) {
      unlock ();
      bset.unlock ();
      throw;
    }
  }

  // reset this bitset

  void Bitset::reset (void) {
    wrlock ();
    try {
      // clean old array
      delete [] p_byte;
      // reset elements
      d_size = 0;
      d_bsiz = 0;
      p_byte = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the bit set size
  
  long Bitset::length (void) const {
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

  // return the bit set byte size
  
  long Bitset::bsize (void) const {
    rdlock ();
    try {
      long result = d_bsiz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // check if a bit is marked by position

  bool Bitset::ismark (const long pos) const {
    rdlock ();
    try {
      // check for valid bounds
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("bitset-error", "invalid bit position in marked-p");
      }
      t_byte byte = p_byte[pos/8];
      long   boff = pos % 8;
      t_byte mask = (boff == 0) ? 0x01 : (0x01 << boff);
      unlock ();
      return ((byte & mask) == mask);
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear a bit at a certain position

  void Bitset::clear (const long pos) {
    // lock and clear
    wrlock ();
    try {
      // check for valid bounds
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("bitset-error", "invalid bit position in clear");
      }
      // clear at position
      long   boff    = pos % 8;
      t_byte mask    = (boff == 0) ? 0x01 : (0x01 << boff);
      p_byte[pos/8] &= (~mask);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // mark a bit at a certain position

  void Bitset::mark (const long pos) {
    wrlock ();
    try {
      // check for valid bounds
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("bitset-error", "invalid bit position in mark");
      }
      // mark at position
      long   boff    = pos % 8;
      t_byte mask    = (boff == 0) ? 0x01 : (0x01 << boff);
      p_byte[pos/8] |= mask;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // mark a bit at a certain position by value

  void Bitset::mark (const long pos, const bool bval) {
    wrlock ();
    try {
      if (bval == true) mark (pos); else clear (pos);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a bit by position

  void Bitset::add (const long pos, const bool bval) {
    wrlock ();
    try {
      // check for negative index
      if (pos < 0) {
	throw Exception ("bitset-error", "invalid position for adding");
      }
      // check for position in bounds
      if (pos < d_size) {
	// create a bitset with one more bit
	Bitset bs (d_size + 1);
	// mark the bitset by part
	for (long i = 0; i < pos; i++) bs.mark (i, ismark (i));
	bs.mark (pos, bval);
	for (long i = pos; i < d_size; i++) bs.mark (i+1, ismark (i));
	// copy locally and exit
	*this = bs;
	unlock ();
	return;
      }
      // create an out-of-bounds position
      Bitset bs (pos + 1);
      for (long i = 0; i < d_size; i++) bs.mark (i, ismark (i));
      bs.mark (pos, bval);
      *this = bs;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a bitset by octa value

  void Bitset::set (const t_octa oval) {
    wrlock ();
    try {
      // clear old value
      delete [] p_byte;
      // set the new bitset
      d_bsiz = sizeof (t_octa);
      d_size = d_bsiz << 3;
      p_byte = new t_byte[d_bsiz];
      for (long i = 0; i < d_bsiz; i++) p_byte[i] = nilc;
      // copy the octa
      for (long i = 0; i < BSET_OCTA_BIT; i++) {
	// prepare the sid mask
	t_octa mask = (i == 0) ? BSET_OCTA_MSK : BSET_OCTA_MSK << i;
	// check the value with the mask
	bool bval = (oval & mask) == mask;
	// mark the bitset bit
	mark (i, bval);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a bitset by string value

  void Bitset::set (const String& sval) {
    wrlock ();
    try {
      // delete the old value
      delete [] p_byte;
      d_size = 0;
      d_bsiz = 0;
      p_byte = nullptr;
      // check the bitset length
      d_size = bset_bits_size (sval);
      d_bsiz = bset_byte_size (d_size); 
      if (d_size == 0) {
	unlock ();
	return;
      }
      p_byte = new t_byte[d_bsiz];
      for (long i = 0; i < d_bsiz; i++) p_byte[i] = nilc;
      // now parse the string
      long len = sval.length ();
      // check for starting index
      long si = 0;
      bool hb = false;
      if ((len > 2) && (sval[0] == Unicode::toquad ('0')) && 
	  ((sval[1] == Unicode::toquad ('b')) || 
	   (sval[1] == Unicode::toquad ('B')))) si = 2;
      if ((len > 2) && (sval[0] == Unicode::toquad ('0')) && 
	  ((sval[1] == Unicode::toquad ('x')) || 
	   (sval[1] == Unicode::toquad ('X')))) hb = true;
      if (hb == true) si = 2;
      // parse in reverse in bit mode
      if (hb == false) {
	long pos = d_size;
	for (long i = si ; i < len; i++) {
	  t_quad c = sval[i];
	  if (c == Unicode::toquad ('_')) continue;
	  if (c == Unicode::toquad ('0')) {
	    clear (--pos);
	    continue;
	  }
	  if (c == Unicode::toquad ('1')) {
	    mark (--pos);
	    continue;
	  }
	  throw Exception ("bitset-error", "cannot parse bitset string" , sval);
	}
      } else {
	// check bit size validity
	if ((d_size % 4) != 0) {
	  throw Exception ("internal-error", "invalid bitset haxadecimal size");
	}
	// preset the byte position
	long pos = d_bsiz - 1;
	// check for a non-null first byte
	bool ub = (d_size % 8) == 0;
	if (ub == true) p_byte[pos] = Unicode::htob (sval[si++]) << 4;
	p_byte[pos--] |= Unicode::htob (sval[si++]);
	// process the last bytes
	for (long i = si; i < len; i++) {
	  if (pos < 0) {
	    throw Exception ("internal-error", "negative bitset index");
	  }
	  // process the upper byte
	  p_byte[pos] = Unicode::htob (sval[i++]) << 4;
	  // process the lower byte
	  p_byte[pos--] |= Unicode::htob (sval[i]);
	}
      }
      unlock ();
    } catch (...) {
      delete [] p_byte;
      d_size = 0;
      d_bsiz = 0;
      p_byte = nullptr;
      unlock ();
      throw;
    }
  }

  // map the bitset to a byte by position

  t_byte Bitset::tobyte (const long pos) const {
    rdlock ();
    try {
      // check for valid bounds
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("bitset-error", "invalid bit position in getb");
      }
      // initialize result
      t_byte result = 0x00;
      // loop for 8 bits
      for (long i = 0; i < 8; i++) {
	// get the bit at position without error on overflow
	long bpos = pos + i;
	bool bval = (bpos < d_size) ? ismark (bpos) : false;
	// set it in the result
	if (bval == true) {
	  result |= (i == 0) ? 0x01 : (0x01 << i);
	}
      }
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the clamping bit index by value

  long Bitset::getcbi (const bool bval) const {
    rdlock ();
    try {
      for (long i = d_size-1; i >= 0; i--) {
	if (ismark (i) == bval) {
	  unlock ();
	  return i;
	}
      }
      unlock ();
      return -1;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clamp the bitset by value

  void Bitset::clamp (const bool bval) {
    wrlock ();
    try {
      // get the clamping bit
      long cbi = getcbi (bval);
      // check for empty
      if (cbi == -1) {
	delete [] p_byte;
	d_size = 0;
	d_bsiz = 0;
	p_byte = nullptr;
      } else {
	// adjust to new size
	long size = cbi + 1;
	long bsiz = bset_byte_size (size);
	t_byte* byte = new t_byte[bsiz];
	for (long i = 0; i < bsiz; i++) byte[i] = p_byte[i];
	delete [] p_byte;
	d_size = size;
	d_bsiz = bsiz;
	p_byte = byte;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a sub bitset by size

  Bitset Bitset::subset (const long size) const {
    rdlock ();
    try {
      Bitset result = subset (size, 0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a sub bitset by size and position

  Bitset Bitset::subset (const long size, const long spos) const {
    // check for null
    if (size == 0) return Bitset ();
    rdlock ();
    try {
      // check for valid size
      if ((size < 0) || (spos < 0) || ((spos + size) > d_size)) {
	unlock ();
	throw Exception ("bitset-error", "invalid sub bitset size");
      }
      // create a bitset by size
      Bitset bset (size);
      // mark the bit by index
      for (long i = 0; i < size; i++) bset.mark (i, ismark (spos+i));
      // here it is
      unlock ();
      return bset;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert a bitset to a relatif

  Relatif Bitset::torelatif (void) const {
    rdlock ();
    try {
      Relatif result (p_byte, d_bsiz);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map this buffer to an anonymous data structure

  long Bitset::tomap (void* data, const long size) const {
    rdlock ();
    try {
      // check for a null content
      if (d_bsiz == 0) {
	unlock ();
	return 0;
      }
      // process normal size
      long result = (size < d_bsiz) ? size : d_bsiz;
      t_byte* ptr = reinterpret_cast <t_byte*> (data);
      // copy the buffer content
      for (long i = 0; i < result; i++) ptr[i] = p_byte[d_bsiz-1-i];
      // unlock and return
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
  static const long QUARK_ZONE_LENGTH = 11;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_SET    = zone.intern ("set");
  static const long QUARK_MARK   = zone.intern ("mark");
  static const long QUARK_MARKP  = zone.intern ("marked-p");
  static const long QUARK_RESET  = zone.intern ("reset");
  static const long QUARK_CLEAR  = zone.intern ("clear");
  static const long QUARK_CLAMP  = zone.intern ("clamp");
  static const long QUARK_BSIZE  = zone.intern ("bsize");
  static const long QUARK_LENGTH = zone.intern ("length");
  static const long QUARK_TOBYTE = zone.intern ("to-byte");
  static const long QUARK_SUBSET = zone.intern ("subset");
  static const long QUARK_TORELA = zone.intern ("to-relatif");

  // create a new object in a generic way

  Object* Bitset::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc == 0) return new Bitset;
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for an integer
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj != nullptr) {
	long size = iobj->tolong ();
	return new Bitset (size);
      }
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) {
	return new Bitset (*sobj);
      }
      throw Exception ("type-error", "invalid object with bitset constructor",
		       Object::repr (obj));
    }
    if (argc == 2) {
      // get the buffer argument
      Object* obj = argv->get (0);
      Buffer* buf = dynamic_cast <Buffer*> (obj);
      if (buf == nullptr) {
	throw Exception ("type-error", "invalid bitset argument as buffer",
			 Object::repr (obj));
      }
      bool abf = argv->getbool (1);
      return new Bitset (*buf, abf);      
    }
    throw Exception ("argument-error", "too many argument for bitset");
  }

  // return true if the given quark is defined

  bool Bitset::isquark (const long quark, const bool hflg) const {
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

  Object* Bitset::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_BSIZE)  return new Integer (bsize  ());
      if (quark == QUARK_LENGTH) return new Integer (length ());
      if (quark == QUARK_TORELA) return new Relatif (torelatif ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SET) {
	Object* obj = argv->get (0);
	// check for an integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  t_octa oval = iobj->tolong ();
	  set (oval);
	  return nullptr;
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  set (*sobj);
	  return nullptr;
	}
	// invalid argument
	throw Exception ("type-error", "invalid object with set",
			 Object::repr (obj));
      }
      if (quark == QUARK_MARKP) {
	long pos = argv->getlong (0);
	return new Boolean (ismark (pos));
      }
      if (quark == QUARK_MARK) {
	long pos = argv->getlong (0);
	mark (pos);
	return nullptr;
      }
      if (quark == QUARK_CLEAR) {
	long pos = argv->getlong (0);
	clear (pos);
	return nullptr;
      }
      if (quark == QUARK_TOBYTE) {
	long pos = argv->getlong (0);
	return new Byte (tobyte (pos));
      }
      if (quark == QUARK_CLAMP) {
	bool bval = argv->getbool (0);
	clamp (bval);
	return nullptr;
      }
      if (quark == QUARK_SUBSET) {
	long size = argv->getlong (0);
	return new Bitset (subset (size));
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_MARK) {
	long pos = argv->getlong (0);
	bool val = argv->getbool (1);
	mark (pos, val);
	return nullptr;
      }
      if (quark == QUARK_ADD) {
	long pos = argv->getlong (0);
	bool val = argv->getbool (1);
	add (pos, val);
	return nullptr;
      }
      if (quark == QUARK_SUBSET) {
	long size = argv->getlong (0);
	long spos = argv->getlong (1);
	return new Bitset (subset (size, spos));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
