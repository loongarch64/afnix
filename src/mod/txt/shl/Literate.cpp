// ---------------------------------------------------------------------------
// - Literate.cpp                                                            -
// - afnix:txt module - transliterate object class implementation            -
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

#include "Byte.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Literate.hpp"
#include "Character.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default literate

  Literate::Literate (void) {
    reset ();
  }

  // create a literate with an escape byte

  Literate::Literate (const char escb) {
    reset ();
    d_escb = escb;
  }

  // create a literate with a character

  Literate::Literate (const t_quad escc) {
    reset ();
    d_escc = escc;
  }

  // create a literate with a model

  Literate::Literate (const t_model model) {
    setmodel (model);
  }

  // return the class name

  String Literate::repr (void) const {
    return "Literate";
  }

  // reset this literate

  void Literate::reset (void) {
    wrlock ();
    try {
      // reset byte tables
      d_escb = nilc;
      for (long i = 0; i < 256; i++) {
	d_mptb[i] = (char) i;
	d_estb[i] = nilc;
      }
      // reset character tables
      d_mptc.reset ();
      d_estc.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a transliteration model

  void Literate::setmodel (const t_model model) {
    wrlock ();
    try {
      // always reset the model
      reset ();
      // the afnix escaped string model
      if (model == ASTR) {
	setescb ('\\');
	setemcb ('t',  tabc);
	setemcb ('n',  eolc);
	setemcb ('\\', '\\');
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a mapping byte

  void Literate::setmapb (const char mapb, const char altb) {
    wrlock ();
    try {
      long cidx    = static_cast <long> (mapb);
      d_mptb[cidx] = altb;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a mapping byte

  char Literate::getmapb (const char mapb) const {
    rdlock ();
    try {
      long cidx   = static_cast <long> (mapb);
      char result = d_mptb[cidx];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a mapping character

  void Literate::setmapc (const t_quad mapc, const t_quad altc) {
    wrlock ();
    try {
      d_mptc.add (mapc, altc);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a mapping character

  t_quad Literate::getmapc (const t_quad mapc) const {
    rdlock ();
    try {
      t_quad result = mapc;
      if (d_mptc.exists (mapc) == true) result = d_mptc.get (mapc);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the escape byte

  void Literate::setescb (const char escb) {
    wrlock ();
    try {
      d_escb = escb;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the escape byte

  char Literate::getescb (void) const {
    rdlock ();
    try {
      char result = d_escb;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the escape character

  void Literate::setescc (const t_quad escc) {
    wrlock ();
    try {
      d_escc = escc;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the escape character

  t_quad Literate::getescc (void) const {
    rdlock ();
    try {
      t_quad result = d_escc;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an escaped mapping byte

  void Literate::setemcb (const char mapb, const char altb) {
    wrlock ();
    try {
      long cidx    = static_cast <long> (mapb);
      d_estb[cidx] = altb;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return an escaped mapping byte

  char Literate::getemcb (const char mapb) const {
    rdlock ();
    try {
      long cidx   = static_cast <long> (mapb);
      char result = d_estb[cidx];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an escaped mapping character

  void Literate::setemcc (const t_quad mapc, const t_quad altc) {
    wrlock ();
    try {
      d_estc.add (mapc, altc);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return an escaped mapping character

  t_quad Literate::getemcc (const t_quad mapc) const {
    rdlock ();
    try {
      t_quad result = d_estc.get (mapc);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // transliterate a string into another string

  String Literate::translate (const String& s) const {
    rdlock ();
    try {
      // create a result buffer
      Buffer buf (Encoding::EMOD_UTF8);
      // loop in the string
      long len = s.length ();
      long max = len - 1;
      for (long i = 0; i < len; i++) {
	t_quad mapc = s[i];
	t_quad altc = getmapc (mapc);
	// check for escape and map
	if ((i < max) && (d_escc != nilq) && (d_escc == altc)) {
	  t_quad ec = s[i+1];
	  if (d_estc.exists (ec) == true) {
	    altc = d_estc.get (ec);
	    i++;
	  }
	}
	// add it into the buffer
	buf.add (altc);
      }
      unlock ();
      return buf.tostring ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read a byte from an input stream and map it

  char Literate::read (InputStream& is) const {
    rdlock ();
    try {
      long cidx = static_cast <long> (is.read ());
      // map the byte
      char altb = d_mptb[cidx];
      // check for escape
      if ((d_escb != nilc) && (altb == d_escb)) {
	char nb = is.read ();
	long bi = static_cast <long> (nb);
	char eb = d_estb[bi];
	if (eb!= nilc) {
	  altb = eb;
	} else {
	  is.pushback (nb);
	}
      }
      unlock ();
      return altb;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read a character from an input stream and map it

  t_quad Literate::getu (InputStream& is) const {
    rdlock ();
    try {
      // read the character and map it
      t_quad mapc = is.getu ();
      t_quad altc = getmapc (mapc);
      // check for escape
      if ((d_escc != nilq) && (d_escc == altc)) {
	t_quad nc = is.getu ();
	if (d_estc.exists (nc) == true) {
	  altc = d_estc.get (nc);
	} else {
	  is.pushback (nc);
	}
      }
      unlock ();
      return altc;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 16;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_READ    = zone.intern ("read");
  static const long QUARK_GETU    = zone.intern ("getu");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_SETMAPB = zone.intern ("set-map-byte");
  static const long QUARK_GETMAPB = zone.intern ("get-map-byte");
  static const long QUARK_SETMAPC = zone.intern ("set-map-character");
  static const long QUARK_GETMAPC = zone.intern ("get-map-character");
  static const long QUARK_SETESCB = zone.intern ("set-esc-byte");
  static const long QUARK_GETESCB = zone.intern ("get-esc-byte");
  static const long QUARK_SETESCC = zone.intern ("set-esc-character");
  static const long QUARK_GETESCC = zone.intern ("get-esc-character");
  static const long QUARK_SETEMCB = zone.intern ("set-emc-byte");
  static const long QUARK_GETEMCB = zone.intern ("get-emc-byte");
  static const long QUARK_SETEMCC = zone.intern ("set-emc-character");
  static const long QUARK_GETEMCC = zone.intern ("get-emc-character");
  static const long QUARK_TRANSLT = zone.intern ("translate");

  // create a new object in a generic way

  Object* Literate::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Literate;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a byte
      Byte* bobj = dynamic_cast <Byte*> (obj);
      if (bobj != nullptr) {
	char escb = bobj->tobyte ();
	return new Literate (escb);
      }
      // check for a character
      Character* cobj = dynamic_cast <Character*> (obj);
      if (cobj != nullptr) {
	t_quad escc = cobj->toquad ();
	return new Literate (escc);
      }
      throw Exception ("type-error", "invalid arguments with literate");
    }
    throw Exception ("argument-error", "too many arguments with literate");
  }

  // return true if the given quark is defined

  bool Literate::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark
  
  Object* Literate::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETESCB) return new      Byte (getescb ());
      if (quark == QUARK_GETESCC) return new Character (getescc ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GETMAPB) {
	char mapb = argv->getbyte (0);
	return new Byte (getmapb (mapb));
      }
      if (quark == QUARK_GETMAPC) {
	t_quad mapc = argv->getchar (0);
	return new Character (getmapc (mapc));
      }
      if (quark == QUARK_SETESCB) {
	char escb = argv->getbyte (0);
	setescb (escb);
	return nullptr;
      }
      if (quark == QUARK_SETESCC) {
	t_quad escc = argv->getchar (0);
	setescc (escc);
	return nullptr;
      }
      if (quark == QUARK_GETEMCB) {
	char mapb = argv->getbyte (0);
	return new Byte (getemcb (mapb));
      }
      if (quark == QUARK_GETEMCC) {
	t_quad mapc = argv->getchar (0);
	return new Character (getemcc (mapc));
      }
      if (quark == QUARK_TRANSLT) {
	String sval = argv->getstring (0);
	return new String (translate (sval));
      }
      if (quark == QUARK_READ) {
	Object* obj = argv->get (0);
	// check for an input stream
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is == nullptr) throw Exception ("type-error",
					 "invalid object with read method",
					 Object::repr (obj));
	return new Byte (read (*is));
      }
      if (quark == QUARK_GETU) {
	Object* obj = argv->get (0);
	// check for an input stream
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is == nullptr) throw Exception ("type-error",
					 "invalid object with getu method",
					 Object::repr (obj));
	return new Character (getu (*is));
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETMAPB) {
	char mapb = argv->getbyte (0);
	char altb = argv->getbyte (1);
	setmapb (mapb, altb);
	return nullptr;
      }
      if (quark == QUARK_SETMAPC) {
	t_quad mapc = argv->getchar (0);
	t_quad altc = argv->getchar (1);
	setmapc (mapc, altc);
	return nullptr;
      }
      if (quark == QUARK_SETEMCB) {
	char mapb = argv->getbyte (0);
	char altb = argv->getbyte (1);
	setemcb (mapb, altb);
	return nullptr;
      }
      if (quark == QUARK_SETEMCC) {
	t_quad mapc = argv->getchar (0);
	t_quad altc = argv->getchar (1);
	setemcc (mapc, altc);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
