// ---------------------------------------------------------------------------
// - Record.cpp                                                              -
// - afnix:sps module - record class implementation                          -
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
#include "Combo.hpp"
#include "Spssid.hxx"
#include "Record.hpp"
#include "Sorter.hpp"
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

  // this function compare an object with another one in ascent mode
  static bool qsort_cmplth (Object* ref, Object* slv) {
    if (ref == nullptr) return true;
    Object*   obj = ref->oper (Object::OPER_LTH, slv);
    Boolean* bobj = dynamic_cast <Boolean*> (obj);
    bool result   = (bobj == nullptr) ? false : bobj->tobool ();
    Object::cref (obj);
    return result;
  }
  
  // this function compare an object with another one in descent mode
  static bool qsort_cmpgth (Object* ref, Object* slv) {
    if ((ref == nullptr) && (slv == nullptr)) return true;
    if ((ref == nullptr) && (slv != nullptr)) return false;
    Object*   obj = ref->oper (Object::OPER_GTH, slv);
    Boolean* bobj = dynamic_cast <Boolean*> (obj);
    bool result   = (bobj == nullptr) ? false : bobj->tobool ();
    Object::cref (obj);
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a nil record

  Record::Record (void) {
    d_quark = 0;
    p_style = nullptr;
    reset ();
  }

  // create a new record by name

  Record::Record (const String& name) {
    d_quark = name.toquark ();
    p_style = nullptr;
    reset ();
  }

  // copy construct this record

  Record::Record (const Record& that) {
    that.rdlock ();
    try {
      d_quark = that.d_quark;
      d_trnum = that.d_trnum;
      p_style = (that.p_style == nullptr) ? nullptr : new Style (*that.p_style);
      Object::iref (p_style);
      long rlen = that.length ();
      for (long i = 0; i < rlen; i++) {
	Cell* cell = that.get (i);
	if (cell == nullptr) continue;
	add (new Cell (*cell));
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this record

  Record::~Record (void) {
    Object::dref (p_style);
  }

  // return the object name

  String Record::repr (void) const {
    return "Record";
  }

  // return a clone of this object

  Object* Record::clone (void) const {
    return new Record (*this);
  }

  // return the serial did

  t_word Record::getdid (void) const {
    return SRL_DEOD_SPS;
  }

  // return the serial sid

  t_word Record::getsid (void) const {
    return SRL_RECD_SID;
  }
  
  // serialize a record

  void Record::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // save the record name
      const String& name = String::qmap (d_quark);
      name.wrstream (os);
      // save the transaction number
      Integer trnum  (d_trnum);
      trnum.wrstream (os);
      // save the style
      if (p_style == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_style->serialize (os);
      }
      // save the vector
      d_vcell.wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this record

  void Record::rdstream (InputStream& is) {
    wrlock ();
    try {
      // get the record name
      String name;
      name.rdstream (is);
      d_quark = name.toquark ();
      // et the transaction number
      Integer trnum;
      trnum.rdstream (is);
      d_trnum = trnum.tolong ();
      // deserialize the style
      Object* sobj = Serial::deserialize (is);
      p_style = dynamic_cast <Style*> (sobj);
      if ((sobj != nullptr) && (p_style == nullptr)) {
	throw  Exception ("type-error", "invalid record style object", 
			  Object::repr (sobj));
      }
      Object::iref (p_style);
      // get the vector
      d_vcell.rdstream (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // assign a record to this one

  Record& Record::operator = (const Record& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_quark = that.d_quark;
      d_trnum = that.d_trnum;
      p_style = (that.p_style == nullptr) ? nullptr : new Style (*that.p_style);
      Object::iref (p_style);
      long rlen = that.length ();
      for (long i = 0; i < rlen; i++) {
	Cell* cell = that.get (i);
	if (cell == nullptr) continue;
	add (new Cell (*cell));
      }
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this record

  void Record::reset (void) {
    wrlock ();
    try {
      d_trnum = -1L;
      if (p_style != nullptr) p_style->reset ();
      d_vcell.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this record

  void Record::clear (void) {
    wrlock ();
    try {
      d_trnum = -1L;
      long rlen = length ();
      for (long k = 0L; k < rlen; k++) {
	Cell* cell = get (k);
	if (cell != nullptr) cell->clear ();
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the record name

  String Record::getname (void) const {
    rdlock ();
    try {
      String result = String::qmap (d_quark);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the record name

  void Record::setname (const String& name) {
    wrlock ();
    try {
      d_quark = name.toquark ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cell name by index

  void Record::setname (const long index, const String& name) {
    wrlock ();
    try {
      Cell* cell = dynamic_cast <Cell*> (d_vcell.get (index));
      if (cell != nullptr) cell->setname (name);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the transaction number

  void Record::setrnum (const long trnum) {
    wrlock ();
    try {
      d_trnum = trnum < 0 ? -1 : trnum;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the transaction number

  long Record::getrnum (void) const {
    rdlock ();
    try {
      long result = d_trnum;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the record style

  Style* Record::getstyle (void) const {
    rdlock ();
    try {
      Style* result = p_style;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the record style

  Style* Record::getstyle (const long index) const {
    rdlock ();
    try {
      // get the cell by index
      Cell* cell = get (index);
      // get the cell style
      Style* result = (cell == nullptr) ? nullptr : cell->getstyle ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the length of the record list

  long Record::length (void) const {
    rdlock ();
    try {
      long result = d_vcell.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a cell in this record

  void Record::add (Cell* cell) {
    wrlock ();
    try {
      Cell* data = (cell == nullptr) ? new Cell : cell;
      d_vcell.add (data);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a literal in this record

  void Record::add (Literal* lobj) {
    wrlock ();
    Cell* cell = nullptr;
    try {
      cell = new Cell (lobj);
      add (cell);
      unlock ();
    } catch (...) {
      delete cell;
      unlock ();
      throw;
    }
  }

  // add a literal in this record

  void Record::add (const String& name, Literal* lobj) {
    wrlock ();
    Cell* cell = nullptr;
    try {
      cell = new Cell (name, lobj);
      add (cell);
      unlock ();
    } catch (...) {
      delete cell;
      unlock ();
      throw;
    }
  }

  // add an object to this record

  void Record::add (Object* object) {
    if (object == nullptr) {
      add (new Cell);
      return;
    }
    // check for a cell
    Cell* cell = dynamic_cast <Cell*> (object);
    if (cell != nullptr) {
      add (cell);
      return;
    }
    // check for a literal
    Literal* lobj = dynamic_cast <Literal*> (object);
    if (lobj != nullptr) {
      add (lobj);
      return;
    }
    // check for a cons cell - which contains the name and literal value
    Cons* cons = dynamic_cast <Cons*> (object);
    if (cons != nullptr) {
      // check for 2 elements
      if (cons->length () != 2) {
	throw Exception ("cons-error", "invalid cons cell to add in record");
      }
      // check the car
      Object* car  = cons->getcar ();
      Object* scst = (car == nullptr) ? nullptr : car->reduce ();
      String* sobj = (car == nullptr) ? nullptr : dynamic_cast <String*> (scst);
      if (sobj == nullptr) {
	throw Exception ("type-error", "invalid object in cons cell to add",
			 Object::repr (car));
      }
      // check the cadr
      Object* cadr = cons->getcadr ();
      Object* lcst = (car == nullptr) ? nullptr : cadr->reduce ();
      lobj = dynamic_cast <Literal*> (lcst);
      if (sobj == nullptr) {
	throw Exception ("type-error", "invalid object in cons cell to add",
			 Object::repr (cadr));
      }
      // add the the literal by name
      add (*sobj, lobj);
      return;
    }
    // invalid object
    throw Exception ("type-error", "invalid object to add in record",
		     Object::repr (object));
  }

  // add a vector of objects

  void Record::add (const Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc == 0) return;
    wrlock ();
    try {
      for (long i = 0; i < argc; i++) add (argv->get (i));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a cell by index

  Cell* Record::get (const long index) const {
    rdlock ();
    try {
      long rlen = d_vcell.length ();
      Cell* result = (index < rlen) 
	? dynamic_cast <Cell*> (d_vcell.get (index))
	: nullptr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a cell literal by index

  Literal* Record::map (const long index) const {
    rdlock ();
    try {
      Cell* cell = get (index);
      Literal* lobj = (cell == nullptr) ? nullptr : cell->get ();
      unlock ();
      return lobj;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a cell in this record by index

  void Record::set (const long index, Cell* cell) {
    wrlock ();
    try {
      // fix the cell
      Cell* c = (cell == nullptr) ? new Cell : cell;
      // set or add
      long rlen = length ();
      if (index < rlen) {
	d_vcell.set (index, c);
      } else {
	// add missing cell
	for (long k = rlen; k < index; k++) add (new Cell);
	// add the cell
	add (c); 
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a literal in this record by index

  Cell* Record::set (const long index, Literal* lobj) {
    wrlock ();
    try {
      long  rlen = length ();
      Cell* cell = nullptr;
      if (index < rlen) {
	cell = get (index);
	if (cell == nullptr) d_vcell.set (index, new Cell (lobj));
	else cell->set (lobj);
      } else {
	// add missing cells
	for (long k = rlen; k < index; k++) add (new Cell);
	// add the cell
	cell = new Cell (lobj);
	add (cell);
      }
      unlock();
      return cell;
    } catch (...) {
      unlock ();
      throw;
    }
    unlock ();
  }

  // set an object in this record by index

  Cell* Record::set (const long index, Object* object) {
    // check for nil
    if (object == nullptr) return nullptr;
    // lock and set
    wrlock ();
    try {
      // check for a cell
      Cell* cell = dynamic_cast <Cell*> (object);
      if (cell != nullptr) {
	set (index, cell);
	unlock ();
	return cell;
      }
      // check for a literal
      Literal* lobj = dynamic_cast <Literal*> (object);
      if (lobj != nullptr) {
	Cell* cell = set (index, lobj);
	unlock ();
	return cell;
      }
      throw Exception ("type-error", "invalid object to set in record",
		       Object::repr (object));
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // find a cell by quark

  Cell* Record::find (const long quark) const {
    rdlock ();
    try {
      long rlen = d_vcell.length ();
      for (long i = 0; i < rlen; i++) {
	Cell* cell = dynamic_cast <Cell*> (d_vcell.get (i));
	if (cell == nullptr) continue;
	if (*cell == quark) {
	  unlock ();
	  return cell;
	}
      }
      unlock ();
      return nullptr;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // find a cell by name

  Cell* Record::find (const String& name) const {
    long quark = name.toquark ();
    return find (quark);
  }

  // find a cell by quark or throw an exception

  Cell* Record::lookup (const long quark) const {
    rdlock ();
    try {
      Cell* cell = find (quark);
      if (cell == nullptr) {
	throw Exception ("lookup-error", "cannot find cell", 
			 String::qmap (quark));
      }
      unlock ();
      return cell;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  
  // find a cell by name or throw an exception

  Cell* Record::lookup (const String& name) const {
    long quark = name.toquark ();
    return lookup (quark);
  }

  // get a cell index by quark

  long Record::getindex (const long quark) const {
    rdlock ();
    try {
      long rlen = d_vcell.length ();
      for (long i = 0; i < rlen; i++) {
	Cell* cell = dynamic_cast <Cell*> (d_vcell.get (i));
	if (cell == nullptr) continue;
	if (*cell == quark) {
	  unlock ();
	  return i;
	}
      }
      throw Exception ("index-error", "cannot find cell by index",
		       String::qmap (quark));
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a cell index by name

  long Record::getindex (const String& name) const {
    long quark = name.toquark ();
    return getindex (quark);
  }

  // check if the a literal match a cell by index

  bool Record::isequal (const long index, const Literal& lobj) const {
    rdlock ();
    try {
      // get the cell by index
      Cell* cell = get (index);
      // check the literal
      bool result = (cell == nullptr) ? false : (*cell == lobj);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // sort a record by ascending or descending mode

  void Record::sort (const bool mode) {
    wrlock ();
    // build the sorter object
    Sorter sorter;
    if (mode == true) 
      sorter.setcmpf (qsort_cmplth);
    else
      sorter.setcmpf (qsort_cmpgth);
    // sort the vector
    try {
      sorter.qsort (&d_vcell);
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
  static const long QUARK_ZONE_LENGTH = 14;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the record supported quarks
  static const long QUARK_ADD      = zone.intern ("add");
  static const long QUARK_GET      = zone.intern ("get");
  static const long QUARK_MAP      = zone.intern ("map");
  static const long QUARK_SET      = zone.intern ("set");
  static const long QUARK_FIND     = zone.intern ("find");
  static const long QUARK_SORT     = zone.intern ("sort");
  static const long QUARK_RESET    = zone.intern ("reset");
  static const long QUARK_CLEAR    = zone.intern ("clear");
  static const long QUARK_LOOKUP   = zone.intern ("lookup");
  static const long QUARK_LENGTH   = zone.intern ("length");
  static const long QUARK_SETNAME  = zone.intern ("set-name");
  static const long QUARK_GETINDEX = zone.intern ("get-index");
  static const long QUARK_SETRNUM  = zone.intern ("set-transaction-number");
  static const long QUARK_GETRNUM  = zone.intern ("get-transaction-number");

  // create a new object in a generic way

  Object* Record::mknew (Vector* argv) {
    // get number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Record;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Record (name);
    }
    throw Exception ("argument-error", "too many argument with record");
  }

  // return true if the given quark is defined

  bool Record::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Saveas::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // evaluate this object object by quark
  
  Object* Record::eval (Evaluable* zobj, Nameset* nset, const long quark) {
    rdlock ();
    try {
      // find the cell by quark
      Cell* cell = find (quark);
      if (cell != nullptr) {
	Object* result = cell->get ();
	zobj->post (result);
	unlock ();
	return result;
      }
      // make it as a combo
      unlock ();
      return new Combo (quark, this);
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Record::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // check for generic quark
    if (quark == QUARK_ADD) {
      add (argv);
      return nullptr;
    }

    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH ) return new Integer (length  ());
      if (quark == QUARK_GETRNUM) return new Integer (getrnum ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_CLEAR) {
	clear ();
	return nullptr;
      }
      if (quark == QUARK_SORT) {
	sort (true);
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETNAME) {
	String name = argv->getstring (0);
	setname (name);
	return nullptr;
      }
      if (quark == QUARK_SETRNUM) {
	long trnum = argv->getlong (0);
	setrnum (trnum);
	return nullptr;
      }
      if (quark == QUARK_SORT) {
	bool mode = argv->getbool (0);
	sort (mode);
	return nullptr;
      }
      if (quark == QUARK_FIND) {
	String name = argv->getstring (0);
	rdlock ();
	Object* result = find (name);
	zobj->post (result);
	unlock ();
	return result;
      }
      if (quark == QUARK_LOOKUP) {
	String name = argv->getstring (0);
	rdlock ();
	Object* result = lookup (name);
	zobj->post (result);
	unlock ();
	return result;
      }
      if (quark == QUARK_GETINDEX) {
	String name = argv->getstring (0);
	long result = getindex (name);
	return new Integer (result);
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
      if (quark == QUARK_MAP) {
	long idx = argv->getlong (0);
	rdlock ();
	try {
	  Object* result = map (idx);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // dispatch 2 argument
    if (argc == 2) {
      if (quark == QUARK_SET) {
	long    idx = argv->getlong (0);
	Object* obj = argv->get (1);
	wrlock ();
	try {
	  Object* result = set (idx, obj);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_SETNAME) {
	long    idx = argv->getlong (0);
	String name = argv->getstring (1);
	setname (idx, name);
	return nullptr;
      }
    }
    // call the saveas method
    return Saveas::apply (zobj, nset, quark, argv);
  }
}
