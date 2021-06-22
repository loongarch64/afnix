// ---------------------------------------------------------------------------
// - InputBuffer.cpp                                                         -
// - standard object library - input stream buffer class implementation      -
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
#include "Exception.hpp"
#include "InputBuffer.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default input object

  InputBuffer::InputBuffer (void) {
    d_sbuf.reset ();
  }

  // set the stream encoding mode

  void InputBuffer::setemod (const Encoding::t_emod emod) {
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

  // flush the input stream buffer
  
  void InputBuffer::flush (void) {
    wrlock ();
    try {
      d_sbuf.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // copy an input stream into a buffer
  
  long InputBuffer::copy (char* rbuf, const long size) {
    wrlock ();
    try {
      long result = d_sbuf.copy (rbuf, size);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pushback a character in the input buffer

  long InputBuffer::pushback (const char value) {
    wrlock ();
    try {
      long result = d_sbuf.pushback (value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pushback a unicode character in the input buffer

  long InputBuffer::pushback (const t_quad value) {
    wrlock ();
    try {
      long result = d_sbuf.pushback (value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pushback a c-string in this input buffer

  long InputBuffer::pushback (const char* s) {
    wrlock ();
    try {
      long   size = Ascii::strlen (s);
      long result = d_sbuf.pushback (s, size);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pushback a c-string in this input buffer

  long InputBuffer::pushback (const char* s, const long size) {
    wrlock ();
    try {
      long result = 0;
      if (size > 0) {
	result = d_sbuf.pushback (s, size);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // consume a stream by reading and pushing back

  long InputBuffer::consume (void) {
    wrlock ();
    try {
      // create a temporary buffer
      Buffer buf;
      while (valid () == true) buf.add (read ());
      // save the buffer length
      long result = buf.length ();
      // merge with the stream buffer
      d_sbuf.add (buf);
      // here is the result
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the size of the input buffer

  long InputBuffer::buflen (void) const {
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
  
  // get a copy of the input buffer

  Buffer InputBuffer::toibuf (void) const {
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
  
  // return a string representation of the stream buffer

  String InputBuffer::tostring (void) const {
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

  // return the buffer content as an octet string

  String InputBuffer::format (void) const {
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
}
