// ---------------------------------------------------------------------------
// - OutputFile.cpp                                                          -
// - standard object library - output file class implementation              -
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
#include "Error.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "QuarkZone.hpp"
#include "OutputFile.hpp"
#include "csio.hpp"
#include "cerr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this function open a file by name and return a file id
  static int open_output_file (const String& name, 
			       const bool tflg, const bool aflg) {
    // check the file name
    if (name.isnil () == true) {
      throw Exception ("name-error", "nil output file name");
    }
    // try to open the file
    char* fname = name.tochar ();
    int sid     = c_openw (fname, tflg, aflg);
    if (sid < 0) {
      delete [] fname;
      throw Exception ("open-error", "cannot open output file", name);
    }
    delete [] fname;
    return sid;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new file output stream by name

  OutputFile::OutputFile (const String& name) {  
    d_name = name;
    d_sid  = open_output_file (d_name, true, false);
  }

  // create a new file output stream by name and encoding mode

  OutputFile::OutputFile (const String& name, const String& emod) {  
    // open the file
    d_name = name;
    d_sid  = open_output_file (d_name, true, false);
    // set the encoding mode
    setemod (emod);
  }

  // create a new file output stream by name and flags

  OutputFile::OutputFile (const String& name,
			  const bool tflg, const bool aflg) {
    d_name = name;
    d_sid  = open_output_file (d_name, tflg, aflg); 
  }

  // destroy this class by closing this file

  OutputFile::~OutputFile (void) {
    close ();
  }

  // return the class name

  String OutputFile::repr (void) const {
    return "OutputFile";
  }

  // return the stream descriptor

  int OutputFile::getsid (void) const {
    return d_sid;
  }

  // write one character to the output stream
  
  long OutputFile::write (const char value) {
    wrlock ();
    try {
      long code = c_write (d_sid, &value, 1);
      if (code < 0) throw Error ("write-error", c_errmsg (code), code);
      unlock ();
      return code;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a character string to the output stream

  long OutputFile::write (const char* data) {
    wrlock ();
    try {
      long size = Ascii::strlen (data);
      if (size == 0) {
	unlock ();
	return 0;
      }
      // write the buffer
      long code = c_write (d_sid, data, size);
      if (code < 0L) throw Error ("write-error", c_errmsg (code), code);
      unlock ();
      return code;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a character array into an output stream

  long OutputFile::write (const char* rbuf, const long size) {
    // check argument first
    if ((rbuf == nullptr) || (size <= 0)) return 0;
    // lock and write
    wrlock ();
    try {
      long result = 0;
      while (result != size) {
	long code = c_write (d_sid, &rbuf[result], size-result);
	if (code < 0L) throw Error ("write-error", c_errmsg (code), code);
	if (code == 0L) break;
	result += code;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the file name associated with this stream
  
  String OutputFile::getname (void) const {
    rdlock ();
    String result = d_name;
    unlock ();
    return result;
  }

  // close this output file
  
  bool OutputFile::close (void) {
    wrlock ();
    if ((d_sid == -1) || (Object::uref (this) == false)) {
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
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* OutputFile::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new OutputFile (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String emod = argv->getstring (1);
      return new OutputFile (name, emod);
    }
    // check for 3 arguments
    if (argc == 3) {
      String name  = argv->getstring (0);
      bool   tflag = argv->getbool (1);
      bool   aflag = argv->getbool (2);
      return new OutputFile (name, tflag, aflag);
    }
    throw Exception ("argument-error", 
		     "invalid arguments with with output file"); 
  }

  // return true if the given quark is defined

  bool OutputFile::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      // check the nameable class
      bool result = hflg ? Nameable::isquark (quark, hflg) : false;
      // check the output class
      if (result == false) {
	result = hflg ? OutputStream::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* OutputFile::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // check the nameable class
    if (Nameable::isquark (quark, true) == true) {
      return Nameable::apply (zobj, nset, quark, argv);
    }
    // fallback with the output stream method
    return OutputStream::apply (zobj, nset, quark, argv);
  }
}
