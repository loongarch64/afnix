// ---------------------------------------------------------------------------
// - Strbuf.cpp                                                              -
// - standard object library - string buffer class implementation            -
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

#include "Ascii.hpp"
#include "Strbuf.hpp"
#include "Unicode.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // default buffer size
  const long BUFFER_SIZE = 1024;

  // this procedure clean a string buffer by length
  static void clrbuf (t_quad** buf, const long len) {
    if ((buf == nullptr) || (len <= 0)) return;
    for (long i = 0; i < len; i++) delete [] buf[i];
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new buffer

  Strbuf::Strbuf (void) {
    d_size   = BUFFER_SIZE;
    p_buffer = new t_quad*[BUFFER_SIZE];
    d_length = 0;
  }

  // create a new buffer with a predefined size

  Strbuf::Strbuf (const long size) {
    d_size   = (size <= 0) ? BUFFER_SIZE : size;
    p_buffer = new t_quad*[d_size];
    d_length = 0;
  }

  // create a new buffer and initialize it with a c-string

  Strbuf::Strbuf (const char* value) {
    d_size   = BUFFER_SIZE;
    p_buffer = new t_quad*[BUFFER_SIZE];
    d_length = 0;
    add (value);
  }

  // create a new buffer and initialize it with a string

  Strbuf::Strbuf (const String& value) {
    d_size   = BUFFER_SIZE;
    p_buffer = new t_quad*[BUFFER_SIZE];
    d_length = 0;
    add (value);
  }
  
  // destroy this strbuf
  
  Strbuf::~Strbuf (void) {
    clrbuf (p_buffer, d_length);
    delete [] p_buffer;
  }
  
  // return the class name

  String Strbuf::repr (void) const {
    return "Strbuf";
  }

  // reset this buffer
  
  void Strbuf::reset (void) {
    wrlock ();
    clrbuf (p_buffer, d_length);
    d_length = 0;
    unlock ();
  }

  // return the length of this buffer
  
  long Strbuf::length (void) const {
    rdlock ();
    long result = 0;
    for (long i = 0; i < d_length; i++) {
      result += Unicode::strlen (p_buffer[i]);
    }
    unlock ();
    return result;
  }

  // return the non combining length of this buffer

  long Strbuf::ncclen (void) const {
    rdlock ();
    long result = d_length;
    unlock ();
    return result;
  }

  // resize this buffer

  void Strbuf::resize (const long size) {
    wrlock ();
    if ((size < 0) || (size < d_length)) {
      unlock ();
      return;
    }
    // allocate and copy the new buffer
    t_quad** buf = new t_quad*[size];
    for (long i = 0; i < d_length; i++) buf[i] = p_buffer[i];
    delete [] p_buffer;
    // restore new buffer
    d_size   = size;
    p_buffer = buf;
    unlock ();
  }

  // add a character in this buffer

  void Strbuf::add (const char value) {
    wrlock ();
    try {
      add (Unicode::toquad (value));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a unicode character in this buffer
  
  void Strbuf::add (const t_quad value) {
    wrlock ();
    try {
      // check for combining class
      if (Unicode::isncc (value) == true) {
	// normalize the charcater
	t_quad* buf = Unicode::strmak (value);
	if (buf == nullptr) {
	  unlock ();
	  return;
	}
	// check for resize
	if (d_length == d_size) resize (d_size * 2);
	// add the charcater buffer
	p_buffer[d_length++] = buf;
      } else {
	// check if we have a non combining character
	// we do not add floating combining characters
	if (d_length > 0) {
	  long index = d_length - 1;
	  t_quad* buf = Unicode::strmak (p_buffer[index], value);
	  delete [] p_buffer[index];
	  p_buffer[index] = buf;
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a unicode character at a certain position

  void Strbuf::add (const t_quad value, const long pos) {
    // check for position
    if (pos < 0) return;
    wrlock ();
    try {
      // check for add at end
      if (pos >= d_length) {
	add (value);
	unlock ();
	return;
      }
      if (Unicode::isncc (value) == true) {
	t_quad* buf = Unicode::strmak (value);
	if (buf == nullptr) {
	  unlock ();
	  return;
	}
	// here we are in the middle - so we need to shift first
	// and the add in position - but first we check for resize
	if (d_length == d_size) resize (d_size * 2);
	for (long i = d_length; i > pos; i--) p_buffer[i] = p_buffer[i-1];
	// the insert slot is free so we can add in position
	p_buffer[pos] = buf;
      } else {
	t_quad* buf = Unicode::strmak (p_buffer[pos], value);
	delete [] p_buffer[pos];
	p_buffer[pos] = buf;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a character buffer in this buffer

  void Strbuf::add (const char* s) {
    wrlock ();
    try {
      long size = Ascii::strlen (s);
      add (s, size);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a character buffer in this buffer by size
  
  void Strbuf::add (const char* s, const long size) {
    if ((s == nullptr) || (size == 0)) return;
    wrlock ();
    try {
      for (long i = 0; i < size; i++) add (s[i]);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a unicode buffer in this buffer

  void Strbuf::add (const t_quad* s) {
    wrlock ();
    try {
      long size = Unicode::strlen (s);
      add (s, size);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a unicode buffer in this buffer by size
  
  void Strbuf::add (const t_quad* s, const long size) {
    if ((s == nullptr) || (size == 0)) return;
    wrlock ();
    try {
      for (long i = 0; i < size; i++) add (s[i]);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a string in this buffer
  
  void Strbuf::add (const String& s) {
    wrlock ();
    try {
      // get the quad representation
      t_quad* buf = s.toquad ();
      // add the buffer and clean
      add (buf);
      delete [] buf;
    } catch (...) {
      unlock ();
      throw;
    }
    unlock ();
  }

  // put a unicode character at a certain position

  void Strbuf::put (const t_quad value, const long pos) {
    // check for position
    if (pos < 0) return;
    wrlock ();
    try {
      // check for add at end
      if (pos >= d_length) {
	add (value);
	unlock ();
	return;
      }
      if (Unicode::isncc (value) == true) {
	t_quad* buf = Unicode::strmak (value);
	if (buf == nullptr) {
	  unlock ();
	  return;
	}
	// here we are in the middle - so we replace in position
	delete [] p_buffer[pos];
	p_buffer[pos] = buf;
      } else {
	t_quad* buf = Unicode::strmak (p_buffer[pos], value);
	delete [] p_buffer[pos];
	p_buffer[pos] = buf;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
   
  // remove a character at a certain position

  void Strbuf::chdel (const long pos) {
    // check for position
    if (pos < 0) return;
    wrlock ();
    try {
      // check if we have something to delete
      if (d_length == 0) {
	unlock ();
	return;
      }
      // try to delete at the end
      if (pos >= d_length) {
	delete [] p_buffer[--d_length];
	unlock ();
	return;
      }
      // delete in place and shift
      delete [] p_buffer[pos];
      for (long i = pos; i < d_length; i++) p_buffer[i] = p_buffer[i+1];
      d_length--;
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
 
  // return the corresponding string accumulated in this buffer
  
  String Strbuf::tostring (void) const {
    rdlock ();
    // create a temporary buffer to hold the characters
    long    len = length ();
    t_quad* buf = new t_quad[len+1];
    // build the result string
    try {
      long idx = 0;
      for (long i = 0; i < d_length; i++) {
	t_quad* s = p_buffer[i];
	while (*s != nilq) buf[idx++] = *s++;	
      }
      buf[idx] = nilq;
      String result = buf;
      delete [] buf;
      unlock ();
      return result;
    } catch (...) {
      delete [] buf;
      unlock ();
      throw;
    }
  }

  // return a substring from a position to the end

  String Strbuf::substr (const long pos) const {
    rdlock ();
    // check for good position
    if (d_length - pos <= 0) {
      String result;
      unlock ();
      return result;
    }
    // allocate a buffer to store the result
    long    len = length ();
    t_quad* buf = new t_quad[len+1];
    // copy the characters in the buffer
    try {
      long idx = 0;
      for (long i = pos; i < d_length; i++) {
	t_quad* s = p_buffer[i];
	while (*s != nilq) buf[idx++] = *s++;	
      }
      buf[idx] = nilq;
      String result = buf;
      delete [] buf;
      unlock ();
      return result;
    } catch (...) {
      delete [] buf;
      unlock ();
      throw;
    }
  }
}
