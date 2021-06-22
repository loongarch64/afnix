// ---------------------------------------------------------------------------
// - NamedFifo.cpp                                                           -
// - afnix:sio module - stream fifo class implementation                     -
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
#include "NamedFifo.hpp"
#include "InputFile.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "OutputFile.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default named fifo

  NamedFifo::NamedFifo (void) {
    d_name = "";
  }

  // create a named fifo by name

  NamedFifo::NamedFifo (const String& name) {
    d_name = name;
  }

  // create a named fifo by name

  NamedFifo::NamedFifo (const String& name, const bool sdir) {
    d_name = name;
    if (sdir == true) read (d_name);
  }

  // return the class name

  String NamedFifo::repr (void) const {
    return "NamedFifo";
  }

  // get the fifo name

  String NamedFifo::getname (void) const {
    rdlock ();
    try {
      String result = d_name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the fifo name

  void NamedFifo::setname (const String& name) {
    wrlock ();
    try {
      d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read the named fifo

  void NamedFifo::read () {
    wrlock ();
    try {
      if (d_name.isnil () == true) {
	throw Exception ("name-error", "nil name in fifo read mode");
      }
      read (d_name);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read the named fifo file

  void NamedFifo::read (const String& name) {
    wrlock ();
    try {
      InputFile is (name);
      rdstream (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the named fifo to the stream

  void NamedFifo::write (void) const {
    rdlock ();
    try {
      if (d_name.isnil () == true) {
	throw Exception ("name-error", "nil name in fifo write mode");
      }
      write (d_name);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the named fifo to the stream

  void NamedFifo::write (const String& name) const {
    rdlock ();
    try {
      OutputFile os (name);
      wrstream (os);
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
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_READ    = zone.intern ("read");
  static const long QUARK_WRITE   = zone.intern ("write");
  static const long QUARK_SETNAME = zone.intern ("set-name");

  // create a new object in a generic way

  Object* NamedFifo::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 arguments
    if (argc == 0) return new NamedFifo;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new NamedFifo (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      bool   sdir = argv->getbool   (1);
      return new NamedFifo (name, sdir);
    }
    throw Exception ("argument-error",
		     "too many argument with named fifo");
  }

  // return true if the given quark is defined

  bool NamedFifo::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Strfifo::isquark (quark, hflg) : false;
    if (result == false) {
      result = hflg ? Nameable::isquark (quark, hflg) : false;
    }
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark
  
  Object* NamedFifo::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_READ) {
	read ();
	return nullptr;
      }
      if (quark == QUARK_WRITE) {
	write ();
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
     if (quark == QUARK_SETNAME) {
	String name = argv->getstring (0);
	setname (name);
	return nullptr;
      }
      if (quark == QUARK_READ) {
	String name = argv->getstring (0);
	read (name);
	return nullptr;
      }
      if (quark == QUARK_WRITE) {
	String name = argv->getstring (0);
	write (name);
	return nullptr;
      }
    }
    // call the string vector method
    if (Strfifo::isquark (quark, true) == true) {
      return Strfifo::apply (zobj, nset, quark, argv);
    }
    // call the nameable method
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
