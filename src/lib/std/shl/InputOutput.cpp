// ---------------------------------------------------------------------------
// - InputOutput.cpp                                                         -
// - standard object library - input-output stream class implementation      -
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
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Character.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputOutput.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the read non blocking mode
  static const long DEF_NBLK_MODE = 0;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new input-output stream

  InputOutput::InputOutput (void) {
    d_sbuf.setrflg (false);
    d_sbuf.reset   ();
  }

  // create a new input-output stream with a resize flag

  InputOutput::InputOutput (const bool rflg) {
    d_sbuf.setrflg (rflg);
    d_sbuf.reset   ();
  }

  // create a new input-output stream with a data string

  InputOutput::InputOutput (const String& data) {
    d_sbuf.setrflg (false);
    d_sbuf.reset   ();
    d_sbuf.add     (data);
  }

  // create a new input-output stream with a buffer

  InputOutput::InputOutput (const Buffer& sbuf) {
    d_sbuf = sbuf;
    d_sbuf.setrflg (false);
  }
  
  // create a new input-output stream with a buffer and a size

  InputOutput::InputOutput (const char* data, const long size) {
    d_sbuf.setrflg (false);
    d_sbuf.reset   ();
    d_sbuf.add     (data, size);
  }

  // return the class name

  String InputOutput::repr (void) const {
    return "InputOutput";
  }

  // get the stream timeout

  long InputOutput::gettout (void) const {
    return DEF_NBLK_MODE;
  }

  // set the stream timeout

  void InputOutput::settout (const long tout) {
    throw Exception ("stream-error", "cannot set input-output stream timeout");
  }

  // return the eos flag

  bool InputOutput::iseos (void) const {
    rdlock ();
    try {
      bool result = d_sbuf.empty ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if we can read a valid character

  bool InputOutput::valid (void) const {
    rdlock ();
    try {
      bool result = (d_sbuf.empty () == false);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read a character from the stream

  char InputOutput::read (void) {
    wrlock ();
    try {
      char result = d_sbuf.empty () ? eosc : d_sbuf.read ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a character in the buffer

  long InputOutput::write (const char value) {
    wrlock ();
    try {
      long result = d_sbuf.add (value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a character string in the buffer

  long InputOutput::write (const char* value) {
    wrlock ();
    try {
      long result = d_sbuf.add (value, Ascii::strlen (value));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the stream with a string after a buffer flush

  void InputOutput::set (const String& data) {
    wrlock ();
    try {
      d_sbuf.reset ();
      d_sbuf.add   (data);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get one byte but do not remove it

  char InputOutput::get (void) const {
    rdlock ();
    try {
      char result = d_sbuf.get ();
      if (result == nilc) result = eosc;
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
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SET = zone.intern ("set");
  static const long QUARK_GET = zone.intern ("get");

  // create a new object in a generic way

  Object* InputOutput::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new InputOutput;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a boolean
      Boolean* bobj = dynamic_cast <Boolean*> (obj);
      if (bobj != nullptr) return new InputOutput (bobj->tobool ());
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new InputOutput (*sobj);
      // check for a buffer
      Buffer* sbuf = dynamic_cast <Buffer*> (obj);
      if (sbuf != nullptr) return new InputOutput (*sbuf);
      // invalid object
      throw Exception ("type-error", "invalid object for input-output stream",
		       Object::repr (obj));
    }
    throw Exception ("argument-error", 
		     "invalid arguments with with input-output stream");
  }

  // return true if the given quark is defined

  bool InputOutput::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? InputStream::isquark (quark, hflg) : false;
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

  Object* InputOutput::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (quark == QUARK_GET) return new Character (get ());

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SET) {
	String val = argv->getstring (0);
	set (val);
	return nullptr;
      }
    }
    // check with the input stream object
    if (InputStream::isquark (quark, true) == true) {
      return InputStream::apply (zobj, nset, quark, argv);
    }
    // fallback with the output stream
    return OutputStream::apply (zobj, nset, quark, argv);
  }
}
