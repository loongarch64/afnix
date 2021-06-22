// ---------------------------------------------------------------------------
// - InputFile.cpp                                                           -
// - standard object library - input file class implementation               -
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

#include "Error.hpp"
#include "Vector.hpp"
#include "System.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "InputFile.hpp"
#include "QuarkZone.hpp"
#include "csio.hpp"
#include "cerr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this function open a file by name and return a file id
  static int if_open (const String& name) {
    // check the file name
    if (name.isnil () == true) {
      throw Exception ("name-error", "nil input file name");
    }
    // try to open the file
    char* fname = name.tochar ();
    int sid     = c_openr (fname);
    // clean, check and return
    delete [] fname;
    if (sid < 0) {
      throw Exception ("open-error", "cannot open input file", name);
    }
    return sid;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new input file by name

  InputFile::InputFile (const String& name) {
    d_name = name;
    d_sid  = if_open (name);
  }

  // create a new input file by name and encodind mode

  InputFile::InputFile (const String& name, const String& emod) {
    // open the file
    d_name = name;
    d_sid  = if_open (name);
    // set the encoding mode
    Stream::setemod (emod);
  }

  // close and destroy this input file
  InputFile::~InputFile (void) {
    close ();
  }

  // return the class name

  String InputFile::repr (void) const {
    return "InputFile";
  }

  // return the stream descriptor

  int InputFile::getsid (void) const {
    rdlock ();
    try {
      int result = d_sid;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the eos flag is set

  bool InputFile::iseos (void) const {
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

  bool InputFile::valid (void) const {
    wrlock ();
    try {
      if (d_sbuf.empty () == false) {
	unlock ();
	return true;
      }
      // check if we can read one character
      bool status = c_rdwait (d_sid, d_tout);
      if (status == false) {
	unlock ();
	return false;
      }
      // read in the character - might be the eos
      char c = nilc;
      long code = c_read (d_sid, &c, 1);
      if (code < 0L)  throw Error ("input-error", c_errmsg (code), code);
      if (code == 0L) {
	unlock ();
	return false;
      }
      // pushback and unlock
      d_sbuf.pushback (c);
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read one character from the input stream.

  char InputFile::read (void) {
    wrlock ();
    try {
      // check if we can read a character
      if (valid () == false) {
	unlock ();
	return eosc;
      }
      // check the pushback buffer first
      if (d_sbuf.empty () == false) {
	char result = d_sbuf.read ();
	unlock ();
	return result;
      }
      // read the next character in case valid
      // does not push a character (safety code only)
      char c = nilc;
      long code = c_read (d_sid, &c, 1);
      if (code < 0L) throw Error ("read-error", c_errmsg (code), code);
      // check for eos
      if (code == 0L) c = eosc;
      unlock ();
      return c;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // copy an input file stream into a buffer

  long InputFile::copy (char* rbuf, const long size) {
    // check argument first
    if ((rbuf == nullptr) || (size <= 0)) return 0;
    // lock and fill
    wrlock ();
    try {
      // initialize result
      long result = 0;
      // check the pushback buffer first
      for (long i = 0; i < size; i++) {
	if (d_sbuf.empty () == true) break;
	rbuf[i] = d_sbuf.read ();
	result++;
      }
      if (result == size) {
	unlock ();
	return result;
      }
      // check if we can read one character
      bool status = c_rdwait (d_sid, d_tout);
      if (status == false) {
	unlock ();
	return result;
      }
      // read by block
      long code = c_read (d_sid, &rbuf[result], size-result);
      if (code < 0L) throw Error ("read-error", c_errmsg (code), code);
      // update result and unlock
      result+= code;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the file name associated with this stream

  String InputFile::getname (void) const {
    rdlock ();
    try {
      String result = d_name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // close this input file and set the eos marker
  
  bool InputFile::close (void) {
    wrlock ();
    try {
      if ((d_sid < 0) || (Object::uref (this) == false)) {
	unlock ();
	return true;
      }
      if (c_close (d_sid) == false) {
	unlock ();
	return false;
      }
      d_sid = -1;
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the input stream at a certain position

  void InputFile::lseek (const t_long pos) {
    wrlock ();
    try {
      // move the file cursor
      c_lseek (d_sid, pos);
      // reset everything
      d_sbuf.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the input file size

  t_long InputFile::length (void) const {
    rdlock ();
    try {
      t_long result = c_fsize (d_sid);
      if (result < 0) {
	throw Exception ("file-error", "cannot access file size");
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the file modification time

  t_long InputFile::mtime (void) const {
    rdlock ();
    try {
      t_long result = c_mtime (d_sid);
      if (result < 0) {
	throw Exception ("file-error", "cannot access file modification time");
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
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_LSEEK  = zone.intern ("lseek");
  static const long QUARK_MTIME  = zone.intern ("get-modification-time");
  static const long QUARK_LENGTH = zone.intern ("length");

  // create a new object in a generic way

  Object* InputFile::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new InputFile (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String emod = argv->getstring (1);
      return new InputFile (name, emod);
    }
    throw Exception ("argument-error", 
		     "invalid arguments with with input file"); 
  }

  // return true if the given quark is defined

  bool InputFile::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      // check the nameable class
      bool result = hflg ? Nameable::isquark (quark, hflg) : false;
      // check the input stream class
      if (result == false) {
	result = hflg ? InputStream::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* InputFile::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {

    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_MTIME)  return new Integer (mtime  ());
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
    // check the nameable class
    if (Nameable::isquark (quark, true) == true) {
      return Nameable::apply (zobj, nset, quark, argv);
    }
    // fallback with the input stream method
    return InputStream::apply (zobj, nset, quark, argv);
  }
}
