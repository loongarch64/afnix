// ---------------------------------------------------------------------------
// - Hyperlink.cpp                                                           -
// - afnix:csm service - hyperlink class implementation                      -
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

#include "Uri.hpp"
#include "Nwgsid.hxx"
#include "Vector.hpp"
#include "Hyperlink.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a nil hyperlink
  
  Hyperlink::Hyperlink (void) {
    reset ();
  }

  // create a hyperlink by string uri
  
  Hyperlink::Hyperlink (const String& suri) {
    d_suri = suri;
  }

  // copy construct this hyperlink

  Hyperlink::Hyperlink (const Hyperlink& that) {
    that.rdlock ();
    try {
      // copy base plist
      Plist::operator = (that);
      // copy locally
      d_suri = that.d_suri;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign an hyperlink to this one

  Hyperlink& Hyperlink::operator = (const Hyperlink& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy base object
      Plist::operator =  (that);
      // copy locally
      d_suri = that.d_suri;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // return the hyperlink class name

  String Hyperlink::repr (void) const {
    return "Hyperlink";
  }

  // return a clone of this object

  Object* Hyperlink::clone (void) const {
    return new Hyperlink (*this);
  }
  
  // return the serial did

  t_word Hyperlink::getdid (void) const {
    return SRL_DEOD_NWG;
  }

  // return the serial sid

  t_word Hyperlink::getsid (void) const {
    return SRL_HLNK_SID;
  }
  
  // serialize this hyperlink

  void Hyperlink::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize base object
      Plist::wrstream (os);
      // serialize locally
      d_suri.wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this hyperlink

  void Hyperlink::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize base object
      Plist::rdstream (is);
      // deserialize locally
      d_suri.rdstream (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this hyperlink

  void Hyperlink::reset (void) {
    wrlock ();
    try {
      // reset base plist
      Plist::reset ();
      // reset locally
      d_suri.clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the hyperlink uri

  void Hyperlink::setsuri (const String& suri) {
    wrlock ();
    try {
      d_suri = suri;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the hyperlink uri

  String Hyperlink::getsuri (void) const {
    rdlock ();
    try {
      String result = d_suri;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the hyperling host

  String Hyperlink::gethost (void) const {
    rdlock ();
    try {
      String result = Uri::isuri (d_suri) ? Uri(d_suri).gethost () : "";
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the hyperling path

  String Hyperlink::getpath (void) const {
    rdlock ();
    try {
      String result = Uri::isuri (d_suri) ? Uri(d_suri).getpath () : d_suri;
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
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET    = zone.intern ("reset");
  static const long QUARK_SETSURI  = zone.intern ("set-uri");
  static const long QUARK_GETSURI  = zone.intern ("get-uri");
  static const long QUARK_GETHOST  = zone.intern ("get-host");
  static const long QUARK_GETPATH  = zone.intern ("get-path");

  // create a new object in a generic way

  Object* Hyperlink::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Hyperlink;
    // check for 1 argument
    if (argc == 1) {
      String suri = argv->getstring (0);
      return new Hyperlink (suri);
    }
    throw Exception ("argument-error",
                     "too many argument with hyperlink constructor");
  }

  // return true if the given quark is defined

  bool Hyperlink::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Plist::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Hyperlink::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETSURI) return new String (getsuri ());
      if (quark == QUARK_GETHOST) return new String (gethost ());
      if (quark == QUARK_GETPATH) return new String (getpath ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETSURI) {
	String suri = argv->getstring (0);
	setsuri (suri);
	return nullptr;
      }
    }
    // call the plist method
    return Plist::apply (zobj, nset, quark, argv);
  }
}
