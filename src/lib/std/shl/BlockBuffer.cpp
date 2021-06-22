// ---------------------------------------------------------------------------
// - BlockBuffer.cpp                                                         -
// - standard object library - block buffer class implementation             -
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
#include "Vector.hpp"
#include "System.hpp"
#include "Unicode.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "BlockBuffer.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a default block buffer

  BlockBuffer::BlockBuffer (void) : Buffer (Encoding::EMOD_BYTE){
    // set the block buffer
    d_rcnt = 0LL;
    d_wcnt = 0LL;
    // force non resizable
    setrflg (false);
  }

  // create a block buffer by size

  BlockBuffer::BlockBuffer (const long size) :
    Buffer (size, Encoding::EMOD_BYTE) {
    // set the block buffer
    d_rcnt = 0LL;
    d_wcnt = 0LL;
    // force non resizable
    setrflg (false);
  }
    
  // return the class name

  String BlockBuffer::repr (void) const {
    return "BlockBuffer";
  }

  // reset this block buffer

  void BlockBuffer::reset (void) {
    wrlock ();
    try {
      // reset the base object
      Buffer::reset ();
      // reset the local members
      d_rcnt = 0LL;
      d_wcnt = 0LL;
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a character to this block buffer

  long BlockBuffer::add (const char value) {
    wrlock ();
    try {
      // add the character in the buffer
      long result = Buffer::add (value);
      // update the write counter
      d_wcnt += result;
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // get the next available character

  char BlockBuffer::read (void) {
    wrlock ();
    try {
      // read the next character
      char result = Buffer::read ();
      // update the read counter
      d_rcnt++;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // pushback a character to this block buffer

  long BlockBuffer::pushback (const char value) {
    wrlock ();
    try {
      // add the character in the buffer
      long result = Buffer::pushback (value);
      // update the write counter
      d_wcnt += result;
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // get the buffer read count

  t_long BlockBuffer::getrcnt (void) const {
    rdlock ();
    try {
      t_long result = d_rcnt;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the buffer write count

  t_long BlockBuffer::getwcnt (void) const {
    rdlock ();
    try {
      t_long result = d_wcnt;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // copy a character array into the block buffer

  long BlockBuffer::copy (const char* data, const long size) {
    wrlock ();
    try {
      // reset the buffer in bound mode
      if (full () == true) Buffer::reset ();
      // add the buffer data
      long result = Buffer::add (data, size);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a string in this buffer
  
  long BlockBuffer::copy (const String& s) {
    wrlock ();
    char* cbuf = nullptr;
    try {
      // encode the string
      cbuf = Unicode::encode (d_emod, s);
      long  size = Ascii::strlen (cbuf);
      // add the buffer
      long result = copy (cbuf, size);
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

  // copy a buffer into the block buffer

  long BlockBuffer::copy (Buffer& buffer) {
    wrlock ();
    try {
      // reset the buffer in bound mode
      if (full () == true) Buffer::reset ();
      // add the buffer
      long result = 0;
      while ((buffer.empty () == false) && (full () == false)) {
	result += add (buffer.read ());
      }
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // copy an input stream into the block buffer

  long BlockBuffer::copy (InputStream& is) {
    wrlock ();
    try {
      // initialize result
      long result = 0;
      // reset the buffer if full (i.e bound mode)
      if (full () == true) Buffer::reset ();
      // copy directly the block
      if (empty () == true ) {
	// copy directly by block
	result = is.copy (p_data, d_size);
	// update indexes
	d_blen  = result;
	d_wcnt += result;
      } else {
	// add the buffer data
	while ((is.valid () == true) && (full () == false)) {
	  result += add (is.read ());
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // copy a block buffer to an output stream

  long BlockBuffer::copy (OutputStream& os) {
    wrlock ();
    try {
      long result = 0;
      // loop until empty
      while (empty () == false) {
	// copy the buffer
	long size = os.write (p_data, d_blen);
	// adjust by shifting
	shl (size);
	// update counter
	result += size;
      }
      d_rcnt += result;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // copy an input stream into an output stream

  long BlockBuffer::copy (OutputStream& os, InputStream& is) {
    wrlock ();
    try {
      long result = 0;
      while (is.valid () == true) {
	// copy the input stream
	long rdsz = copy (is);
	// copy to the output stream
	long wrsz = copy (os);
	if (rdsz != wrsz) {
	  throw Exception ("buffer-error", "buffer copy size differences");
	}
	result += rdsz;
      }
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
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_COPY   = zone.intern ("copy");
  static const long QUARK_RCNT   = zone.intern ("read-count");
  static const long QUARK_WCNT   = zone.intern ("write-count");
  static const long QUARK_COPYIS = zone.intern ("copy-input-stream");
  static const long QUARK_COPYOS = zone.intern ("copy-output-stream");

  // create a new object in a generic way

  Object* BlockBuffer::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check 0 argument
    if (argc == 0) return new BlockBuffer;
    // check 1 argument
    if (argc == 1) {
      long size = argv->getlong (0);
      return new BlockBuffer (size);
    }
    throw Exception ("argument-error", "too many argument for block buffer");
  }

  // return true if the given quark is defined

  bool BlockBuffer::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }      
      bool result = hflg ? Buffer::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* BlockBuffer::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_RCNT) return new Integer (getrcnt ());
      if (quark == QUARK_WCNT) return new Integer (getwcnt ());
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_COPY) {
	Object* obj = argv->get (0);
	// check for a string
	String* sobj = dynamic_cast<String*> (obj);
	if (sobj != nullptr) {
	  long result = copy (*sobj);
	  return new Integer (result);
	}
	// check for a buffer
	Buffer* bobj = dynamic_cast<Buffer*> (obj);
	if (bobj != nullptr) {
	  long result = copy (*bobj);
	  return new Integer (result);
	}
	// check for an input stream
	InputStream* is = dynamic_cast<InputStream*> (obj);
	if (is != nullptr) {
	  long result = copy (*is);
	  return new Integer (result);
	}
	// check for an output stream
	OutputStream* os = dynamic_cast<OutputStream*> (obj);
	if (os != nullptr) {
	  long result = copy (*os);
	  return new Integer (result);
	}
	throw Exception ("type-error", 
			 "invalid object to copy in block buffer",
			 Object::repr (obj));
      }
      if (quark == QUARK_COPYIS) {
	Object* obj = argv->get (0);
	// check for an input stream
	InputStream* is = dynamic_cast<InputStream*> (obj);
	if (is != nullptr) {
	  long result = copy (*is);
	  return new Integer (result);
	}
	throw Exception ("type-error", 
			 "invalid object to copy in block buffer",
			 Object::repr (obj));
      }
      if (quark == QUARK_COPYOS) {
	Object* obj = argv->get (0);
	// check for an output stream
	OutputStream* os = dynamic_cast<OutputStream*> (obj);
	if (os != nullptr) {
	  long result = copy (*os);
	  return new Integer (result);
	}
	throw Exception ("type-error", 
			 "invalid object to copy in block buffer",
			 Object::repr (obj));
      }
    }

    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_COPY) {
	Object* oobj = argv->get (0);
	// check for an output stream
	OutputStream* os = dynamic_cast<OutputStream*> (oobj);
	if (os == nullptr) {
	  throw Exception ("type-error", 
			   "invalid object to copy in block buffer",
			   Object::repr (oobj));
	}
	// check for an input stream
	Object* iobj = argv->get (1);
	InputStream* is = dynamic_cast<InputStream*> (iobj);
	if (is == nullptr) {
	  throw Exception ("type-error", 
			   "invalid object to copy in block buffer",
			   Object::repr (iobj));
	}
	long result = copy (*os, *is);
	return new Integer (result);
      }
    }
    // call the buffer method
    return Buffer::apply (zobj, nset, quark, argv);
  }
}
