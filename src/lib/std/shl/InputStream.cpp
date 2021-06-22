// ---------------------------------------------------------------------------
// - InputStream.cpp                                                         -
// - standard object library - input stream class implementation             -
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
#include "Vector.hpp"
#include "Unicode.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Character.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "ccnv.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // get the content as an octet string

  String InputStream::format (void) const {
    rdlock ();
    try {
      // get the stream as a buffer
      Buffer ibuf = toibuf ();
      // format the buffer
      String result = ibuf.format ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // flush the input stream until a character is reached

  bool InputStream::flush (const char value) {
    wrlock ();
    bool result = false;
    try {
      while (valid () == true) {
	if (read () == value) {
	  result = true;
	  break;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // flush the input stream until a character is reached

  bool InputStream::flush (const t_quad value) {
    wrlock ();
    bool result = false;
    try {
      while (valid () == true) {
	if (getu () == value) {
	  result = true;
	  break;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read the next available byte

  t_byte InputStream::readb (void) {
    wrlock ();
    try {
      // check for timeout
      if (valid () == false) {
	if (iseos () == true) {
	  t_byte result = eosb;
	  unlock ();
	  return result;
	}
	throw Exception ("read-error", "read byte timeout");
      }
      // read and encode byte
      t_byte result = (t_byte) read ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read the next available word

  t_word InputStream::readw (const bool hflg) {
    wrlock ();
    try {
      t_word result = nilw;
      if (hflg == false) {
	for (long k = 0L; k < 2; k++) {
	  // shift the previous byte
	  result <<= 8;
	  // read and encode next byte
	  result |= (0x00FFU & readb ());
	}
      } else {
	// extract the character buffer
        t_byte buf[2];
        for (long i = 0; i < 2; i++) buf[i] = (t_byte) readb ();
        // convert it in host format
        result = c_wntoh (buf);
	if (c_isbe() == false) result = c_wswap (result);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // read the next available quad

  t_quad InputStream::readq (const bool hflg) {
    wrlock ();
    try {
      t_quad result = nilq;
      if (hflg == false) {
	for (long k = 0L; k < 4; k++) {
	  // shift the previous byte
	  result <<= 8;
	  // read and encode next byte
	  result |= (0x000000FFU & readb ());
	}
      } else {
	// extract the character buffer
        t_byte buf[4];
        for (long i = 0; i < 4; i++) buf[i] = (t_byte) readb ();
        // convert it in host format
        result = c_qntoh (buf);
	if (c_isbe() == false) result = c_qswap (result);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read the next available octa

  t_octa InputStream::reado (const bool hflg) {
    wrlock ();
    try {
      t_octa result = nilo;
      if (hflg == false) {
	for (long k = 0L; k < 8; k++) {
	  // shift the previous byte
	  result <<= 8;
	  // read and encode next byte
	  result |= (0x00000000000000FFU & readb ());
	}
      } else {
	t_byte buf[8];
        for (long i = 0; i < 8; i++) buf[i] = (t_byte) read ();
        // convert it in host format
        result = c_ontoh (buf);
	if (c_isbe() == false) result = c_oswap (result);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // copy an input stream into a buffer

  long InputStream::copy (char* rbuf, const long size) {
    // check argument first
    if ((rbuf == nullptr) || (size <= 0)) return 0;
    // lock and fill
    wrlock ();
    try {
      // initialize result
      long result = 0;
      // loop while valid
      for (long i = 0; i < size; i++) {
	if (valid () == false) break;
	rbuf[i] = read ();
	result++;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read the next quad byte

  t_quad InputStream::getb (void) {
    wrlock ();
    try {
      // check for timeout
      if (valid () == false) {
	if (iseos () == true) {
	  t_quad result = encode (eosq);
	  unlock ();
	  return result;
	}
	throw Exception ("read-error", "read byte character timeout");
      }
      // read and encode character
      t_quad result = encode (read ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read the next quad unicode character

  t_quad InputStream::getc (void) {
    wrlock ();
    try {
      // the utf byte buffer
      char ubuf[Unicode::MAX_UTF8_SIZE];
      // the read status
      bool status = true;
      // loop until full
      for (long i = 0; i < Unicode::MAX_UTF8_SIZE; i++) {
	// check for timeout
	if (valid () == false) {
	  if ((iseos () == true) && (status == true)) {
	    unlock ();
	    return eosq;
	  }
	  throw Exception ("read-error", "read unicode character timeout");
	}
	// read the character
	ubuf[i] = read ();
	// check for validity
	status = Unicode::valid (Encoding::EMOD_UTF8, ubuf, i+1);
	if (status == true) {
	  t_quad result = Unicode::decode (ubuf);
	  unlock ();
	  return result;
	}
      }
      // invalid character read
      throw Exception ("read-error", "cannot read unicode character");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read the next unicode character

  t_quad InputStream::getu (void) {
    wrlock ();
    try {
      // process byte mode
      if (d_emod == Encoding::EMOD_BYTE) {
	t_quad result = getb ();
	unlock ();
	return result;
      }
      // process utf mode
      if (d_emod == Encoding::EMOD_UTF8) {
	t_quad result = getc ();
	unlock ();
	return result;
      }
      // internal error
      throw Exception ("internal-error", "unicode character read mode");
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // read a buffer by size

  Buffer* InputStream::read (const long size) {
    wrlock ();
    Buffer* result = new Buffer;
    try {
      for (long i = 0; i < size; i++) {
	if (valid () == false) break;
	result->add (read ());
      }
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // read a line from this input stream

  String InputStream::readln (void) {
    wrlock ();
    try {
      // create a buffer to accumulate characters
      Buffer buf;
      bool   flg = false;
      // read the character in the buffer
      while (valid () == true) {
	t_quad c = getu ();
	if (c == crlq) {
	  flg = true;
	  continue;
	}
	if (c == eolq) break;
	if (flg == true) {
	  buf.add (crlq);
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

  // pushback a string on this stream
  
  long InputStream::pushback (const String& s) {
    wrlock ();
    try {
      // check for byte mode
      if (d_emod == Encoding::EMOD_BYTE) {
	// get the quad buffer
	t_quad* sbuf = s.toquad ();
	if (sbuf == nullptr) {
	  unlock ();
	  return 0;
	}
	// transcode to the character buffer
	char* cbuf = decode (sbuf);
	delete [] sbuf;
	// write the buffer and clean
	long result = pushback (cbuf);
	delete [] cbuf;
	unlock ();
	return result;
      }
      // check for utf-8 mode
      if (d_emod == Encoding::EMOD_UTF8) {
	char* cbuf = Unicode::encode (d_emod, s);
	if (cbuf == nullptr) {
	  unlock ();
	  return 0;
	}
	// write string content
	long result = pushback (cbuf);
	delete [] cbuf;
	unlock ();
	return result;
      }
      throw Exception ("pushback-error", "invalid mode for string pushback");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 13;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_EOSP     = zone.intern ("eos-p");
  static const long QUARK_READ     = zone.intern ("read");
  static const long QUARK_GETU     = zone.intern ("getu");
  static const long QUARK_FLUSH    = zone.intern ("flush");
  static const long QUARK_PUSHB    = zone.intern ("pushback");
  static const long QUARK_VALIDP   = zone.intern ("valid-p");
  static const long QUARK_READLN   = zone.intern ("readln");
  static const long QUARK_FORMAT   = zone.intern ("format");
  static const long QUARK_GETOUT   = zone.intern ("get-timeout");
  static const long QUARK_SETOUT   = zone.intern ("set-timeout");
  static const long QUARK_BUFLEN   = zone.intern ("get-buffer-length");
  static const long QUARK_CONSUME  = zone.intern ("consume");
  static const long QUARK_TOSTRING = zone.intern ("to-string");

  // return true if the given quark is defined

  bool InputStream::isquark (const long quark, const bool hflg) const {
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

  Object* InputStream::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_READ)     return new Byte      (read     ());
      if (quark == QUARK_GETU)     return new Character (getu     ());
      if (quark == QUARK_EOSP)     return new Boolean   (iseos    ());
      if (quark == QUARK_VALIDP)   return new Boolean   (valid    ());
      if (quark == QUARK_READLN)   return new String    (readln   ());
      if (quark == QUARK_FORMAT)   return new String    (format   ());
      if (quark == QUARK_GETOUT)   return new Integer   (gettout  ());
      if (quark == QUARK_BUFLEN)   return new Integer   (buflen   ());
      if (quark == QUARK_CONSUME)  return new Integer   (consume  ());
      if (quark == QUARK_TOSTRING) return new String    (tostring ());
      if (quark == QUARK_FLUSH) {
	flush ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
     if (quark == QUARK_FLUSH) {
	t_quad c = argv->getchar (0);
	return new Boolean (flush (c));
      }
      if (quark == QUARK_SETOUT) {
	long tout = argv->getlong (0);
	settout (tout);
	return nullptr;
      }

      if (quark == QUARK_READ) {
	long size = argv->getlong (0);
	return read (size);
      }
      if (quark == QUARK_PUSHB) {
	Object* obj = argv->get (0);
	// check for a byte
	Byte* bobj = dynamic_cast <Byte*> (obj);
	if (bobj != nullptr) {
	  long result = pushback ((char) bobj->tobyte ());
	  return new Integer (result);
	}
	// check for a character
	Character* cobj = dynamic_cast <Character*> (obj);
	if (cobj != nullptr) {
	  long result = pushback (cobj->toquad ());
	  return new Integer (result);
	}
	String* s = dynamic_cast <String*> (obj);
	if (s != nullptr) {
	  long result = pushback (*s);
	  return new Integer (result);
	}
	throw Exception ("type-error", "invalid object with pushback method",
			 Object::repr (obj));
      }
    }
    // apply these arguments with the stream object
    return Stream::apply (zobj, nset, quark, argv);
  }
}
