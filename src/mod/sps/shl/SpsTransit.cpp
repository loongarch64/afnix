// ---------------------------------------------------------------------------
// - SpsTransit.cpp                                                          -
// - afnix:sps module - sps objects transit class implementation             -
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

#include "Csv.hpp"
#include "Locale.hpp"
#include "Reader.hpp"
#include "System.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "InputFile.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "SpsTransit.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // default transit mode
  static const auto   SPS_TMOD_DEF = SpsTransit::TMOD_AUTO;
  // default break sequence
  static const String SPS_SBRK_DEF = ",;\t";
  // the csv transit mode
  static const String SPS_XMOD_CSV = "CSV";
  // the sps transit mode
  static const String SPS_XMOD_ALS = "ALS";

  // this procedure map a string to a transit mode
  static SpsTransit::t_tmod sps_to_tmod (const String& xmod) {
    // check for nil
    if (xmod.isnil () == true) return SPS_TMOD_DEF;
    // map string to mode
    String xu = xmod.toupper ();
    if (xu == SPS_XMOD_ALS) return SpsTransit::TMOD_XALS;
    if (xu == SPS_XMOD_CSV) return SpsTransit::TMOD_XCSV;
    throw Exception ("sps-error", "invalid transit mode", xmod);
  }

  // this procedure exports a structure in auto mode

  static bool sps_xprt_auto (const Structure& s, OutputStream* os) {
    // check for valid stream
    if (os == nullptr) return false;
    // get structure geometry
    long ssiz = s.getssiz ();
    long dsiz = s.getdsiz ();
    if ((ssiz == 0L) || (dsiz == 0L)) return false;
    // create a result printable
    PrintTable ptbl(dsiz);
    // loop in the structure
    for (long sidx = 0L; sidx < ssiz; sidx++) {
      // add a row to the table
      long ridx = ptbl.add ();
      for (long didx = 0L; didx < dsiz; didx++) {
	// get the structure object as a literal
	auto lobj = dynamic_cast<Literal*> (s.get(sidx, didx));
	if (lobj == nullptr) continue;
	// add it to the table
	ptbl.set (ridx, didx, lobj);
      }
    }
    ptbl.format (*os);
    return true;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default transit

  SpsTransit::SpsTransit (void) {
    d_tmod = SPS_TMOD_DEF;
    d_sbrk = SPS_SBRK_DEF;
    p_tsht = nullptr;
    p_os   = nullptr;
  }

  // create a default transit by output stream

  SpsTransit::SpsTransit (OutputStream* os) {
    d_tmod = SPS_TMOD_DEF;
    d_sbrk = SPS_SBRK_DEF;
    p_tsht = nullptr;
    Object::iref (p_os = os);
  }
  
  // create a transit by mode and break sequence

  SpsTransit::SpsTransit (const String& xmod, const String& sbrk) {
    p_tsht = nullptr;
    p_os   = nullptr;
    setxmod (xmod);
    setsbrk (sbrk);
  }

  // destroy this record transit

  SpsTransit::~SpsTransit (void) {
    Object::dref (p_tsht);
    Object::dref (p_os);
  }

  // return the object name

  String SpsTransit::repr (void) const {
    return "SpsTransit";
  }

  // set the transit mode

  void SpsTransit::setxmod (const String& xmod) {
    wrlock ();
    try {
      d_tmod = sps_to_tmod (xmod);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the break sequence

  void SpsTransit::setsbrk (const String& sbrk) {
    wrlock ();
    try {
      d_sbrk = (sbrk.isnil () == true) ? SPS_SBRK_DEF : sbrk;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a locale definition

  long SpsTransit::addldef (const String& ldef) {
    wrlock ();
    try {
      // create a new locale object
      Locale lo (ldef);
      // get the index and check
      long lidx = lo.getlidx ();
      if (lidx < 0) {
	throw Exception ("transit-error",
			 "missing index in locale descriptor");
      }
      d_locv.add (lidx, lo.clone ());
      unlock ();
      return lidx;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the transit sheet

  Sheet* SpsTransit::gettsht (void) const {
    rdlock ();
    try {
      Sheet* result = p_tsht;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the sheet to import

  void SpsTransit::settsht (Sheet* tsht) {
    wrlock ();
    try {
      Object::iref(tsht); Object::dref (p_tsht); p_tsht = tsht;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the detached transit sheet

  Sheet* SpsTransit::detach (void) {
    wrlock ();
    try {
      Sheet* result = p_tsht; Object::tref (p_tsht); p_tsht = nullptr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // fill a structure with a sheet

  bool SpsTransit::fill (Structure* strc) const {
    rdlock ();
    try {
      // check for a valid sheet and structure
      if ((p_tsht == nullptr) || (strc == nullptr)) {
	unlock ();
	return false;
      }
      // get the local indexes - [0] is the number of indexes
      long  dsiz = strc->getdsiz ();
      long* lidx = strc->getlidx ();
      // get the sheet length and loop in rows
      long rows = p_tsht->length ();
      for (long i = 0L; i < rows; i++) {
	// open the structure
	strc->open ();
	// loop in the indexes
	for (long k = 0L; k < dsiz; k++) {
	  // get the literal object by indexes
	  Object* lobj = p_tsht->map(i, lidx[k]);
	  // add it to the structure
	  if (strc->add (lobj) == false) {
	    strc->reset ();
	    unlock ();
	    return false;
	  }
	}
	// close the structure
	strc->close ();
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add a form into a sheet

  void SpsTransit::import (Cons* cons) {
    if (cons == nullptr) return;
    wrlock ();
    Record* rcd = new Record;
    try {
      // check for a sheet
      if (p_tsht == nullptr) Object::iref (p_tsht = new Sheet);
      // iterate in the cons cell
      while (cons != nullptr) {
	// get the object and reduce it
	Object* car = cons->getcar ();
	Object* obj = (car == nullptr) ? nullptr : car->reduce ();
	rcd->add (obj);
	cons = cons->getcdr ();
      }
      p_tsht->add (rcd);
    } catch (...) {
      Object::cref (rcd);
      unlock ();
      throw;
    }
  }

  // import stream data into the sheet

  void SpsTransit::import (InputStream* is) {
    // check for nil
    if (is == nullptr) return;
    // lock and import
    wrlock ();
    Form* form = nullptr;
    Former* rd = nullptr;
    try {
      // protect the stream
      Object::iref (is);
      // get the former reader
      switch (d_tmod) {
      case SpsTransit::TMOD_AUTO:
	throw Exception ("transit-error", "cannot import stream in auto mode");
	break;
      case SpsTransit::TMOD_XALS:
	rd = new Reader (is);
	break;
      case SpsTransit::TMOD_XCSV:
	rd = new Csv (is, d_sbrk, d_locv);
	break;
      }
      // parse with the reader
      while (true) {
	form = rd->parse ();
	if (form == nullptr) break;
	import (form);
	Object::cref (form); form = nullptr;
      }
      delete rd;
      Object::tref (is);
    } catch (Exception& e) {
      if (form == nullptr) {
	e.setlnum (rd->getlnum ());
      } else {
	e.setlnum (form->getlnum ());
	Object::cref (form);
      }
      delete rd;
      unlock ();
      throw;
    } catch (...) {
      delete rd;
      Object::tref (is);
      Object::cref (form);
      unlock ();
      throw;
    }
  }
  
  // import data from a file

  void SpsTransit::import (const String& name) {
    wrlock ();
    InputStream* is = nullptr;
    try {
      // try to detect in automatic mode
      d_tmod = sps_to_tmod (System::xext (name));
      // open the file and import
      Object::iref (is = new InputFile (name));
      import (is);
      Object::dref (is);
      unlock ();
    } catch (...) {
      Object::dref (is);
      unlock ();
      throw;
    }
  }

  // export a structure to an output stream

  bool SpsTransit::xport (const Structure& s) {
    wrlock ();
    try {
      // preset result
      bool result = false;
      // check for auto mode
      if (d_tmod == TMOD_AUTO) result = sps_xprt_auto (s, p_os);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section (transit)                                              -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 9;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_FILL    = zone.intern ("fill");
  static const long QUARK_XPORT   = zone.intern ("export");
  static const long QUARK_IMPORT  = zone.intern ("import");
  static const long QUARK_DETACH  = zone.intern ("detach");
  static const long QUARK_SETXMOD = zone.intern ("set-transit-mode");
  static const long QUARK_SETSBRK = zone.intern ("set-break-sequence");
  static const long QUARK_ADDLDEF = zone.intern ("add-locale");
  static const long QUARK_SETTSHT = zone.intern ("set-sheet");
  static const long QUARK_GETTSHT = zone.intern ("get-sheet");

  // create a new object in a generic way

  Object* SpsTransit::mknew (Vector* argv) {
    // get number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new SpsTransit;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      auto os = dynamic_cast<OutputStream*>(obj);
      if (os == nullptr) {
	throw Exception ("type-error",
			 "invalid object as output stream transit",
			 Object::repr (obj));
      }
      return new SpsTransit (os);
    }
    // check for 2 arguments
    if (argc == 2) {
      String xmod = argv->getstring (0);
      String sbrk = argv->getstring (1);
      return new SpsTransit (xmod, sbrk);
    }
    throw Exception ("argument-error", "too many argument with sps transit");
  }
  
  // return true if the given quark is defined

  bool SpsTransit::isquark (const long quark, const bool hflg) const {
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

  Object* SpsTransit::apply (Evaluable* zobj, Nameset* nset, const long quark, 
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_DETACH) {
	wrlock ();
	try {
	  Sheet* tsht = detach ();
	  zobj->post (tsht);
	  unlock ();
	  return tsht;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETTSHT) {
	rdlock ();
	try {
	  Sheet* tsht = gettsht ();
	  zobj->post (tsht);
	  unlock ();
	  return tsht;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETXMOD) {
	String xmod = argv->getstring (0);
	setxmod (xmod);
	return nullptr;
      }
      if (quark == QUARK_SETSBRK) {
	String sbrk = argv->getstring (0);
	setsbrk (sbrk);
	return nullptr;
      }
      if (quark == QUARK_ADDLDEF) {
	String ldef = argv->getstring (0);
	return new Integer (addldef (ldef));
      }
      if (quark == QUARK_SETTSHT) {
	Object* obj = argv->get (0);
	// check for a sheet
	auto* tsht = dynamic_cast <Sheet*> (obj);
	if (tsht == nullptr) {
	  throw Exception ("type-error", "invalid object for set-record ",
			   Object::repr (obj));
	}
	settsht (tsht);
	return nullptr;
      }
      if (quark == QUARK_FILL) {
	Object* obj = argv->get (0);
	// check for a structure
	auto* strc = dynamic_cast <Structure*> (obj);
	if (strc == nullptr) {
	  throw Exception ("type-error", "invalid object for structure fill ",
			   Object::repr (obj));
	}
	return new Boolean (fill(strc));
      }
      if (quark == QUARK_IMPORT) {
	Object* obj = argv->get (0);
	// check for an input stream
	auto is = dynamic_cast <InputStream*> (obj);
	if (is != nullptr) {
	  import (is);
	  return nullptr;
	}
	// check for a string
	auto sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  import (*sobj);
	  return nullptr;
	}
	// check for a cons cell
	auto cons = dynamic_cast <Cons*> (obj);
	if (cons != nullptr) {
	  import (cons);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object for import ",
			 Object::repr (obj));
      }
      if (quark == QUARK_XPORT) {
	Object* obj = argv->get (0);
	// check for a structure
	auto s = dynamic_cast<Structure*>(obj);
	if (s != nullptr) {
	  return new Boolean (xport (*s));
	}
	throw Exception ("type-error", "invalid object as for export transit",
			 Object::repr (obj));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
