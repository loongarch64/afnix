// ---------------------------------------------------------------------------
// - InputMapped.cpp                                                         -
// - standard object library - mapped input stream class implementation      -
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
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputMapped.hpp"
#include "cmem.hpp"
#include "csio.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this function open a file by name and return a file descriptor
  static int open_input_file (const String& name) {
    // check the file name
    if (name.isnil () == true) {
      throw Exception ("name-error", "nil input file name");
    }
    // try to open the file
    char* fname = name.tochar ();
    int sid     = c_openr (fname);
    if (sid < 0) {
      delete [] fname;
      throw Exception ("open-error", "cannot open input file", name);
    }
    delete [] fname;
    return sid;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a null mapped input stream

  InputMapped::InputMapped (void) {
    p_mbuf = nullptr;
    d_anon = false;
    d_size = 0LL;
    d_mark = 0LL;
  }

  // create a new mapped input stream by name

  InputMapped::InputMapped (const String& name) {
    // open the input file by name
    int sid = open_input_file (name);
    // get the file size
    d_size = c_fsize (sid);
    // try to map it
    p_mbuf = (char*) c_mmap (sid, d_size, 0);
    c_close (sid);
    // check for valid buffer
    if (p_mbuf == nullptr) {
      throw Exception ("map-error", "cannot map input file", name);
    }
    // set the mapped data
    d_anon = false;
    d_mark = 0LL;
  }

  // create a new mapped input stream by buffer

  InputMapped::InputMapped (const Buffer& buf) {
    // get the buffer size
    d_size = buf.length ();
    // try to map the buffer
    p_mbuf = (d_size == 0) ? nullptr : new char[d_size];
    // map the buffer content
    d_size = buf.tomap (p_mbuf, d_size);
    // set the mapped data
    d_anon = true;
    d_mark = 0LL;
  }

  // create a new mapped input stream by bitset

  InputMapped::InputMapped (const Bitset& bset) {
    // get the bitset byte size
    d_size = bset.bsize ();
    // try to map the buffer
    p_mbuf = (d_size == 0) ? nullptr : new char[d_size];
    // map the buffer content
    d_size = bset.tomap (p_mbuf, d_size);
    // set the mapped data
    d_anon = true;
    d_mark = 0LL;
  }

  // create a new mapped input stream by name, size and offset

  InputMapped::InputMapped (const String& name, const t_long size, 
			    const t_long boff) {
    // open the input file by name
    int sid = open_input_file (name);
    // try to map it
    p_mbuf = (char*) c_mmap (sid, size, boff);
    c_close (sid);
    // check for valid buffer
    if (p_mbuf == nullptr) {
      throw Exception ("map-error", "cannot map input file", name);
    }
    // save map data
    d_anon = false;
    d_size = size;
    d_mark = 0;
  }

  // destroy this mapped input stream

  InputMapped::~InputMapped (void) {
    if (d_anon == true) {
      delete [] p_mbuf;
    } else {
      c_munmap (p_mbuf, d_size);
    }
  }

  // return the class name

  String InputMapped::repr (void) const {
    return "InputMapped";
  }

  // return true if the eos flag is set

  bool InputMapped::iseos (void) const {
    wrlock ();
    try {
      bool result = !valid ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if we can read a character

  bool InputMapped::valid (void) const {
    wrlock ();
    try {
      // check the pushback buffer
      if (d_sbuf.empty () == false) {
	unlock ();
	return true;
      }
      // check for a null mapped input stream
      if ((d_anon == false) && (p_mbuf == nullptr)) {
	unlock ();
	return true;
      }
      // check for any mode
      bool result = (d_mark >= d_size) ? false : true;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read one character from the input stream.

  char InputMapped::read (void) {
    wrlock ();
    try {
      // check the pushback buffer first
      if (d_sbuf.empty () == false) {
	long result = d_sbuf.read ();
	unlock ();
	return result;
      }
      // check for null mapped input stream
      if ((d_anon == false) && (p_mbuf == nullptr)) {
	unlock ();
	return nilc;
      }
      // check for eos in any mode
      if (d_mark >= d_size) {
	unlock ();
	return eosc;
      }
      // get next character
      char result = p_mbuf[d_mark++];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // copy an input stream into a buffer
  
  long InputMapped::copy (char* rbuf, const long size) {
    wrlock ();
    try {
      // start by copy the input buffer
      long result = InputBuffer::copy (rbuf, size);
      // check reminder
      if (result < size) {
	// compute delta
	long dlta = size - result;
	// compute available length
	long mlen = d_size - d_mark;
	// compute copy size
	long msiz = (dlta <= mlen) ? dlta : mlen;
	// perform the copy
	for (long k = 0L; k < msiz; k++) rbuf[result++] = p_mbuf[d_mark++];
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the input stream at a certain position

  void InputMapped::lseek (const t_long pos) {
    wrlock ();
    try {
      // check for null mapped input stream
      if ((d_anon == false) && (p_mbuf == nullptr)) {
	unlock ();
	return;
      }
      // check position in any mode
      if ((pos >= 0) && (pos < d_mark)) {
	d_mark = pos;
      } else {
	if (pos < 0)       d_mark = 0;
	if (pos >= d_size) d_mark = d_size;
      }
      // reset the pushback buffer
      d_sbuf.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the mapped input stream size

  t_long InputMapped::length (void) const {
    rdlock ();
    try {
      t_long result = d_size;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a buffer of this mapped stream

  Buffer* InputMapped::tobuffer (void) const {
    rdlock ();
    Buffer* result = nullptr;
    try {
      // the result buffer
      result = new Buffer (d_size);
      // copy its content
      result->add (p_mbuf, d_size);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_LSEEK  = zone.intern ("lseek");
  static const long QUARK_TOBUF  = zone.intern ("to-buffer");
  static const long QUARK_LENGTH = zone.intern ("length");

  // create a new object in a generic way

  Object* InputMapped::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new InputMapped;

    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new InputMapped (*sobj);
      // check for a buffer
      Buffer* bobj = dynamic_cast <Buffer*> (obj);
      if (bobj != nullptr) return new InputMapped (*bobj);
      // invalid object
      throw Exception ("type-error", "invalid object with mapped input strean",
		       Object::repr (obj));
    }
    // check for 3 arguments
    if (argc == 3) {
      String name = argv->getstring (0);
      t_long size = argv->getlong (1);
      t_long boff = argv->getlong (2);
      return new InputMapped (name, size, boff);
    }
    throw Exception ("argument-error", 
		     "too many arguments with mapped input stream ");
  }

  // return true if the given quark is defined

  bool InputMapped::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }      
      bool result = hflg ? InputStream::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* InputMapped::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOBUF)  return tobuffer ();
      if (quark == QUARK_LENGTH) return new Integer (length ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_LSEEK) {
	t_long pos = argv->getlong (0);
	lseek (pos);
	return nullptr;
      }
    }
    // fallback with the input stream method
    return InputStream::apply (zobj, nset, quark, argv);
  }
}
