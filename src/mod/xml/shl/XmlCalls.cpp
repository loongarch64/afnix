// ---------------------------------------------------------------------------
// - XmlCalls.cpp                                                            -
// - afnix:xml module - xml specific calls implementation                    -
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

#include "Cons.hpp"
#include "Vector.hpp"
#include "XmlCalls.hpp"
#include "XmlSystem.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // prenormalize a text by keeping the eol characters

  Object* xml_prenorm (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String text = argv->getstring (0);
	delete argv; argv = nullptr;
	return new String (XmlSystem::prenorm (text));
      }
      throw Exception ("argument-error", 
		       "too many arguments with pre-normal");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // normalize a text by keeping only one blank character

  Object* xml_tonorm (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String text = argv->getstring (0);
	delete argv; argv = nullptr;
	return new String (XmlSystem::tonorm (text));
      }
      throw Exception ("argument-error", 
		       "too many arguments with to-normal");
    } catch (...) {
      delete argv;
      throw;
    }
  }
}
