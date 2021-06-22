// ---------------------------------------------------------------------------
// - MthTransit.cpp                                                          -
// - afnix:mth module - math object transit class implementation             -
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
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "OutputFile.hpp"
#include "MthTransit.hpp"
#include "OutputTerm.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

   // default transit mode
  static const auto MTH_TMOD_DEF = MthTransit::TMOD_AUTO;
  
  // this procedure exports a vector in auto mode
  static bool mth_xprt_auto (const Ivi& v, OutputStream* os) {
    // check for valid stream
    if (os == nullptr) return false;
    // get vector size
    long size = v.getsize ();
    if (size == 0L) return false;
    // set marker
    *os << "[" << eolc;
    // loop in vector
    for (long k = 0L; k < size; k++) {
      *os << " " << v.get (k);
      if (k < (size - 1L)) *os << ";";
      *os << eolc;
    }
    // set marker
    *os << "];" << eolc;
    return true;
  }
  
  // this procedure exports a vector in auto mode
  static bool mth_xprt_auto (const Rvi& v, OutputStream* os) {
    // check for valid stream
    if (os == nullptr) return false;
    // get vector size
    long size = v.getsize ();
    if (size == 0L) return false;
    // set marker
    *os << "[" << eolc;
    // loop in vector
    for (long k = 0L; k < size; k++) {
      *os << " " << v.get (k);
      if (k < (size - 1L)) *os << ";";
      *os << eolc;
    }
    // set marker
    *os << "];" << eolc;
    return true;
  }

  // this procedure exports a matrix in auto mode
  static bool mth_xprt_auto (const Rmi& m, OutputStream* os) {
    // check for valid stream
    if (os == nullptr) return false;
    // get matrix size
    long rsiz = m.getrsiz ();
    long csiz = m.getcsiz ();
    if ((rsiz == 0L) || (csiz == 0L)) return false;
    // set marker
    *os << "[" << eolc;
    // loop in matrix rows/cols
    for (long i = 0L; i < rsiz; i++) {
      for (long j = 0L; j < csiz; j++) {
	*os << " " << m.get (i, j);
	if (j < (csiz - 1L)) *os << ",";
      }
      if (i < (rsiz - 1L)) *os << ";";
      *os << eolc;
    }
    // set marker
    *os << "];" << eolc;
    return true;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default transit

  MthTransit::MthTransit (void) {
    d_tmod = MTH_TMOD_DEF;
    Object::iref (p_os = new OutputTerm (OutputTerm::OUTPUT));
  }

  // create a default transit by output stream

  MthTransit::MthTransit (OutputStream* os) {
    d_tmod = MTH_TMOD_DEF;
    Object::iref (p_os = os);
  }

  // destroy this transit

  MthTransit::~MthTransit (void) {
    Object::dref (p_os);
  }

  // return the class name
  
  String MthTransit::repr (void) const {
    return "MthTransit";
  }

  // reset this transit

  void MthTransit::reset (void) {
    wrlock ();
    try {
      d_tmod = TMOD_AUTO;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // export a vector to an output stream

  bool MthTransit::xport (const Ivi& v) {
    wrlock ();
    try {
      // preset result
      bool result = false;
      // check for auto mode
      if (d_tmod == TMOD_AUTO) result = mth_xprt_auto (v, p_os);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // export a vector to an output stream

  bool MthTransit::xport (const Rvi& v) {
    wrlock ();
    try {
      // preset result
      bool result = false;
      // check for auto mode
      if (d_tmod == TMOD_AUTO) result = mth_xprt_auto (v, p_os);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // export a matrix to an output stream

  bool MthTransit::xport (const Rmi& m) {
    wrlock ();
    try {
      // preset result
      bool result = false;
      // check for auto mode
      if (d_tmod == TMOD_AUTO) result = mth_xprt_auto (m, p_os);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // export a vector by symbol

  bool MthTransit::xport (const Ivi& v, const String& s) {
    wrlock ();
    try {
      // preset result
      bool result = false;
      // check for auto mode
      if (d_tmod == TMOD_AUTO) {
	*p_os << s << " = ";
	result = xport (v);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // export a vector by symbol

  bool MthTransit::xport (const Rvi& v, const String& s) {
    wrlock ();
    try {
      // preset result
      bool result = false;
      // check for auto mode
      if (d_tmod == TMOD_AUTO) {
	*p_os << s << " = ";
	result = xport (v);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // export a matrix by symbol

  bool MthTransit::xport (const Rmi& m, const String& s) {
    wrlock ();
    try {
      // preset result
      bool result = false;      
      // check for auto mode
      if (d_tmod == TMOD_AUTO) {
	*p_os << s << " = ";
	result = xport (m);
      }
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
  static const long QUARK_RESET = zone.intern ("reset");
  static const long QUARK_XPORT = zone.intern ("export");

  // create a new object in a generic way
  
  Object* MthTransit::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new MthTransit;    
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      auto os = dynamic_cast<OutputStream*>(obj);
      if (os == nullptr) {
	throw Exception ("type-error",
			 "invalid object as output stream transit",
			 Object::repr (obj));
      }
      return new MthTransit (os);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with math transit object");
  }

  // return true if the given quark is defined

  bool MthTransit::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true){
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
  
  Object* MthTransit::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_XPORT) {
	Object* obj = argv->get (0);
	// check for a long vector
	auto lv = dynamic_cast<Ivi*>(obj);
	if (lv != nullptr) {
	  return new Boolean (xport (*lv));
	}
	// check for a real vector
	auto rv = dynamic_cast<Rvi*>(obj);
	if (rv != nullptr) {
	  return new Boolean (xport (*rv));
	}
	// check for a real matrix
	auto rm = dynamic_cast<Rmi*>(obj);
	if (rm != nullptr) {
	  return new Boolean (xport (*rm));
	}
	throw Exception ("type-error", "invalid object as for export transit",
			 Object::repr (obj));
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_XPORT) {
	Object* obj = argv->get (0);
	String    s = argv->getstring (1);
	// check for a long vector
	auto lv = dynamic_cast<Ivi*>(obj);
	if (lv != nullptr) {
	  return new Boolean (xport (*lv, s));
	}
	// check for a real vector
	auto rv = dynamic_cast<Rvi*>(obj);
	if (rv != nullptr) {
	  return new Boolean (xport (*rv, s));
	}
	// check for a matrix
	auto rm = dynamic_cast<Rmi*>(obj);
	if (rm != nullptr) {
	  return new Boolean (xport (*rm, s));
	}
	throw Exception ("type-error", "invalid object as for export transit",
			 Object::repr (obj));
      }
    }
    // call the interface method
    return Object::apply (zobj, nset, quark, argv);
  }
}
