// ---------------------------------------------------------------------------
// - InputString.cpp                                                         -
// - standard object library - string stream class implementation            -
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
#include "Character.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputString.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the read non blocking mode
  static const long DEF_NBLK_MODE = 0;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty string stream

  InputString::InputString (void)  {
    d_sbuf.setemod (Encoding::EMOD_UTF8);
    setemod (Encoding::EMOD_UTF8);
  }

  // create a string stream with a buffer

  InputString::InputString (const String& data)  {
    d_sbuf.setemod (Encoding::EMOD_UTF8);
    setemod (Encoding::EMOD_UTF8);
    d_sbuf.pushback (data);
  }

  // return the class name

  String InputString::repr (void) const {
    return "InputString";
  }

  // get the string stream timeout

  long InputString::gettout (void) const {
    return DEF_NBLK_MODE;
  }

  // set the string stream timeout

  void InputString::settout (const long tout) {
    throw Exception ("stream-error", "cannot set input string stream timeout");
  }

  // return true if we are at the eos

  bool InputString::iseos (void) const {
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

  // return true if we can read a character

  bool InputString::valid (void) const {
    rdlock ();
    try {
      bool result = !d_sbuf.empty ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // read one byte from the input stream.

  char InputString::read (void) {
    wrlock ();
    try {
      char result = d_sbuf.read ();
      if (result == nilc) result = eosc;
      unlock();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the stream with a string after a buffer flush

  void InputString::set (const String& data) {
    wrlock ();
    try {
      d_sbuf.reset ();
      d_sbuf.pushback (data);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get one byte but do not remove it

  char InputString::get (void) const {
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
  static const long QUARK_GET = zone.intern ("get");
  static const long QUARK_SET = zone.intern ("set");

  // create a new object in a generic way

  Object* InputString::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new InputString;

    // check for 1 argument
    if (argc == 1) {
      String sval = argv->getstring (0);
      return new InputString (sval);
    }
    throw Exception ("argument-error", 
		     "invalid arguments with with input string");
  }

  // return true if the given quark is defined

  bool InputString::isquark (const long quark, const bool hflg) const {
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
  
  Object* InputString::apply (Evaluable* zobj, Nameset* nset, const long quark,
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
    // call the input stream method
    return InputStream::apply (zobj, nset, quark, argv);
  }
}
