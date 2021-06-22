// ---------------------------------------------------------------------------
// - OutputStream.cpp                                                        -
// - standard object library - output stream class implementation            -
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
#include "Unicode.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the crlf sequence
  static const t_quad CRLF[2] = {crlq, eolq};

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // return true if we have a tty - by default is false

  bool OutputStream::istty (void) const {
    return false;
  }

  // write a quad byte character
  
  long OutputStream::putb (const t_quad value) {
    wrlock ();
    try {
      long result = write (decode (value));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write an quad unicode character
  
  long OutputStream::putc (const t_quad value) {
    wrlock ();
    try {
      // encode in character form
      char* cbuf = Unicode::encode (d_emod, value);
      long result = write (cbuf);
      delete [] cbuf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write an unicode character
  
  long OutputStream::write (const t_quad value) {
    wrlock ();
    try {
      // check for byte mode
      if (d_emod == Encoding::EMOD_BYTE) {
	long result = putb (value);
	unlock ();
	return result;
      }
      // check for utf8 mode
      if (d_emod == Encoding::EMOD_UTF8) {
	long result = putc (value);
	unlock ();
	return result;
      }
      throw Exception ("write-error", "invalid unicode character to write");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write an unicode string to the output stream

  long OutputStream::write (const t_quad* value) {
    wrlock ();
    long size = Unicode::strlen (value);
    try {
      long result = write  (value, size);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a character array to the output stream

  long OutputStream::write (const char* rbuf, const long size) {
    // check argument first
    if ((rbuf == nullptr) || (size == 0)) return 0;
    // lock and write
    wrlock ();
    try {
      long result = 0;
      for (long i = 0; i < size; i++) {
	result += write (rbuf[i]);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write an array of unicode character to the output stream

  long OutputStream::write (const t_quad* value, const long size) {
   // check argument first
    if ((value == nullptr) || (size == 0)) return 0;
    // lock and write
    wrlock ();
    try {
      // check for byte mode
      if (d_emod == Encoding::EMOD_BYTE) {
	// transcode the quad buffer
	char* cbuf = decode (value, size);
	long result = write (cbuf);
	delete [] cbuf;
	unlock ();
	return result;
      }
      // check for utf8 mode
      if (d_emod == Encoding::EMOD_UTF8) {
	// encode the unicode buffer in character form
	char* cbuf = Unicode::encode (d_emod, value, size);
	long result = write (cbuf);
	delete [] cbuf;
	unlock ();
	return result;
      }
      throw Exception ("write-error", "invalid unicode buffer to write");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a string to the output stream

  long OutputStream::write (const String& value) {
    wrlock ();
    try {
      // check for byte mode
      if (d_emod == Encoding::EMOD_BYTE) {
	// get the quad buffer
	t_quad* sbuf = value.toquad ();
	if (sbuf == nullptr) {
	  unlock ();
	  return 0;
	}
	// transcode to the character buffer
	char* cbuf = decode (sbuf);
	delete [] sbuf;
	// write the buffer and clean
	long result = write (cbuf);
	delete [] cbuf;
	unlock ();
	return result;
      }
      // check for utf8 mode
      if (d_emod == Encoding::EMOD_UTF8) {
	t_quad* sbuf = value.toquad ();
	if (sbuf == nullptr) {
	  unlock ();
	  return 0;
	}
	// write string content
	long result = write (sbuf, Unicode::strlen (sbuf));
	delete [] sbuf;
	unlock ();
	return result;
      }
      throw Exception ("write-error", "invalid unicode string to write");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a buffer to the output stream

  long OutputStream::write (const Buffer& buf) {
    wrlock ();
    char*   cbuf = nullptr;
    t_quad* data = nullptr;
    try {
      // check for byte to byte writing
      if ((d_emod == Encoding::EMOD_BYTE) && (buf.getemod () == d_emod)) {
	long blen = buf.length ();
	cbuf = buf.tochar ();
	long result = write (cbuf, blen);
	delete [] cbuf;
	unlock ();
	return result;
      }
      // convert the buffer to a quad array
      data = buf.toquad ();
      if ((data == nullptr) || (data[0] == nilq)) {
	unlock ();
	return 0;
      }
      // check for byte mode
      if (d_emod == Encoding::EMOD_BYTE) {
	// transcode to the character buffer
	char* cbuf = decode (data);
	delete [] data;
	// write the buffer and clean
	long result = write (cbuf);
	delete [] cbuf;
	unlock ();
	return result;
      }
      // check for utf8 mode
      if (d_emod == Encoding::EMOD_UTF8) {
	// write string content
	long result = write (data);
	delete [] data;
	unlock ();
	return result;
      }
      throw Exception ("write-error", "invalid encoding mode with write");
    } catch (...) {
      delete cbuf;
      delete data;
      unlock ();
      throw;
    }
  }

  // write a string as an error string

  void OutputStream::error (const String& value) {
    wrlock ();
    try {
      write (value);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a panic string as an error string

  void OutputStream::panic (const String& value) {
    wrlock ();
    try {
      String pmesgs = "panic  : internal error";
      String reason = "reason : ";
      errorln (pmesgs);
      errorln (reason + value);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a newline to this output stream

  void OutputStream::newline (void) {
    wrlock ();
    try {
      write (eolq);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a newline with a carriage return

  void OutputStream::newline (const bool crlf) {
    wrlock ();
    try {
      if (crlf == true) {
	write (CRLF, 2);
      } else {
	write (eolq);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a string followed by a new line

  void OutputStream::writeln (const String& line) {
    write (line + eolq);
  }

  // write a string followed by a carraige return and a new line

  void OutputStream::writeln (const String& line, const bool crlf) {
    wrlock ();
    try {
      if (crlf == true) {
	write (line + crlq + eolq);
      } else {
	write (line + eolq);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a string followed by a new line in error mode

  void OutputStream::errorln (const String& line) {
    wrlock ();
    try {
      error (line + eolq);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a string followed by a carriage return and a new line in error mode

  void OutputStream::errorln (const String& line, const bool crlf) {
    wrlock ();
    try {
      if (crlf == true) {
	error (line + crlq + eolq);
      } else {
	error (line + eolq);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write an exception string on this output stream

  void OutputStream::errorln (const Exception& e) {
    wrlock ();
    try {
      String what = "exception : ";
      String file = "in file   : "; 
      String resm = "reason    : ";
      String resv = e.getval ();
      if (e.getnlf () == true) newline ();
      error (what + e.geteid ());
      newline ();
      long   lnum = e.getlnum ();
      String name = e.getname ();
      if ((lnum != 0) && (name.isnil () == false)) {
	error (file + name + " at or around line " + lnum);
	newline ();
      }
      if (resv.isnil () == false) {
	error (resm + resv);
	newline ();
      }
    } catch (...) {
      unlock ();
      throw;
    }
    unlock ();
  }

  // write a boolean on the output stream - no exception is thrown

  OutputStream& OutputStream::operator << (const bool value) {
    wrlock ();
    try {
      String data = Utility::tostring (value);
      this->write (data);
      unlock ();
    } catch (...) {
      unlock ();
    }
    return *this;
  }

  // write a character on the output stream - no exception is thrown

  OutputStream& OutputStream::operator << (const char c) {
    wrlock ();
    try {
      this->write (c);
      unlock ();
    } catch (...) {
      unlock ();
    };
    return *this;
  }

  // write an integer on the output stream - no exception is thrown

  OutputStream& OutputStream::operator << (const long value) {
    wrlock ();
    try {
      String data = Utility::tostring (value);
      this->write (data);
      unlock ();
    } catch (...) {
      unlock ();
    }
    return *this;
  }

  // write an integer on the output stream - no exception is thrown

  OutputStream& OutputStream::operator << (const t_long value) {
    wrlock ();
    try {
      String data = Utility::tostring (value);
      this->write (data);
      unlock ();
    } catch (...) {
      unlock ();
    }
    return *this;
  }

  // write a real on the output stream - no exception is thrown

  OutputStream& OutputStream::operator << (const t_real value) {
    wrlock ();
    try {
      String data = Utility::tostring (value);
      this->write (data);
      unlock ();
    } catch (...) {
      unlock ();
    }
    return *this;
  }

  // write a string on the output stream - no exception is thrown
  
  OutputStream& OutputStream::operator << (const char* s) {
    wrlock ();
    try {
      this->write (s);
      unlock ();
    } catch (...) {
      unlock ();
    }
    return *this;
  }

  // write a string on the output stream - no exception is thrown
  
  OutputStream& OutputStream::operator << (const String& s) {
    wrlock ();
    try {
      this->write (s);
      unlock ();
    } catch (...) {
      unlock ();
    }
    return *this;
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 8;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_WRITE   = zone.intern ("write");
  static const long QUARK_WRITELN = zone.intern ("writeln");
  static const long QUARK_ERRORLN = zone.intern ("errorln");
  static const long QUARK_NEWLINE = zone.intern ("newline");
  static const long QUARK_WRITSOH = zone.intern ("write-soh");
  static const long QUARK_WRITSTX = zone.intern ("write-stx");
  static const long QUARK_WRITETX = zone.intern ("write-etx");
  static const long QUARK_WRITEOS = zone.intern ("write-eos");

  // return true if the given quark is defined

  bool OutputStream::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Stream::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* OutputStream::apply (Evaluable* zobj, Nameset* nset, const long quark,
			       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch variable length arguments
    if ((quark == QUARK_WRITE) || (quark == QUARK_WRITELN)) {
      String sval;
      long   result = 0;
      for (long i = 0; i < argc; i++) {
	Object*   obj = argv->get (i);
	// check for a literal
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if (lobj != nullptr) {
	  sval += lobj->tostring ();
	  continue;
	}
	// check for a byte
	Byte* bobj = dynamic_cast <Byte*> (obj);
	if (bobj != nullptr) {
	  // eventually write the string value
	  if (sval.isnil () == false) {
	    result += write (sval);
	       sval = nilq;
	  }
	  result += write ((char) bobj->tobyte ());
	  continue;
	}
	// check for a buffer
	Buffer* buf = dynamic_cast <Buffer*> (obj);
	if (buf != nullptr) {
	  result += write (*buf);
	  continue;
	}
	throw Exception ("type-error", "invalid object to write", 
			 Object::repr (obj));
      }
      if (quark == QUARK_WRITELN) sval = sval + eolc;
      result += write (sval);      
      return new Integer (result);
    }

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_NEWLINE) {
	newline ();
	return nullptr;
      }
      if (quark == QUARK_WRITSOH) {
	write (sohc);
	return nullptr;
      }
      if (quark == QUARK_WRITSTX) {
	write (stxc);
	return nullptr;
      }
      if (quark == QUARK_WRITETX) {
	write (etxc);
	return nullptr;
      }
      if (quark == QUARK_WRITEOS) {
	write (eosc);
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ERRORLN) {
	String val = argv->getstring (0);
	errorln (val);
	return nullptr;
      }
    }
    // call the stream method
    return Stream::apply (zobj, nset, quark, argv);
  }
}
