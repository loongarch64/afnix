// ---------------------------------------------------------------------------
// - InputCipher.cpp                                                         -
// - afnix:sec module - input stream cipher class implementation             -
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
#include "QuarkZone.hpp"
#include "InputCipher.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the read non blocking mode
  static const long DEF_NBLK_MODE = 0;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default input cipher

  InputCipher::InputCipher (void) {
    p_sc = nullptr;
    p_is = nullptr;
  }

  // create a new input cipher by stream

  InputCipher::InputCipher (InputStream* is) {
    p_sc = nullptr;
    Object::iref (p_is = is);
  }

  // create a new input cipher by cipher

  InputCipher::InputCipher (Cipher* sc) {
    Object::iref (p_sc = sc);
    p_is = nullptr;
  }

  // create a new input cipher by stream and cipher

  InputCipher::InputCipher (InputStream* is, Cipher* sc) {
    Object::iref (p_sc = sc);
    Object::iref (p_is = is);
  }

  // destroy this input cipher

  InputCipher::~InputCipher (void) {
    Object::dref (p_sc);
    Object::dref (p_is);
  }

  // return the class name

  String InputCipher::repr (void) const {
    return "InputCipher";
  }

  // reset the input stream buffer

  void InputCipher::reset (void) {
    wrlock ();
    try {
      // reset the stream cipher
      if (p_sc != nullptr) p_sc->reset ();
      // flush the local buffer
      flush ();
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the input stream cipher timeout

  long InputCipher::gettout (void) const {
    rdlock ();
    try {
      long result = (p_is == nullptr) ? DEF_NBLK_MODE : p_is->gettout ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

 // set the input stream cipher timeout

  void InputCipher::settout (const long tout) {
    wrlock ();
    try {
      if (p_is != nullptr) p_is->settout (tout);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the eos flag is set

  bool InputCipher::iseos (void) const {
    wrlock ();
    try {
      // check the pushback buffer first
      if (d_sbuf.empty () == false) {
	unlock ();
	return false;
      }
      // check for an input stream
      if (p_is == nullptr) {
	unlock ();
	return true;
      }
      // check for a stream cipher
      if (p_sc == nullptr) {
	bool status = p_is->iseos ();
	unlock ();
	return status;
      }
      // check the input stream status
      if (p_is->iseos () == true) {
	unlock ();
	return true;
      }
      // try to fill the read buffer
      long rlen = p_sc->stream (d_sbuf, *p_is);
      if ((rlen == 0) || (d_sbuf.empty () == true)) {
	unlock ();
	return true;
      }
      // here we have some valid characters
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if we can read a character

  bool InputCipher::valid (void) const {
    wrlock ();
    try {
      // check the pushback buffer first
      if (d_sbuf.empty () == false) {
	unlock ();
	return true;
      }
      // check for an input stream
      if (p_is == nullptr) {
	unlock ();
	return false;
      }
      // check for a stream cipher
      if (p_sc == nullptr) {
	bool status = p_is->valid ();
	unlock ();
	return status;
      }
      // check the input stream status
      if (p_is->valid () == false) {
	unlock ();
	return false;
      }
      // try to fill the read buffer
      long rlen = p_sc->stream (d_sbuf, *p_is);
      if ((rlen == 0) || (d_sbuf.empty () == true)) {
	unlock ();
	return false;
      }
      // here we have some valid characters
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read one character from the input stream cipher

  char InputCipher::read (void) {
    wrlock ();
    try {
      // check the pushback buffer first
      if (d_sbuf.empty () == false) {
	char result = d_sbuf.read ();
	unlock ();
	return result;
      }
      // check for an input stream
      if (p_is == nullptr) {
	unlock ();
	return eosc;
      }
      // check for a stream cipher
      if (p_sc == nullptr) {
	char result = p_is->valid () ? p_is->read () : eosc;
	unlock ();
	return result;
      }
      // check the input stream status
      if (p_is->valid () == false) {
	unlock ();
	return eosc;
      }
      // try to fill the read buffer
      long rlen = p_sc->stream (d_sbuf, *p_is);
      if ((rlen == 0) || (d_sbuf.empty () == true)) {
	unlock ();
	return eosc;
      }
      // here we have some valid characters
      char result = d_sbuf.read ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the input stream

  void InputCipher::setis (InputStream* is) {
    wrlock ();
    try {
      // set the stream
      Object::iref (is);
      Object::dref (p_is);
      p_is = is;
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the stream cipher

  void InputCipher::setsc (Cipher* sc) {
    wrlock ();
    try {
      // set the stream
      Object::iref (sc);
      Object::dref (p_sc);
      p_sc = sc;
      // unlock and return
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
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETIS = zone.intern ("set-input-stream");
  static const long QUARK_SETSC = zone.intern ("set-stream-cipher");

  // create a new object in a generic way

  Object* InputCipher::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new InputCipher;

    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for an input stream
      InputStream* sobj = dynamic_cast <InputStream*> (obj);
      if (sobj != nullptr) return new InputCipher (sobj);
      // check for a cipher
      Cipher* cobj = dynamic_cast <Cipher*> (obj);
      if (cobj != nullptr) return new InputCipher (cobj);
      // invalid object
      throw Exception ("type-error", "invalid object with input cipher",
		       Object::repr (obj));
    }

    // check for 2 arguments
    if (argc == 2) {
      Object* obj = argv->get (0);
      // check for an input stream
      InputStream* sobj = dynamic_cast <InputStream*> (obj);
      if (sobj == nullptr) {
	throw Exception ("type-error", "invalid object with input cipher",
			 Object::repr (obj));	
      }
      obj = argv->get (1);
      // check for a cipher
      Cipher* cobj = dynamic_cast <Cipher*> (obj);
      if (cobj == nullptr) {
	throw Exception ("type-error", "invalid object with input cipher",
			 Object::repr (obj));
      }
      return new InputCipher (sobj, cobj);
    }
    throw Exception ("argument-error", "invalid arguments with input cipher");
  }

  // return true if the given quark is defined
  
  bool InputCipher::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? InputStream::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark
  
  Object* InputCipher::apply (Evaluable* zobj, Nameset* nset, const long quark,
                              Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETIS) {
        Object* obj = argv->get (0);
        InputStream* is = dynamic_cast <InputStream*> (obj);
        if (is == nullptr) {
          throw Exception ("type-error",
                           "invalid input stream for input cipher",
                           Object::repr (obj));
        }
        setis (is);
        return nullptr;
      }
      if (quark == QUARK_SETSC) {
        Object* obj = argv->get (0);
        Cipher*  sc  = dynamic_cast <Cipher*> (obj);
        if (sc == nullptr) {
          throw Exception ("type-error",
                           "invalid cipher object for input cipher",
                           Object::repr (obj));
        }
        setsc (sc);
        return nullptr;
      }
    }
    // call the input stream method
    return InputStream::apply (zobj, nset, quark, argv);
  }
}
