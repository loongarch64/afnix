// ---------------------------------------------------------------------------
// - JsonMime.cpp                                                            -
// - afnix:wax service - json mime class implementation                      -
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
#include "JsonMime.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the xml mime definitions
  static const String PN_JSN_EXTN = "json"; 
  static const String PN_JSN_MIME = Mime::tomime (PN_JSN_EXTN, false);

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default json mime

  JsonMime::JsonMime (void) {
    d_mime = PN_JSN_MIME;
  }

  // return the object class name

  String JsonMime::repr (void) const {
    return "JsonMime";
  }

  // write the json buffer into another one

  void JsonMime::write (Buffer& buf) const {
    rdlock ();
    try {
      buf = d_jbuf;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the data samples to an output stream

  void JsonMime::write (OutputStream& os) const {
    rdlock ();
    try {
      os.write (d_jbuf);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* JsonMime::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default html tree object
    if (argc == 0) return new JsonMime;
    throw Exception ("argument-error",
                     "too many argument with json mime constructor");
  }
  
  // return true if the given quark is defined

  bool JsonMime::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      bool result = hflg ? Mime::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? Json::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* JsonMime::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // check the mime object
    if (Mime::isquark (quark, true) == true) {
      return Mime::apply (zobj, nset, quark, argv);
    }
    // call the json method
    return Json::apply (zobj, nset, quark, argv);
  }
}
