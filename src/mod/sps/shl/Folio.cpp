// ---------------------------------------------------------------------------
// - Folio.cpp                                                               -
// - afnix:sps module - folio class implementation                           -
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

#include "Folio.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the folio magic number
  const long SPS_MSIZE   = 4;
  const char SPS_MAGIC[] = {'\177', 'S', 'P', 'S'};

  // this function write the folio header
  static void write_folio_magic (OutputStream& os) {
    for (long i = 0; i < SPS_MSIZE; i++) os.write (SPS_MAGIC[i]);
  }

  // this function check that the header matches the edc magic number
  static bool check_folio_magic (InputStream* is) {
    // check for nil and reset
    if (is == nullptr) return false;
    // read in the magic number
    char mbuf[SPS_MSIZE];
    for (long i = 0; i < SPS_MSIZE; i++) {
      mbuf[i] = is->read ();
      if (mbuf[i] != SPS_MAGIC[i]) {
	is->pushback (mbuf, i+1);
	return false;
      }
    }
    return true;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a nil folio

  Folio::Folio (void) {
    reset ();
  }
  
  // create a new folio by name
  
  Folio::Folio (const String& name) {
    reset ();
    setname (name);
  }
  
  // create a new folio by name and info

  Folio::Folio (const String& name, const String& info) {
    reset ();
    setname (name);
    setinfo (info);
  }
		
  // create a new folio by stream

  Folio::Folio (InputStream* is) {
    try {
      // protect the input stream
      Object::iref (is);
      // reset the folio
      reset ();
      // check for valid header
      if (check_folio_magic (is) == false) {
	throw Exception ("folio-error", "invalid stream header");
      }
      // read-in data
      rdstream (*is);
      // fixed input stream
      Object::tref (is);
    } catch (...) {
      Object::tref (is);
      throw;
    }
  }
		
  // return the object name

  String Folio::repr (void) const {
    return "Folio";
  }

  // reset this folio

  void Folio::reset (void) {
    wrlock ();
    try {
      d_name = "";
      d_info = "";
      d_prop.reset ();
      d_vsht.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
	
  // write the folio to an output stream
  
  void Folio::write (OutputStream& os) {
    rdlock ();
    try {
      // write the header
      write_folio_magic (os);
      // serialize the folio
      wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // serialize a folio

  void Folio::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // save the base object
      Saveas::wrstream (os);
      // save the folio prop
      d_prop.wrstream (os);
      // save the vector
      d_vsht.wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this folio

  void Folio::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the base object
      Saveas::rdstream (is);
      // get the folio prop
      d_prop.rdstream (is);
      // get the vector
      d_vsht.rdstream (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the folio property list

  Plist Folio::getplist (void) const {
    rdlock ();
    try {
      Plist result = d_prop;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a property exists

  bool Folio::isprop (const String& name) {
    rdlock ();
    try {
      bool result = d_prop.exists (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of folio properties

  long Folio::lenprop (void) const {
    rdlock ();
    try {
      long result = d_prop.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a folio property by index
    
  Property* Folio::getprop (const long index) const {
    rdlock ();
    try {
      Property* result = d_prop.get (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a folio property by name

  Property* Folio::findprop (const String& name) const {
    rdlock ();
    try {
      Property* result = d_prop.find (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a folio property by name or throw an exception

  Property* Folio::lookprop (const String& name) const {
    rdlock ();
    try {
      Property* result = d_prop.lookup (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a folio property value by name

  String Folio::getpval (const String& name) const {
    rdlock ();
    try {
      String result = d_prop.getpval (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a folio property

  void Folio::addprop (Property* prop) {
    wrlock ();
    try {
      d_prop.add (prop);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a property by name and value

  void Folio::addprop (const String& name, const Literal& lval) {
    wrlock ();
    try {
      d_prop.add (name, lval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a property by name and value

  void Folio::setprop (const String& name, const Literal& lval) {
    wrlock ();
    try {
      d_prop.set (name, lval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the length of the folio

  long Folio::length (void) const {
    rdlock ();
    try {
      long result = d_vsht.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a sheet in this folio

  void Folio::add (Sheet* sheet) {
    wrlock ();
    try {
      d_vsht.add (sheet);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a folio sheet by index

  Sheet* Folio::get (const long index) const {
    rdlock ();
    try {
      Sheet* result = dynamic_cast <Sheet*> (d_vsht.get (index));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set this folio by index and sheet

  void Folio::set (const long index, Sheet* sheet) {
    wrlock ();
    try {
      d_vsht.set (index, sheet);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a tag exists

  bool Folio::istag (const String& tag) const {
    rdlock ();
    try {
      // get the folio length and iterate
      long len = length ();
      for (long i = 0; i < len; i++) {
	Sheet* sheet = get (i);
	if (sheet == nullptr) continue;
	if (sheet->istag (tag) == true) {
	  unlock ();
	  return true;
	}
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the sheet index by tag

  Index* Folio::getsidx (const String& tag) const {
    Index* indx= new Index;
    rdlock ();
    try {
      // get the folio length and iterate
      long len = length ();
      for (long i = 0; i < len; i++) {
	Sheet* sheet = get (i);
	if (sheet == nullptr) continue;
	if (sheet->istag (tag) == true) {
	  indx->add (-1, -1, i);
	}
      }
      unlock ();
      return indx;
    } catch (...) {
      delete indx;
      unlock ();
      throw;
    }
  }

  // find a sheet by tag - the first found is returned

  Sheet* Folio::find (const String& tag) const {
    rdlock ();
    try {
      // get the folio length and iterate
      long len = length ();
      for (long i = 0; i < len; i++) {
	Sheet* sheet = get (i);
	if (sheet == nullptr) continue;
	if (sheet->istag (tag) == false) continue;
	unlock ();
	return sheet;
      }
      unlock ();
      return nullptr;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // find a sheet by tag of throw an exception

  Sheet* Folio::lookup (const String& tag) const {
    rdlock ();
    try {
      Sheet* result = find (tag);
      if (result == nullptr) {
	throw Exception ("lookup-error", "cannot find sheet with tag", tag);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // filter a folio by tag and return a new folio

  Folio* Folio::filter (const String& tag) const {
    // create a result folio
    Folio* result = new Folio;
    rdlock ();
    // lock and process
    try {
      // get the folio length and iterate
      long len = length ();
      for (long i = 0; i < len; i++) {
	Sheet* sheet = get (i);
	if (sheet == nullptr) continue;
	if (sheet->istag (tag) == false) continue;
	result->add (sheet);
      }
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
    unlock ();
    return result;
  }
  
  // get a full xref table

  Xref* Folio::getxref (void) const {
    // create a result xref
    Xref* xref = new Xref;
    // lock and process
    rdlock ();
    try {
      // iterate in the folio
      long flen = length ();
      for (long i = 0; i < flen; i++) {
	// get the next sheet
	Sheet* sheet = get (i);
	if (sheet == nullptr) continue;
	// iterate in the sheet
	long slen = sheet->length ();
	for (long j = 0; j < slen; j++) {
	  // get the next record
	  Record* rcd = sheet->get (j);
	  if (rcd == nullptr) continue;
	  // iterate in the record
	  long rlen = rcd->length ();
	  for (long k = 0; k < rlen; k++) {
	    // get the next cell
	    Cell* cell = rcd->get (k);
	    if (cell == nullptr) continue;
	    // get the cell name and update
	    String name = cell->getname ();
	    xref->add (name, k, j, i);
	  }
	}
      }
      unlock ();
      return xref;
    } catch (...) {
      delete xref;
      unlock ();
      throw;
    }
  }

  // get a xref table by tag

  Xref* Folio::getxref (const String& tag) const {
    // create a result xref
    Xref* xref = new Xref;
    // lock and process
    rdlock ();
    try {
      // iterate in the folio
      long flen = length ();
      for (long i = 0; i < flen; i++) {
	// get the next sheet
	Sheet* sheet = get (i);
	if (sheet == nullptr) continue;
	if (sheet->istag (tag) == false) continue;
	// iterate in the sheet
	long slen = sheet->length ();
	for (long j = 0; j < slen; j++) {
	  // get the next record
	  Record* rcd = sheet->get (j);
	  if (rcd == nullptr) continue;
	  // iterate in the record
	  long rlen = rcd->length ();
	  for (long k = 0; k < rlen; k++) {
	    // get the next cell
	    Cell* cell = rcd->get (k);
	    if (cell == nullptr) continue;
	    // get the cell name and update
	    String name = cell->getname ();
	    xref->add (name, k, j, i);
	  }
	}
      }
      unlock ();
      return xref;
    } catch (...) {
      delete xref;
      unlock ();
      throw;
    }
  }

  // get a xref table by coordinate

  Xref* Folio::getxref (const long cidx) const {
    // create a result xref
    Xref* xref = new Xref;
    // lock and process
    rdlock ();
    try {
      // iterate in the folio
      long flen = length ();
      for (long i = 0; i < flen; i++) {
	// get the next sheet
	Sheet* sheet = get (i);
	if (sheet == nullptr) continue;
	// iterate in the sheet
	long slen = sheet->length ();
	for (long j = 0; j < slen; j++) {
	  // get the next record
	  Record* rcd = sheet->get (j);
	  if (rcd == nullptr) continue;
	  // get the cell at the index
	  Cell* cell = rcd->get (cidx);
	  if (cell == nullptr) continue;
	  // get the cell name and update
	  String name = cell->getname ();
	  xref->add (name, cidx, j, i);
	}
      }
      unlock ();
      return xref;
    } catch (...) {
      delete xref;
      unlock ();
      throw;
    }
  }

  // get a xref table by coordinate

  Xref* Folio::getxref (const long cidx, const long ridx) const {
    // create a result xref
    Xref* xref = new Xref;
    // lock and process
    rdlock ();
    try {
      // iterate in the folio
      long flen = length ();
      for (long i = 0; i < flen; i++) {
	// get the next sheet
	Sheet* sheet = get (i);
	if (sheet == nullptr) continue;
	// get the record at the index
	Record* rcd = sheet->get (ridx);
	if (rcd == nullptr) continue;
	// get the cell at the index
	Cell* cell = rcd->get (cidx);
	if (cell == nullptr) continue;
	// get the cell name and update
	String name = cell->getname ();
	xref->add (name, cidx, ridx, i);
      }
      unlock ();
      return xref;
    } catch (...) {
      delete xref;
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                         -
  // -------------------------------------------------------------------------
  
  // the quark zone
  static const long QUARK_ZONE_LENGTH = 21;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_SET     = zone.intern ("set");
  static const long QUARK_FIND    = zone.intern ("find");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_WRITE   = zone.intern ("write");
  static const long QUARK_LOOKUP  = zone.intern ("lookup");
  static const long QUARK_FILTER  = zone.intern ("filter");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_ISTAGP  = zone.intern ("tag-p");
  static const long QUARK_PROPLEN = zone.intern ("property-length");
  static const long QUARK_ISPROPP = zone.intern ("property-p");
  static const long QUARK_ADDPROP = zone.intern ("add-property");
  static const long QUARK_SETPROP = zone.intern ("set-property");
  static const long QUARK_GETPROP = zone.intern ("get-property");
  static const long QUARK_GETPLST = zone.intern ("get-property-list");
  static const long QUARK_GETPVAL = zone.intern ("get-property-value");
  static const long QUARK_FNDPROP = zone.intern ("find-property");
  static const long QUARK_LOKPROP = zone.intern ("lookup-property");
  static const long QUARK_GETINDX = zone.intern ("get-index");
  static const long QUARK_GETXREF = zone.intern ("get-xref");

  // create a new object in a generic way

  Object* Folio::mknew (Vector* argv) {
    // get number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Folio;
    // check for 1 argument
    if (argc == 1) {
      Object*  obj = argv->get (0);
      // check for a name
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new Folio (*sobj);
      // check for an input stream
      InputStream* iobj = dynamic_cast <InputStream*> (obj);
      if (iobj != nullptr) return new Folio (iobj);
      // invalid argument
      throw Exception ("argument-error", "invalid folio argument",
		       Object::repr (obj));
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Folio (name, info);
    }
    throw Exception ("argument-error", "too many argument with folio");
  }

  // return true if the given quark is defined

  bool Folio::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Saveas ::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // applythis object with a set of arguments and a quark

  Object* Folio::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH ) return new Integer (length   ());
      if (quark == QUARK_PROPLEN) return new Integer (lenprop  ());
      if (quark == QUARK_GETPLST) return new Plist   (getplist ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_GETXREF) {
	return getxref ();
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ISPROPP) {
	String name = argv->getstring (0);
	return new Boolean (isprop (name));
      }
      if (quark == QUARK_ADDPROP) {
        Object* obj = argv->get (0);
        Property* prop = dynamic_cast <Property*> (obj);
        if ((obj != nullptr) && (prop == nullptr)) {
          throw Exception ("type-error", "invalid object with add-property",
                           Object::repr (obj));
        }
        addprop (prop);
        return nullptr;
      }
      if (quark == QUARK_GETPROP) {
	long index = argv->getlong (0);
	rdlock();
	try {
	  Object* result = getprop (index);
	  zobj->post (result);
	  unlock ();	    
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETPVAL) {
	String name = argv->getstring (0);
	return new String (getpval (name));
      }
      if (quark == QUARK_FNDPROP) {
	rdlock ();
	try {
	  String name = argv->getstring (0);
	  Object* result = findprop (name);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_LOKPROP) {
	rdlock ();
	try {
	  String name = argv->getstring (0);
	  Object* result = lookprop (name);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GET) {
	long idx = argv->getlong (0);
	rdlock ();
	try {
	  Object* result = get (idx);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_ADD) {
	Object* obj = argv->get (0);
	Sheet*  sht = dynamic_cast <Sheet*> (obj);
	if ((obj != nullptr) && (sht == nullptr)) 
	  throw Exception ("type-error", "invalid object to add in folio",
			   Object::repr (obj));
	add (sht);
	return nullptr;
      }
      if (quark == QUARK_ISTAGP) {
	String tag = argv->getstring (0);
	return new Boolean (istag (tag));
      }
      if (quark == QUARK_GETINDX) {
	String tag = argv->getstring (0);
	return getsidx (tag);
      }
      if (quark == QUARK_FIND) {
	rdlock ();
	try {
	  String tag = argv->getstring (0);
	  Sheet* result = find (tag);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_LOOKUP) {
	rdlock ();
	try {
	  String tag = argv->getstring (0);
	  Sheet* result = lookup (tag);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_FILTER) {
	String tag = argv->getstring (0);
	return filter (tag);
      }
      if (quark == QUARK_GETXREF) {
	Object* obj = argv->get (0);
	// check for an integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  long cidx = iobj->tolong ();
	  return getxref (cidx);
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  return getxref (*sobj);
	}
	// invalid type
	throw Exception ("type-error", "invalid object with get-xref",
			 Object::repr (obj));
      }
      if (quark == QUARK_WRITE) {
	Object* obj = argv->get (0);
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os == nullptr) {
	  throw Exception ("type-error", "invalid object with write",
			   Object::repr (obj));
	}
	write (*os);
	return nullptr;
      }
    }
    // dispatch 2 argument
    if (argc == 2) {
      if (quark == QUARK_ADDPROP) {
        String   name = argv->getstring (0);
        Object*   obj = argv->get (1);
        Literal* lobj = dynamic_cast <Literal*> (obj);
        if (lobj == nullptr) {
	  throw Exception ("type-error", "invalid object with add-property",
			   Object::repr (obj));
        }
        addprop (name, *lobj);
        return nullptr;
      }
      if (quark == QUARK_SETPROP) {
        String   name = argv->getstring (0);
        Object*   obj = argv->get (1);
        Literal* lobj = dynamic_cast <Literal*> (obj);
        if (lobj == nullptr) {
        throw Exception ("type-error", "invalid object with set-property",
                         Object::repr (obj));
        }
        setprop (name, *lobj);
        return nullptr;
      }
      if (quark == QUARK_SET) {
	long    idx = argv->getlong (0);
	Object* obj = argv->get (1);
	Sheet*  sht = dynamic_cast <Sheet*> (obj);
	if ((obj != nullptr) && (sht == nullptr)) 
	  throw Exception ("type-error", "invalid object to set in folio",
			   Object::repr (obj));
	set (idx, sht);
	return nullptr;
      }
      if (quark == QUARK_GETXREF) {
	long cidx = argv->getlong (0);
	long ridx = argv->getlong (1);
	return getxref (cidx, ridx);
      }
    }
    // call the saveas method
    return Saveas::apply (zobj, nset, quark, argv);
  }
}
