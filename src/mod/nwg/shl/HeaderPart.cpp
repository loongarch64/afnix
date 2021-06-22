// ---------------------------------------------------------------------------
// - HeaderPart.cpp                                                          -
// - afnix:nwg module - header part class implementation                     -
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

#include "Regex.hpp"
#include "Strvec.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Utility.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "HeaderPart.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  /// the property regex
  static const String HTTP_PROP_RGEX = "(<$a->+):$b*($N+)";

  // this procedure format a property into a string header
  static String get_header_string (const Property& prop) {
    // get the property name
    String result = prop.getname ();
    // add separator
    result += ": ";
    // add value
    result += prop.getpval ();
    // here we are
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a header part

  HeaderPart::HeaderPart (void) {
    d_head = Plist (true);
    reset ();
  }

  // copy construct this header part

  HeaderPart::HeaderPart (const HeaderPart& that) {
    that.rdlock ();
    try {
      d_head = that.d_head;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign a header part to this one

  HeaderPart& HeaderPart::operator = (const HeaderPart& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_head = that.d_head;
      that.unlock ();
      unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }

  // reset this header part

  void HeaderPart::reset (void) {
    wrlock ();
    try {
      d_head.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the header property list

  Plist HeaderPart::hplist (void) const {
    rdlock ();
    try {
      Plist result = d_head;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the header length

  long HeaderPart::hlength (void) const {
    rdlock ();
    try {
      long result = d_head.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a header property exists

  bool HeaderPart::hexists (const String& name) const {
    rdlock ();
    try {
      bool result = d_head.exists (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a header property by name and literal

  void HeaderPart::hset (const String& name, const Literal& lval) {
    wrlock ();
    try {
      d_head.set (name, lval.tostring ());
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a header property by index
  
  Property* HeaderPart::hget (const long index) const {
    rdlock ();
    try {
      Property* result = d_head.get (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a header property by name
  
  Property* HeaderPart::hfind (const String& name) const {
    rdlock ();
    try {
      Property* result = d_head.find (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a header property by name or throw an exception
  
  Property* HeaderPart::hlookup (const String& name) const {
    rdlock ();
    try {
      Property* result = d_head.lookup (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a header property value by name
  
  String HeaderPart::hmap (const String& name) const {
    rdlock ();
    try {
      String result = d_head.getpval (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a header property as a property list

  Plist HeaderPart::hmapl (const String& name) const {
    rdlock ();
    try {
      // get the property value
      String hval = hmap (name);
      // create the plist by name and info
      Plist plst (name, hval);
      // split the string with individual components
      Strvec svec = Strvec::split (hval, ';');
      long   slen = svec.length ();
      // the property regex
      const Regex re ("(<$a->+)=?($N*)");
      // loop in the proprties vectors
      for (long k = 0; k < slen; k++) {
	// get the stripped property
	String prop = svec.get(k).strip ();
	if (prop.isnil () == true) continue;
	// split the property name/value
	if (re != prop) {
	  throw Exception ("cookie-error", "invalid cookie property",prop);
	}
	// extract the name and value
	String pnam = re.getstring(0).strip().tolower();
	String pval = re.getstring(1).strip (" \t\"");
	// add it to the plist
	plst.add (pnam, pval);
      }
      unlock ();
      return plst;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // parse a header from an input stream

  void HeaderPart::hparse (InputStream& is) {
    wrlock ();
    try {
      // create a property regex
      Regex re = HTTP_PROP_RGEX;
      // loop while valid
      while (is.valid () == true) {
	// get the header line
	String line = is.readln ();
	// check for last empty line
	if (line.isnil () == true) break;
	// check for valid header
	if (re == line) {
	  String name = re.getstring (0);
	  String pval = re.getstring (1);
	  d_head.set (name.strip (), pval.strip ());
	} else {
	  throw Exception ("http-error", "invalid header content", line);
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // parse a header from an input stream

  void HeaderPart::hparse (Buffer& buf) {
    wrlock ();
    try {
      // create a property regex
      Regex re = HTTP_PROP_RGEX;
      // loop while valid
      while (buf.empty () == false) {
	// get the header line
	String line = buf.readln ();
	// check for last empty line
	if (line.isnil () == true) break;
	// check for valid header
	if (re == line) {
	  String name = re.getstring (0);
	  String pval = re.getstring (1);
	  d_head.set (name.strip (), pval.strip ());
	} else {
	  throw Exception ("http-error", "invalid header content", line);
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the header to the output stream

  void HeaderPart::hwrite (OutputStream& os) const {
    rdlock ();
    try {
      long hlen = d_head.length ();
      for (long i = 0; i < hlen; i++) {
	// get the property
	Property* prop = d_head.get (i);
	if (prop == nullptr) continue;
	// format the string and write
	os.writeln (get_header_string (*prop), true);
      }
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the header to a buffer

  void HeaderPart::hwrite (Buffer& buf) const {
    rdlock ();
    try {
      long hlen = d_head.length ();
      for (long i = 0; i < hlen; i++) {
	// get the property
	Property* prop = d_head.get (i);
	if (prop == nullptr) continue;
	// format the string and write
	buf.add (get_header_string (*prop));
	buf.add (crlc);
	buf.add (eolc);
      }
      // done
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
  static const long QUARK_ZONE_LENGTH = 11;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_HSET     = zone.intern ("header-set");
  static const long QUARK_HGET     = zone.intern ("header-get");
  static const long QUARK_HMAP     = zone.intern ("header-map");
  static const long QUARK_HMAPL    = zone.intern ("header-map-plist");
  static const long QUARK_RESET    = zone.intern ("reset");
  static const long QUARK_HFIND    = zone.intern ("header-find");
  static const long QUARK_HPLIST   = zone.intern ("header-plist");  
  static const long QUARK_HPARSE   = zone.intern ("header-parse");
  static const long QUARK_HWRITE   = zone.intern ("header-write");
  static const long QUARK_HLENGTH  = zone.intern ("header-length");
  static const long QUARK_HEXISTP  = zone.intern ("header-exists-p");
  static const long QUARK_HLOOKUP  = zone.intern ("header-lookup");  

  // return true if the given quark is defined

  bool HeaderPart::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* HeaderPart::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_HLENGTH)  return new Integer (hlength  ());
      if (quark == QUARK_HPLIST)   return new Plist   (hplist   ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_HPARSE) {
	Object* obj = argv->get (0);
	// check for an input stream
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is != nullptr) {
	  hparse (*is);
	  return nullptr;
	}
	// check for a buffer
	Buffer* buf = dynamic_cast <Buffer*> (obj);
	if (buf != nullptr) {
	  hparse (*buf);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object for header parsing",
			 Object::repr (obj));
      }
      if (quark == QUARK_HEXISTP) {
        String name = argv->getstring (0);
        return new Boolean (hexists (name));
      }
      if (quark == QUARK_HMAP) {
        String name = argv->getstring (0);
        return new String (hmap (name));
      }
      if (quark == QUARK_HMAPL) {
        String name = argv->getstring (0);
        return new Plist (hmapl (name));
      }
      if (quark == QUARK_HGET) {
        long index = argv->getlong (0);
        rdlock();
        try {
          Object* result = hget (index);
          zobj->post (result);
          unlock ();
          return result;
        } catch (...) {
          unlock ();
          throw;
        }
      }
      if (quark == QUARK_HFIND) {
        rdlock ();
        try {
          String name = argv->getstring (0);
          Object* result = hfind (name);
          zobj->post (result);
          unlock ();
          return result;
        } catch (...) {
          unlock ();
          throw;
        }
      }
      if (quark == QUARK_HLOOKUP) {
        rdlock ();
        try {
          String name = argv->getstring (0);
          Object* result = hlookup (name);
          zobj->post (result);
          unlock ();
          return result;
        } catch (...) {
          unlock ();
          throw;
        }
      }
      if (quark == QUARK_HWRITE) {
	Object* obj = argv->get (0);
	// check for an output stream
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os != nullptr) {
	  hwrite (*os);
	  return nullptr;
	}
	// check for a buffer
	Buffer* buf = dynamic_cast <Buffer*> (obj);
	if (buf != nullptr) {
	  hwrite (*buf);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object for header write",
			 Object::repr (obj));
      }
    }
    // dispatch 2 argument
    if (argc == 2) {
      if (quark == QUARK_HSET) {
	String   name = argv->getstring (0);
        Object*   obj = argv->get (1);
        Literal* lobj = dynamic_cast <Literal*> (obj);
        if (lobj == nullptr) {
          throw Exception ("type-error", "invalid object with set-header",
                           Object::repr (obj));
        }
	hset (name, *lobj);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
