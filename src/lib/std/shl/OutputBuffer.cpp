// ---------------------------------------------------------------------------
// - OutputBuffer.cpp                                                        -
// - standard object library - output buffer stream class implementation     -
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
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "OutputBuffer.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new default output buffer 

  OutputBuffer::OutputBuffer (void)  {
    d_sbuf.reset ();
  }

  // create a new output buffer with a value

  OutputBuffer::OutputBuffer (const String& value)  {
    d_sbuf.reset ();
    d_sbuf.add (value);
  }

  // return the class name

  String OutputBuffer::repr (void) const {
    return "OutputBuffer";
  }

  // set the stream encoding mode

  void OutputBuffer::setemod (const Encoding::t_emod emod) {
    wrlock ();
    try {
      // set the base class
      Stream::setemod (emod);
      // set the local buffer mode
      d_sbuf.setemod (emod);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // flush this output buffer
  
  void OutputBuffer::flush (void) {
    wrlock ();
    try {
      d_sbuf.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the length of the output buffer

  long OutputBuffer::length (void) const {
    rdlock ();
    try {
      long result = d_sbuf.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write one character to the output buffer
  
  long OutputBuffer::write (const char value) {
    wrlock ();
    try {
      d_sbuf.add (value);
      unlock ();
      return 1L;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a character string to the output buffer
  
  long OutputBuffer::write (const char* data) {
    wrlock ();
    try {
      long size = Ascii::strlen (data);
      if (size == 0L) {
	unlock ();
	return 0L;
      }
      char      c = nilc;
      long result = 0;
      while ((c = *data++) != nilc) {
	d_sbuf.add (c);
	result++;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return a buffer copy of the output buffer

  Buffer OutputBuffer::tobuffer (void) const {
    rdlock ();
    try {
      Buffer result = d_sbuf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a string representation of the output buffer

  String OutputBuffer::tostring (void) const {
    rdlock ();
    try {
      String result = d_sbuf.tostring ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return an octet string representation of the output buffer

  String OutputBuffer::format (void) const {
    rdlock ();
    try {
      String result = d_sbuf.format ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // send this output buffer to an output stream

  void OutputBuffer::send (OutputStream* os) {
    rdlock ();
    try {
      // write the buffer
      long asl = (os != nullptr) ? os->write (d_sbuf) : 0L;
      // shift the buffer content
      d_sbuf.shl (asl);
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
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_FLUSH    = zone.intern ("flush");
  static const long QUARK_LENGTH   = zone.intern ("length");
  static const long QUARK_FORMAT   = zone.intern ("format");
  static const long QUARK_TOSTRING = zone.intern ("to-string");

  // create a new object in a generic way

  Object* OutputBuffer::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new OutputBuffer;

    // check for 1 argument
    if (argc == 1) {
      String sval = argv->getstring (0);
      return new OutputBuffer (sval);
    }
    throw Exception ("argument-error", 
		     "invalid arguments with with output buffer"); 
  }

  // return true if the given quark is defined

  bool OutputBuffer::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? OutputStream::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* OutputBuffer::apply (Evaluable* zobj, Nameset* nset, const long quark,
			       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH)   return new Integer (length   ());
      if (quark == QUARK_FORMAT)   return new String  (format   ());
      if (quark == QUARK_TOSTRING) return new String  (tostring ());
      if (quark == QUARK_FLUSH) {
	flush ();
	return nullptr;
      }
    }
    // call the output method
    return OutputStream::apply (zobj, nset, quark, argv);
  }
}
