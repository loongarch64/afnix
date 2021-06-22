// ---------------------------------------------------------------------------
// - DipCalls.cpp                                                            -
// - afnix:dip service - image processing specific calls implementation      -
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
#include "Netpbm.hpp"
#include "Boolean.hpp"
#include "DipCalls.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // read a netpbm image

  Object* dip_rdpbm (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 1) {
      delete argv;
      throw Exception ("argument-error", 
		       "invalid number of arguments with read netpbm image");
    }
    try {
      Object* obj = argv->get (0);
      // check for an input stream
      auto is = dynamic_cast<InputStream*> (obj);
      if (is != nullptr) {
	Image* result = Netpbm::read (is);
	delete argv; argv = nullptr;
	return result;
      }
      // check for a uri
      auto uri = dynamic_cast<Uri*> (obj);
      if (uri != nullptr) {
	Image* result = Netpbm::read (*uri);
	delete argv; argv = nullptr;
	return result;
      }
      // check for a string uri
      auto suri = dynamic_cast<String*> (obj);
      if (suri != nullptr) {
	Image* result = Netpbm::read (*suri);
	delete argv; argv = nullptr;
	return result;
      }
      throw Exception ("type-error", "invalid object for netpbm",
		       Object::repr (obj));
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // write a netpbm image

  Object* dip_wrpbm (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 3) {
      delete argv;
      throw Exception ("argument-error", 
		       "invalid number of arguments with write netpbm image");
    }
    try {
      // collect the image and raw flag
      Object* obj = argv->get (1);
      auto img = dynamic_cast<Image*>(obj);
      if (img == nullptr) {
	throw Exception ("type-error", "invalid object for netpbm write",
			 Object::repr (obj));
      }
      bool raw = argv->getbool (2);
      // collect target object
      obj = argv->get (0);
      // check for an output stream
      auto os = dynamic_cast<OutputStream*> (obj);
      if (os != nullptr) {
	bool result = Netpbm::write (os, img, raw);
	delete argv; argv = nullptr;
	return new Boolean (result);
      }
      // check for a uri
      auto uri = dynamic_cast<Uri*> (obj);
      if (uri != nullptr) {
	bool result = Netpbm::write (*uri, img, raw);
	delete argv; argv = nullptr;
	return new Boolean (result);
      }
      // check for a string uri
      auto suri = dynamic_cast<String*> (obj);
      if (suri != nullptr) {
	bool result = Netpbm::write (*suri, img, raw);
	delete argv; argv = nullptr;
	return new Boolean (result);
      }
      throw Exception ("type-error", "invalid object for netpbm write",
		       Object::repr (obj));
    } catch (...) {
      delete argv;
      throw;
    }
  }
}
