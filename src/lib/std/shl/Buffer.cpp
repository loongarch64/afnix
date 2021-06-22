// ---------------------------------------------------------------------------
// - Buffer.cpp                                                              -
// - standard object library - character buffer class implementation         -
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
#include "Ascii.hpp"
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Buffer.hpp"
#include "System.hpp"
#include "Utility.hpp"
#include "Unicode.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"
#include "ccnv.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // the buffer serialization array size
  static const long BUF_ARRY_SIZE = 8192L;
  
  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // generate a random buffer

  Buffer* Buffer::random (const long size) {
    // check for valid size
    if (size < 0) {
      throw Exception ("size-error", "invalid random number size");
    }
    // create a buffer by size
    Buffer* result = new Buffer (size);
    // fill it with random bytes
    for (long i = 0; i < size; i++) {
      result->add ((char) Utility::byternd ());
    }
    // here it is
    return result;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new buffer class with a default size of 1024 characters

  Buffer::Buffer (void) {
    d_size = System::blocksz ();
    p_data = new char [d_size];
    d_blen = 0L;
    d_ridx = 0L;
    d_rflg = true;
    d_pflg = false;
    d_emod = Encoding::EMOD_BYTE;
  }

  // create a new buffer with a predefined size

  Buffer::Buffer (const long size) {
    d_size = (size <= 0) ? System::blocksz () : size;
    p_data = new char[d_size];
    d_blen = 0L;
    d_ridx = 0L;
    d_rflg = true;
    d_pflg = false;
    d_emod = Encoding::EMOD_BYTE;
  }

  // create a new buffer by mode

  Buffer::Buffer (const Encoding::t_emod emod) {
    d_size = System::blocksz ();
    p_data = new char[d_size];
    d_blen = 0L;
    d_ridx = 0L;
    d_rflg = true;
    d_pflg = false;
    d_emod = emod;
  }

  // create a new buffer by size with a mode

  Buffer::Buffer (const long size, const Encoding::t_emod emod) {
    d_size = (size <= 0) ? System::blocksz () : size;
    p_data = new char[d_size];
    d_blen = 0L;
    d_ridx = 0L;
    d_rflg = true;
    d_pflg = false;
    d_emod = emod;
  }

  // create a new buffer and initialize it with a c string

  Buffer::Buffer (const char* value) {
    d_size = System::blocksz ();
    p_data = new char[d_size];
    d_blen = 0L;
    d_ridx = 0L;
    d_rflg = true;
    d_pflg = false;
    d_emod = Encoding::EMOD_UTF8;
    add (value, Ascii::strlen (value));
  }

  // create a new buffer and initialize it with a string

  Buffer::Buffer (const String& value) {
    d_size = System::blocksz ();
    d_blen = 0L;
    d_ridx = 0L;
    d_rflg = true;
    d_pflg = false;
    p_data = new char[d_size];
    d_emod = Encoding::EMOD_UTF8;
    add (value);
  }

  // create a new buffer by size and content, no copy, no resize

  Buffer::Buffer (const long size, const long blen, char* data) {
    d_size = size;;
    d_blen = blen;
    d_ridx = 0L;
    d_rflg = false;
    d_pflg = true;
    p_data = data;
    d_emod = Encoding::EMOD_BYTE;
  }

  // copy construct a buffer

  Buffer::Buffer (const Buffer& that) {
    that.rdlock ();
    try {
      d_size = that.d_size;
      d_blen = 0L;
      d_ridx = 0L;
      d_rflg = that.d_rflg;
      d_pflg = false;
      d_emod = that.d_emod;
      p_data = new char[d_size];
      add (that.p_data + that.d_ridx, that.d_blen);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move a buffer

  Buffer::Buffer (Buffer&& that) noexcept {
    that.wrlock ();
    try {
      // move base viewable
      Viewable::operator = (static_cast<Viewable&&>(that));
      // copy move locally
      d_size = that.d_size; that.d_size = 0L;
      d_blen = that.d_blen; that.d_blen = 0L;
      d_ridx = that.d_ridx; that.d_ridx = 0L;
      d_rflg = that.d_rflg; that.d_rflg = true;
      d_pflg = that.d_pflg; that.d_pflg = false;
      p_data = that.p_data; that.p_data = nullptr;
      d_emod = that.d_emod; that.d_emod = Encoding::EMOD_BYTE;
    } catch (...) {
      d_size = 0L;
      d_blen = 0L;
      d_ridx = 0L;
      d_rflg = true;
      d_pflg = false;
      p_data = nullptr;
      d_emod = Encoding::EMOD_BYTE;
    }
    that.unlock ();
  }

  // destroy this buffer
  
  Buffer::~Buffer (void) {
    if (d_pflg == false) delete [] p_data;
  }

  // assign a buffer to this one

  Buffer& Buffer::operator = (const Buffer& that) {
    if (this == &that) return *this;
    wrlock ();
    that.rdlock ();
    try {
      // clean the old data
      delete [] p_data;
      p_data = nullptr;
      d_blen = 0L;
      d_ridx = 0L;
      // add the new data
      d_size = that.d_size;
      d_rflg = that.d_rflg;
      d_pflg = false;
      d_emod = that.d_emod;
      p_data = new char[d_size];
      add (that.p_data + that.d_ridx, that.d_blen);
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // move a buffer into this one

  Buffer& Buffer::operator = (Buffer&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      // clean locally
      delete [] p_data;
      // move base viewable
      Viewable::operator = (static_cast<Viewable&&>(that));
      // move locally
      d_size = that.d_size; that.d_size = 0L;
      d_blen = that.d_blen; that.d_blen = 0L;
      d_ridx = that.d_ridx; that.d_ridx = 0L;
      d_rflg = that.d_rflg; that.d_rflg = true;
      d_pflg = that.d_pflg; that.d_pflg = false;
      p_data = that.p_data; that.p_data = nullptr;
      d_emod = that.d_emod; that.d_emod = Encoding::EMOD_BYTE;
    } catch (...) {
      d_size = 0L;
      d_blen = 0L;
      d_ridx = 0L;
      d_rflg = true;
      d_pflg = false;
      p_data = nullptr;
      d_emod = Encoding::EMOD_BYTE;
    }
    unlock ();
    that.unlock ();
    return *this;
  }
  
  // compare two buffers

  bool Buffer::operator == (const Buffer& that) const {
    rdlock ();
    that.rdlock ();
    try {
      // check length first
      if (d_blen != that.d_blen) {
	unlock ();
	that.unlock ();
	return false;
      }
      // check for null
      if (d_blen == 0L) {
	unlock ();
	that.unlock ();
	return true;
      }
      // normal compare
      bool result = true;
      for (long k = 0L; k < d_blen; k++) {
	if (p_data[k + d_ridx] != that.p_data[k + that.d_ridx]) {
	  result = false;
	  break;
	}
      }
      unlock ();
      that.unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // compare two buffers

  bool Buffer::operator != (const Buffer& that) const {
    return !(*this == that);
  }

  // xor this buffer with another one

  Buffer& Buffer::operator ^= (const Buffer& that) {
    wrlock ();
    try {
      // check for valid length
      if (d_blen != that.d_blen) {
	throw Exception ("buffer-error", "inconsistent length with xor");
      }
      // xor in the target buffer
      for (long k = 0L; k < d_blen; k++) {
	p_data[k + d_ridx] ^= that.p_data[k + that.d_ridx];
      }
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the class name

  String Buffer::repr (void) const {
    return "Buffer";
  }

  // return a clone of this object

  Object* Buffer::clone (void) const {
    return new Buffer (*this);
  }

  // return the buffer did

  t_word Buffer::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the buffer sid

  t_word Buffer::getsid (void) const {
    return SRL_BUFR_SID;
  }

  // serialize this object
  
  void Buffer::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the object size
      Serial::wrlong (d_size, os);
      Serial::wrlong (d_blen, os);
      Serial::wrlong (0L, os);
      // write the resize flag
      Serial::wrbool (d_rflg, os);
      // write the encoding mode
      os.write ((char) Encoding::tocode (d_emod));
      // write the data
      if (p_data != nullptr) {
	Serial::wrbool (true, os);
        Serial::Array arry (BUF_ARRY_SIZE, Serial::Array::ATYP_BYTE);
        for (long k = 0; k < d_blen; k++) {
          arry.add ((t_byte) p_data[k + d_ridx]);
          if (arry.full () == false) continue;
          Serial::wrarry (arry, os);
          arry.clear ();
        }
        if (arry.empty () == false) Serial::wrarry (arry, os);
      } else {
        Serial::wrbool (false, os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }  
  }

  // deserialize this object

  void Buffer::rdstream (InputStream& is) {
    wrlock ();
    try {
      // get the object size
      d_size = Serial::rdlong (is);
      d_blen = Serial::rdlong (is);
      d_ridx = Serial::rdlong (is);
      // get the resize flag
      d_rflg = Serial::rdbool (is);
      // get the encoding mode
      d_emod = Encoding::toemod ((t_byte) is.read ());
      // check for data
      if (Serial::rdbool (is) == true) {
        p_data = new char[d_size];
        for (long i = 0L; i < d_blen; i++) {
          Array arry = Serial::rdarry (is);
          long  blen = arry.length ();
          for (long k = 0; k < blen; k++) {
            p_data[i+k] = (char) arry.getbyte (k);
          }
          i += (blen - 1L);
        }
        for (long k = d_blen; k < d_size; k++) p_data[k] = nilc;
      }      
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable size

  long Buffer::tosize (void) const {
    rdlock ();
    try {
      long result = d_blen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the viewable data

  t_byte* Buffer::tobyte (void) {
    wrlock ();
    try {
      auto result = reinterpret_cast<t_byte*>(p_data + d_ridx);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the viewable data

  const t_byte* Buffer::tobyte (void) const {
    rdlock ();
    try {
      auto result = reinterpret_cast<const t_byte*>(p_data + d_ridx);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // reset this buffer but do not change the size
  
  void Buffer::reset (void) {
    wrlock ();
    try {
      d_blen = 0L;
      d_ridx = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the size of this buffer
  
  long Buffer::getsize (void) const {
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

  // return the length of this buffer
  
  long Buffer::length (void) const {
    rdlock ();
    try {
      long result = d_blen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the buffer encoding mode

  void Buffer::setemod (const Encoding::t_emod emod) {
    wrlock ();
    try {
      d_emod = emod;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the buffer encoding

  Encoding::t_emod Buffer::getemod (void) const {
    rdlock ();
    try {
      Encoding::t_emod result = d_emod;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the resize flag

  void Buffer::setrflg (const bool rflg) {
    wrlock ();
    try {
      d_rflg = rflg;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the resize flag

  bool Buffer::getrflg (void) const {
    rdlock ();
    try {
      bool result = d_rflg;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the buffer is empty

  bool Buffer::empty (void) const {
    rdlock ();
    try {
      bool result = (d_blen == 0L);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the buffer is full

  bool Buffer::full (void) const {
    rdlock ();
    try {
      bool result = d_rflg ? false : (d_blen >= d_size);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a character in this buffer
  
  long Buffer::add (const char value) {
    wrlock ();
    try {
      // check if we normalize
      if (d_ridx >= (d_size / 2)) normalize ();
      // first check if we are at the buffer end
      if ((d_blen + d_ridx) >= d_size) {
	if (d_rflg == true) {
	  long size = d_size * 2;
	  char* buf = new char[size];
	  for (long k = 0L; k < d_blen; k++) buf[k] = p_data[k + d_ridx];
	  delete [] p_data;
	  d_size = size;
	  d_ridx = 0L;
	  p_data = buf;
	} else {
	  unlock ();
	  return 0L;
	}
      }
      p_data[d_ridx + d_blen++] = value;
      unlock ();
      return 1L;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a unicode character in the buffer

  long Buffer::add (const t_quad value) {
    wrlock ();
    char* cbuf = nullptr;
    try {
      // get the character encoding
      cbuf = Unicode::encode (d_emod, value);
      long  size = Ascii::strlen (cbuf);
      // add the coding in the buffer
      long result = add (cbuf, size);
      // clean and unlock
      delete [] cbuf;
      unlock ();
      return result;
    } catch (...) {
      delete [] cbuf;
      unlock ();
      throw;
    }
  }

  // add a word to this buffer

  void Buffer::add (const t_word wval, const bool hflg) {
    wrlock ();
    try {
      if (hflg == false) {
	add ((char) ((wval >> 8) & 0x00FFU));
	add ((char) (wval & 0x00FFU));
      } else {
	// convert the word in network format
	t_byte buf[2];
	c_whton (wval, buf);
	// add the byte array to the buffer
	add ((const char*) &buf[0], 2);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a quad  to this buffer

  void Buffer::add (const t_quad qval, const bool hflg) {
    wrlock ();
    try {
      if (hflg == false) {
	add ((char) ((qval >> 24) & 0x000000FFU));
	add ((char) ((qval >> 16) & 0x000000FFU));
	add ((char) ((qval >> 8)  & 0x000000FFU));
	add ((char) (qval & 0x000000FFU));
      } else {
	// convert the quad in network format
	t_byte buf[4];
	c_qhton (qval, buf);
	// add the byte array to the buffer
	add ((const char*) &buf[0], 4);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an octa in natural format to this buffer

  void Buffer::add (const t_octa oval, const bool hflg) {
    wrlock ();
    try {
      if (hflg == false) {
	add ((char) ((oval >> 56) & 0x00000000000000FFU));
	add ((char) ((oval >> 48) & 0x00000000000000FFU));
	add ((char) ((oval >> 40) & 0x00000000000000FFU));
	add ((char) ((oval >> 32) & 0x00000000000000FFU));
	add ((char) ((oval >> 24) & 0x00000000000000FFU));
	add ((char) ((oval >> 16) & 0x00000000000000FFU));
	add ((char) ((oval >> 8)  & 0x00000000000000FFU));
	add ((char) (oval & 0x00000000000000FFU));
      } else {
	t_byte buf[8];
	c_ohton (oval, buf);
	// add the byte array to the buffer
	add ((const char*) &buf[0], 8);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a c-string in this buffer

  long Buffer::add (const char* s) {
    wrlock ();
    try {
      long size = Ascii::strlen (s);
      long result = add (s, size);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a character buffer in this buffer
  
  long Buffer::add (const char* cbuf, const long size) {
    if ((cbuf == nullptr) || (size == 0)) return 0;
    wrlock ();
    try {
      long result = 0L;
      for (long k = 0L; k < size; k++) {
	result += add (cbuf[k]);
	if (full () == true) break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a string in this buffer
  
  long Buffer::add (const String& s) {
    wrlock ();
    char* cbuf = nullptr;
    try {
      // encode the string
      cbuf = Unicode::encode (d_emod, s);
      long  size = Ascii::strlen (cbuf);
      // add the buffer
      long result = add (cbuf, size);
      // clean and unlock
      delete [] cbuf;
      unlock ();
      return result;
    } catch (...) {
      delete [] cbuf;
      unlock ();
      throw;
    }
  }

  // add a buffer object to this buffer

  long Buffer::add (const Buffer& buffer) {
    // do not add yourself
    if (this == &buffer) return 0;
    // lock and add
    wrlock ();
    buffer.rdlock ();
    try {
      long result = add (buffer.p_data + buffer.d_ridx, buffer.d_blen);
      buffer.unlock ();
      unlock ();
      return result;
    } catch (...) {
      buffer.unlock ();
      unlock ();
      throw;
    }
  }
  
  // read a character in this buffer
  
  char Buffer::read (void) {
    wrlock ();
    try {
      // check for no character
      if (d_blen == 0L) {
	unlock ();
	return nilc;
      }
      // get value and shift
      char value = p_data[d_ridx++];
      d_blen--;
      unlock ();
      return value;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read the next available byte

  t_byte Buffer::readb (void) {
    wrlock();
    try {
      t_byte result = (t_byte) read ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // read the next available word

  t_word Buffer::readw (const bool hflg) {
    wrlock ();
    try {
      // check valid buffer size
      if (d_blen < 2) {
	throw Exception ("buffer-error", "small buffer size with readw");
      }
      // prepare result
      t_word result = nilw;
      if (hflg == false) {
	// read and shift
	result |= (t_byte) read ();
	result <<= 8;
	result |= (t_byte) read ();
      } else {
	// extract the character buffer
	t_byte buf[2];
	for (long i = 0; i < 2; i++) buf[i] = (t_byte) read ();
	// convert it in host format
	result = c_wntoh (buf);
      }
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read the next available quad

  t_quad Buffer::readq (const bool hflg) {
    wrlock ();
    try {
      // check valid buffer size
      if (d_blen < 4) {
	throw Exception ("buffer-error", "small buffer size with readq");
      }
      // prepare result
      t_quad result = nilq;
      if (hflg == false) {
	// read and shift
	result |= (t_byte) read ();
	result <<= 8;
	result |= (t_byte) read ();
	result <<= 8;
	result |= (t_byte) read ();
	result <<= 8;
	result |= (t_byte) read ();
      } else {
	// extract the character buffer
	t_byte buf[4];
	for (long i = 0; i < 4; i++) buf[i] = (t_byte) read ();
	// convert it in host format
	result = c_qntoh (buf);
      }
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read the next available octa

  t_octa Buffer::reado (const bool hflg) {
    wrlock ();
    try {
      // check valid buffer size
      if (d_blen < 8) {
	throw Exception ("buffer-error", "small buffer size with reado");
      }
      // prepare result
      t_octa result = nilo;
      if (hflg == false) {
	// read and shift
	result |= (t_byte) read ();
	result <<= 8;
	result |= (t_byte) read ();
	result <<= 8;
	result |= (t_byte) read ();
	result <<= 8;
	result |= (t_byte) read ();
	result <<= 8;
	result |= (t_byte) read ();
	result <<= 8;
	result |= (t_byte) read ();
	result <<= 8;
	result |= (t_byte) read ();
	result <<= 8;
	result |= (t_byte) read ();
      } else {
	t_byte buf[8];
	for (long i = 0; i < 8; i++) buf[i] = (t_byte) read ();
	// convert it in host format
	result = c_ontoh (buf);
      }
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the next character but do not remove it

  char Buffer::get (void) const {
    rdlock ();
    try {
      char result = (d_blen == 0L) ? nilc : p_data[d_ridx];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a character by index

  char Buffer::get (const long index) const {
    rdlock ();
    try {
      if ((index < 0L) || (index >= d_blen)) {
	throw Exception ("range-error", "out-of-bound buffer index");
      }
      char result = p_data[index + d_ridx];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a character by index

  void Buffer::set (const long index, const char value) {
    wrlock ();
    try {
      if ((index < 0L) || (index >= d_blen)) {
	throw Exception ("range-error", "out-of-bound buffer index");
      }
      p_data[index + d_ridx] = value;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // pop the last character if any

  char Buffer::pop (void) {
    wrlock ();
    try {
      // check for nil
      if ((d_size == 0L) || (d_blen == 0L)) {
	unlock ();
	return nilc;
      }
      char result = p_data[d_ridx + --d_blen];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move the buffer content into another one
  
  long Buffer::copy (char* rbuf, const long size) {
    // check argument first
    if ((rbuf == nullptr) || (size <= 0)) return 0;
    // lock and copy
    wrlock ();
    try {
      // initialize result
      long blen = (size <= d_blen) ? size : d_blen;
      // copy by loop
      for (long k = 0L; k < blen; k++) rbuf[k] = p_data[k + d_ridx];
      // adjust buffer content and size
      if (blen < d_blen) {
	d_ridx += blen;
	d_blen -= blen;
      } else {
	d_blen = 0L;
	d_ridx = 0L;
      }
      // clean the remaining buffer
      if (size > blen) {
	for (long k = blen; k < size; k++) rbuf[k] = nilc;
      }
      // done
      unlock ();
      return blen;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move the buffer content into another one
  
  Buffer Buffer::move (const long size) {
    // lock and copy
    wrlock ();
    try {
      // check valid argument
      if (size <= 0L) {
	throw Exception ("buffer-error", "invalid buffer size to move");
      }
      // initialize result
      long blen = (size <= d_blen) ? size : d_blen;
      // create target buffer and move
      Buffer result (size);
      // copy by loop
      for (long k = 0L; k < blen; k++) result.p_data[k] = p_data[k + d_ridx];
      // nullify the rest of the buffer
      if (size > blen) {
        for (long k = blen; k < size; k++) result.p_data[k] = nilc;
      }
      // adjust buffer content and size
      if (blen < d_blen) {
	d_ridx += blen;
	d_blen -= blen;
      } else {
	d_blen = 0L;
	d_ridx = 0L;
      }
      result.d_blen = size;
      // done
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // shift the buffer by a certain amount

  void Buffer::shl (const long asl) {
    wrlock ();
    try {
      // normalize the buffer
      normalize ();
      // check for amount
      if (asl < d_blen) {
	// shift the old buffer
	long  blen = d_blen - asl;
	long  size = d_rflg ? d_size : blen;
	char* data = new char[size];
	for (long i = asl; i < d_blen; i++) data[i-asl] = p_data[i];
	// adjust indexes
	delete [] p_data;
	d_size = size;
	d_blen = blen;
	p_data = data;
      } else {
	d_blen = 0;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // trim the buffer to a certain size

  bool Buffer::trim (const long size, const bool rflg) {
    wrlock ();
    try {
      // check for valid trim size
      if ((size <= 0L) || (d_blen < size)) {
	unlock ();
	return false;
      }
      // normalize the buffer
      normalize ();
      // check if we adjust to the size
      if (rflg == false) {
	// copy the old buffer
	char* data = new char[size];
	for (long k = 0L; k < size; k++) data[k] = p_data[k];
	delete [] p_data;
	d_size = size;
	d_blen = size;
	p_data = data;
	d_rflg = false;
      } else {
	d_blen = size;
	d_rflg = true;
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // read a line from this buffer

  String Buffer::readln (void) {
    wrlock ();
    try {
      // create a buffer to accumulate characters
      Buffer buf = d_emod;
      bool   flg = false;
      // read the character in the buffer
      while (empty () == false) {
	char c = read ();
	if (c == crlc) {
	  flg = true;
	  continue;
	}
	if (c == eolc) break;
	if (flg == true) {
	  buf.add (crlc);
	  flg = false;
	}
	buf.add (c);
      }
      unlock ();
      return buf.tostring ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // pushback a character in this buffer
  
  long Buffer::pushback (const char value) {
    wrlock ();
    try {
      // check if we are full
      if (((d_blen + d_ridx) >= d_size) && (d_ridx == 0L)) {
	if (d_rflg == true) {
	  long size = d_size * 2;
	  char* buf = new char[size];
	  for (long k = 0L; k < d_blen; k++) buf[k] = p_data[k + d_ridx];
	  d_size = size;
	  d_ridx = 0L;
	  delete [] p_data;
	  p_data = buf;
	} else {
	  unlock ();
	  return 0;
	}
      }
      // shift the buffer by one if needed
      if (d_ridx > 0L) {
	p_data[--d_ridx] = value;
      } else {
	for (long i = d_blen; i > 0; i--) p_data[i] = p_data[i-1];
	p_data[0] = value;
      }
      d_blen++;
      unlock ();
      return 1L;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pushback a unicode character in the buffer

  long Buffer::pushback (const t_quad value) {
    wrlock ();
    char* cbuf = nullptr;
    try {
      // get the character encoding
      cbuf = Unicode::encode (d_emod, value);
      long size = Ascii::strlen (cbuf);
      // pushback the coding in the buffer
      pushback (cbuf, size);
      // clean and unlock
      delete [] cbuf;
      unlock ();
      return size;
    } catch (...) {
      delete [] cbuf;
      unlock ();
      throw;
    }
  }

  // pushback a character string to this buffer

  long Buffer::pushback (const char* s) {
    wrlock ();
    try {
      long   size = Ascii::strlen (s);
      long result = pushback (s, size);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pushback a buffer in this buffer
  
  long Buffer::pushback (const char* s, const long size) {
    if ((s == nullptr) || (size == 0)) return 0;
    wrlock ();
    try {
      long    len = size - 1L;
      long result = 0L;
      for (long k = len; k >= 0; k--) {
	result += pushback (s[k]);
	if (full () == true) break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // pushback a string in this buffer
  
  long Buffer::pushback (const String& value) {
    wrlock ();
    char* cbuf = nullptr;
    try {
      // encode the string
      cbuf = Unicode::encode (d_emod, value);
      long  size = Ascii::strlen (cbuf);
      // pushback the string
      long result = pushback (cbuf, size);
      // clean and unlock
      delete [] cbuf;
      unlock ();
      return result;
    } catch (...) {
      delete [] cbuf;
      unlock ();
      throw;
    }
  }

  // pushback a buffer in this buffer
  
  long Buffer::pushback (const Buffer& buffer) {
    // do not pushback yourself
    if (this == &buffer) return 0;
    // lock and push
    wrlock ();
    buffer.rdlock ();
    try {
      long result = pushback (buffer.p_data + buffer.d_ridx, buffer.d_blen);
      buffer.unlock ();
      unlock ();
      return result;
    } catch (...) {
      buffer.unlock ();
      unlock ();
      throw;
    }
  }

  // collect a buffer by size

  Buffer Buffer::collect (const long size) {
    wrlock ();
    try {
      // check for valid arguments
      if (size <= 0L) {
	throw Exception ("buffer-error", "invalid size in collect");
      }
      // compute collect length
      long blen = (size <= d_blen) ? size : d_blen;
      // create a result buffer by size
      Buffer result (size);
      // copy in the result buffer
      for (long k = 0L; k < blen; k++) {
	result.p_data[k] = p_data[d_blen + d_ridx - blen + k];
      }
      // nullify the rest of the buffer
      if (size > blen) {
	for (long k = blen; k < size; k++) result.p_data[k] = nilc;
      }
      // update the local buffer
      d_blen -= blen;
      // update the result buffer
      result.d_blen = size;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // extract a buffer by position and size

  Buffer Buffer::extract (const long boff, const long size) const {
    rdlock ();
    try {
      // check for valid arguments
      if ((size <= 0L) || (boff < 0L)) {
	throw Exception ("buffer-error", "invalid size/offset in extract");
      }
      // compute copy length
      long blen = (size <= (d_blen - boff)) ? size : (d_blen - boff);
      // create a result buffer by size
      Buffer result (size);
      // copy in the result buffer
      for (long k = 0L; k < blen; k++) {
	result.p_data[k] = p_data[k + d_ridx + boff];
      }
      // nullify the rest of the buffer
      if (size > blen) {
	for (long k = blen; k < size; k++) result.p_data[k] = nilc;
      }
      // update the result buffer
      result.d_blen = size;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the buffer content

  char* Buffer::tochar (void) const {
    rdlock ();
    try {
      char* result = (d_blen == 0L) ? nullptr : new char[d_blen];
      for (long k = 0L; k < d_blen; k++) result[k] = p_data[k];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // return the buffer content as a null terminated quad buffer

  t_quad* Buffer::toquad (void) const {
    rdlock ();
    try {
      t_quad* result = Unicode::decode (d_emod, p_data + d_ridx, d_blen);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the buffer content as a string
  
  String Buffer::tostring (void) const {
    rdlock ();
    try {
      // decode the buffer
      t_quad* sbuf = Unicode::decode (d_emod, p_data + d_ridx, d_blen);
      // map the result string
      String result = sbuf;
      // clean and return
      delete [] sbuf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format the buffer content as an octet string

  String Buffer::format (void) const {
    rdlock ();
    try {
      // format the string
      String result = Ascii::btos ((const t_byte*) p_data, d_blen);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // map this buffer to an anonymous data structure

  long Buffer::tomap (void* data, const long size) const {
    rdlock ();
    try {
      // check for a null content
      if (d_blen == 0) {
	unlock ();
	return 0;
      }
      // process normal size
      long result = (size < d_blen) ? size : d_blen;
      char* ptr   = reinterpret_cast <char*> (data);
      // copy the buffer content
      for (long k = 0L; k < result; k++) ptr[k] = p_data[k + d_ridx];
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // copy a buffer upto a boundary string
  
  Buffer* Buffer::cpbnds (const String& bnds) {
    wrlock ();
    char* cbuf = Unicode::encode (Encoding::EMOD_UTF8, bnds);
    long  clen = Ascii::strlen (cbuf);
    try {
      // normalize the buffer
      normalize ();
      // reset matching flag
      bool mflg = false;
      long mpos = 0;
      // loop into the buffer
      for (mpos = 0; mpos < d_blen; mpos++) {
	// check for matching
	if (p_data[mpos] != cbuf[0]) continue;
	// try to match
	mflg = true;
	for (long l = 0; l < clen; l++) {
	  long j = mpos + l;
	  if ((j >= d_blen) || (p_data[j] != cbuf[l])) {
	    mflg = false;
	    break;
	  }
	}
	if (mflg == true) break;
      }
      // this a a double check
      if (mpos >= d_blen) {
	throw Exception ("internal-error", "inconsistent buffer copy position");
      }
      // we have a matching at position mpos - which is the length of the
      // new buffer to copy
      Buffer* result = new Buffer (mpos);
      result->setrflg (false);
      // copy to the new buffer
      for (long k = 0; k < mpos; k++) result->p_data[k] = p_data[k];
      result->d_blen = mpos;
      // clean boundary buffer
      delete [] cbuf;
      // update the buffer by shifting upto mpos position
      for (long k = mpos; k < d_blen; k++) {
	long l = k - mpos;
	p_data[l] = p_data[k];
      }
      d_blen -= mpos;
      // done
      unlock ();
      return result;
    } catch (...) {
      delete [] cbuf;
      unlock ();
      throw;
    }
  }

  // remove the last cr/nl characters

  bool Buffer::rmcrnl (void) {
    wrlock ();
    try {
      // check for nil
      if ((d_size == 0L) || (d_blen == 0L)) {
	unlock ();
	return false;
      }
      // normalize the buffer
      normalize ();
      // check for 1 character
      if (d_blen == 1) {
	if (p_data[0] == eolc) {
	  d_blen--;
	  unlock ();
	  return true;
	} else {
	  unlock ();
	  return false;
	}
      }
      // check for cr/nl
      if ((p_data[d_blen-2] == crlc) && (p_data[d_blen-1] == eolc)) {
	d_blen-= 2;
	unlock ();
	return true;
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // normalize the buffer

  void Buffer::normalize (void) {
    wrlock ();
    try {
      if ((d_size == 0L) || (d_blen == 0L) || (d_ridx == 0L)) {
	unlock ();
	return;
      }
      for (long k = 0L; k < d_blen; k++) p_data[k] = p_data[k + d_ridx];
      d_ridx = 0L;
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
  static const long QUARK_ZONE_LENGTH = 20;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_ADD      = zone.intern ("add");
  static const long QUARK_GET      = zone.intern ("get");
  static const long QUARK_SHL      = zone.intern ("shl");
  static const long QUARK_READ     = zone.intern ("read");
  static const long QUARK_MOVE     = zone.intern ("move");
  static const long QUARK_RESET    = zone.intern ("reset");
  static const long QUARK_READW    = zone.intern ("read-word");
  static const long QUARK_READQ    = zone.intern ("read-quad");
  static const long QUARK_READO    = zone.intern ("read-octa");
  static const long QUARK_LENGTH   = zone.intern ("length");
  static const long QUARK_ISFULL   = zone.intern ("full-p");
  static const long QUARK_EMPTYP   = zone.intern ("empty-p");
  static const long QUARK_FORMAT   = zone.intern ("format");
  static const long QUARK_CPBNDS   = zone.intern ("copy-boundary"); 
  static const long QUARK_COLLECT  = zone.intern ("collect");
  static const long QUARK_EXTRACT  = zone.intern ("extract");
  static const long QUARK_GETSIZE  = zone.intern ("get-size");
  static const long QUARK_SETRFLG  = zone.intern ("set-resize");
  static const long QUARK_TOSTRING = zone.intern ("to-string");
  static const long QUARK_PUSHBACK = zone.intern ("pushback");
  static const long QUARK_ISRESIZE = zone.intern ("resize-p");

  // create a new object in a generic way

  Object* Buffer::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create an empty buffer with 0 arguments
    Buffer* result = new Buffer;
    // loop with objects
    for (long i = 0; i < argc; i++) {
      Object* obj = argv->get (i);
      // check for a viewable
      auto wobj = dynamic_cast <Viewable*> (obj);
      if (wobj != nullptr) {
	result->add ((const char*) wobj->tobyte(), wobj->tosize ());
	continue;
      }
      // check for a literal
      auto lobj = dynamic_cast <Literal*> (obj);
      if (lobj != nullptr) {
	result->add (lobj->tostring ());
	continue;
      }
      // check for a vector
      auto vobj = dynamic_cast <Vector*> (obj);
      if (vobj != nullptr) {
	long vlen = vobj->length ();
	for (long j = 0; j < vlen; j++) {
	  result->add ((char) vobj->getbyte (j));
	}
	continue;
      }
      throw Exception ("type-error", "invalid object with buffer",
		       Object::repr (obj));
    }
    return result;
  }

  // return true if the given quark is defined

  bool Buffer::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }      
      bool result = hflg ? Serial::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Buffer::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GET)      return new Byte    (get      ());
      if (quark == QUARK_READ)     return new Byte    (read     ());
      if (quark == QUARK_LENGTH)   return new Integer (length   ());
      if (quark == QUARK_ISFULL)   return new Boolean (full     ());
      if (quark == QUARK_EMPTYP)   return new Boolean (empty    ());
      if (quark == QUARK_FORMAT)   return new String  (format   ());
      if (quark == QUARK_GETSIZE)  return new Integer (getsize  ());
      if (quark == QUARK_TOSTRING) return new String  (tostring ());
      if (quark == QUARK_ISRESIZE) return new Boolean (getrflg  ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GET) {
	long index = argv->getlong (0);
	return new Byte (get (index));
      }
      if (quark == QUARK_READW) {
	bool hflg = argv->getbool (0);
	return new Integer (readw (hflg));
      }
      if (quark == QUARK_READQ) {
	bool hflg = argv->getbool (0);
	return new Integer (readq (hflg));
      }
      if (quark == QUARK_READO) {
	bool hflg = argv->getbool (0);
	return new Integer (reado (hflg));
      }
      if (quark == QUARK_SETRFLG) {
	bool rflg = argv->getbool (0);
	setrflg (rflg);
	return nullptr;
      }
      if (quark == QUARK_MOVE) {
	long size = argv->getlong (0);
	return new Buffer (move (size));
      }
      if (quark == QUARK_ADD) {
	Object* obj = argv->get (0);
	// check for a byte
	auto bobj = dynamic_cast<Byte*> (obj);
	if (bobj != nullptr) {
	  long result = add ((char) bobj->tobyte ());
	  return new Integer (result);
	}
	// check for a buffer
	auto uobj = dynamic_cast<Buffer*> (obj);
	if (uobj != nullptr) {
	  long result = add (*uobj);
	  return new Integer (result);
	}
	// check for a viewable
	auto wobj = dynamic_cast<Viewable*> (obj);
	if (wobj != nullptr) {
	  long result = add ((char*) wobj->tobyte (), wobj->tosize ());
	  return new Integer (result);
	}
	// check for a literal
	auto lobj = dynamic_cast<Literal*> (obj);
	if (lobj != nullptr) {
	  long result = add (lobj->tostring ());
	  return new Integer (result);
	}
	throw Exception ("type-error", "invalid object to add in buffer");
      }
      if (quark == QUARK_PUSHBACK) {
	Object* obj = argv->get (0);
	// check for a byte
	auto bobj = dynamic_cast<Byte*> (obj);
	if (bobj != nullptr) {
	  long result = pushback ((char) bobj->tobyte ());
	  return new Integer (result);
	}
	// check for a buffer
	auto uobj = dynamic_cast<Buffer*> (obj);
	if (uobj != nullptr) {
	  long result = pushback (*uobj);
	  return new Integer (result);
	}
	// check for a viewable
	auto wobj = dynamic_cast<Viewable*> (obj);
	if (wobj != nullptr) {
	  long result = pushback ((char*) wobj->tobyte (), wobj->tosize());
	  return new Integer (result);
	}
	// check for a literal
	auto lobj = dynamic_cast<Literal*> (obj);
	if (lobj != nullptr) {
	  long result = pushback (lobj->tostring ());
	  return new Integer (result);
	}
	throw Exception ("type-error", "invalid object to pushback in buffer");
      }
      if (quark == QUARK_COLLECT) {
	long size = argv->getlong (0);
	return new Buffer (collect (size));
      }
      if (quark == QUARK_EXTRACT) {
	long size = argv->getlong (0);
	return new Buffer (extract (0L, size));
      }
      if (quark == QUARK_SHL) {
	long asl = argv->getlong (0);
	shl (asl);
	return nullptr;
      }
      if (quark == QUARK_CPBNDS) {
	String bnds = argv->getstring (0);
	return cpbnds (bnds);
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_EXTRACT) {
	long boff = argv->getlong (0);
	long size = argv->getlong (1);
	return new Buffer (extract (boff, size));
      }
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
