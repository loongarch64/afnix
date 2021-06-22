// ---------------------------------------------------------------------------
// - Cell.cpp                                                                -
// - afnix:sps module - cell class implementation                            -
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

#include "Cell.hpp"
#include "Spssid.hxx"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty cell
  
  Cell::Cell (void) {
    d_quark = 0;
    p_cobj  = nullptr;
    d_cflg = false;
    p_cstl = nullptr;
  }

  // create a cell with a literal
  
  Cell::Cell (Literal* cobj) {
    d_quark = 0;
    Object::iref (p_cobj = cobj);
    d_cflg = false;
    p_cstl = nullptr;
  }

  // create a cell by name with a literal
  
  Cell::Cell (const String& name, Literal* cobj) {
    d_quark = name.toquark ();
    Object::iref (p_cobj = cobj);
    d_cflg = false;
    p_cstl = nullptr;
  }

  // copy construct this cell

  Cell::Cell (const Cell& that) {
    that.rdlock ();
    try {
      d_quark = that.d_quark;
      d_cflg = that.d_cflg;
      Object::iref (p_cobj = that.p_cobj);
      Object::iref (p_cstl = that.p_cstl);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this cell

  Cell::~Cell (void) {
    Object::dref (p_cobj);
    Object::dref (p_cstl);
  }

  // return the class name

  String Cell::repr (void) const {
    return "Cell";
  }

  // return a clone of this object

  Object* Cell::clone (void) const {
    return new Cell (*this);
  }
  
  // return the serial did

  t_word Cell::getdid (void) const {
    return SRL_DEOD_SPS;
  }

  // return the serial sid

  t_word Cell::getsid (void) const {
    return SRL_CELL_SID;
  }
  
  // serialize this cell

  void Cell::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      const String& name = String::qmap (d_quark);
      name.wrstream (os);
      Serial::wrbool (d_cflg, os);
      if (p_cobj == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_cobj->serialize (os);
      }
      if (p_cstl == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_cstl->serialize (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this cell

  void Cell::rdstream (InputStream& is) {
    wrlock ();
    try {
      String  sval;
      sval.rdstream (is);
      d_quark = sval.toquark ();
      d_cflg  = Serial::rdbool (is);
      // deserialize the literal
      Object* cobj = Serial::deserialize (is);
      p_cobj  = dynamic_cast <Literal*> (cobj);
      if ((cobj != nullptr) && (p_cobj == nullptr)) {
	throw  Exception ("type-error", "invalid cell type object", 
			  Object::repr (cobj));
      }
      Object::iref (p_cobj);
      // deserialize the style
      Object* sobj = Serial::deserialize (is);
      p_cstl  = dynamic_cast <Style*> (sobj);
      if ((sobj != nullptr) && (p_cstl == nullptr)) {
	throw  Exception ("type-error", "invalid cell style object", 
			  Object::repr (sobj));
      }
      Object::iref (p_cstl);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // assign a cell to this one

  Cell& Cell::operator = (const Cell& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_quark = that.d_quark;
      d_cflg = that.d_cflg;
      Object::iref (that.p_cobj);
      Object::dref (p_cobj);
      p_cobj = that.p_cobj;
      Object::iref (that.p_cstl);
      Object::dref (p_cstl);
      p_cstl = that.p_cstl;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // match a quark against the cell

  bool Cell::operator ==  (const long quark) const {
    rdlock ();
    try {
      bool result = (d_quark == quark);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // match a literal against the cell

  bool Cell::operator ==  (const Literal& lobj) const {
    rdlock ();
    try {
      bool result = (p_cobj == nullptr) ? false : (*p_cobj == lobj);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // reset this cell

  void Cell::reset (void) {
    wrlock ();
    try {
      d_quark = 0L;
      d_cflg = false;
      Object::dref (p_cobj); p_cobj = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this cell

  void Cell::clear (void) {
    wrlock ();
    try {
      if ((d_cflg == false) && (p_cobj != nullptr)) p_cobj->clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the cell name

  String Cell::getname (void) const {
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

  // set the cell name

  void Cell::setname (const String& name) {
    wrlock ();
    try {
      d_quark = name.toquark ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // return the cell literal value

  Literal* Cell::get (void) const {
    rdlock ();
    try {
      Literal* result = p_cobj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // set the cell value

  void Cell::set (Literal* cobj) {
    wrlock ();
    try {
      if (d_cflg == true) {
	throw Exception ("const-error", "cell const violation");
      } 
      Object::iref (cobj);
      Object::dref (p_cobj);
      p_cobj = cobj;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // set the cell value with the const flag

  void Cell::set (Literal* cobj, const bool cflg) {
    wrlock ();
    try {
      set (cobj);
      d_cflg = cflg;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // set the cell const flag

  void Cell::setcflg (const bool cflg) {
    wrlock ();
    try {
      d_cflg = cflg;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map the cell literal to a string

  String Cell::tostring (void) const {
    rdlock ();
    try {
      String result = (p_cobj == nullptr) ? "nil" : p_cobj->tostring ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cell style

  Style* Cell::getstyle (void) const {
    rdlock ();
    try {
      Style* result = p_cstl;
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
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GET      = zone.intern ("get");
  static const long QUARK_SET      = zone.intern ("set");
  static const long QUARK_RESET    = zone.intern ("reset");
  static const long QUARK_CLEAR    = zone.intern ("clear");
  static const long QUARK_SETNAME  = zone.intern ("set-name");
  static const long QUARK_SETCONST = zone.intern ("set-const");
  static const long QUARK_TOSTRING = zone.intern ("to-string");

  // create a new object in a generic way

  Object* Cell::mknew (Vector* argv) {
    // get number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Cell;
    // check for 1 argument
    if (argc == 1) {
      Object*  lobj = argv->get (0);
      Literal* cobj = dynamic_cast <Literal*> (lobj);
      if ((lobj != nullptr) && (cobj == nullptr))
	throw Exception ("type-error", "invalid object for cell constructor",
			 Object::repr (lobj));
      return new Cell (cobj);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      Object*  lobj = argv->get (1);
      Literal* cobj = dynamic_cast <Literal*> (lobj);
      if ((lobj != nullptr) && (cobj == nullptr))
	throw Exception ("type-error", "invalid object for cell constructor",
			 Object::repr (lobj));
      return new Cell (name, cobj);
    }    
    throw Exception ("argument-error", "too many arguments with cell");
  }

  // return true if the given quark is defined

  bool Cell::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Saveas::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // set a const object to this object

  Object* Cell::cdef (Evaluable* zobj, Nameset* nset, Object* object) {
    wrlock ();
    try {
      Literal* lobj = dynamic_cast <Literal*> (object);
      if (lobj != nullptr) {
	set (lobj, true);
	zobj->post (lobj);
	unlock ();
	return lobj;
      }
      throw Exception ("type-error", "invalid object with cell cdef",
		       Object::repr (object));
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an object to this object

  Object* Cell::vdef (Evaluable* zobj, Nameset* nset, Object* object) {
    wrlock ();
    try {
      Literal* lobj = dynamic_cast <Literal*> (object);
      if (lobj != nullptr) {
	set (lobj);
	zobj->post (lobj);
	unlock ();
	return lobj;
      }
      throw Exception ("type-error", "invalid object with cell vdef",
		       Object::repr (object));
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Cell::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOSTRING) return new String (tostring ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_CLEAR) {
	clear ();
	return nullptr;
      }
      if (quark == QUARK_GET) {
	rdlock ();
	Object* result = get ();
	zobj->post (result);
	unlock ();
	return result;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETNAME) {
	String name = argv->getstring (0);
	setname (name);
	return nullptr;
      }
      if (quark == QUARK_SETCONST) {
	bool cflg = argv->getbool (0);
	setcflg (cflg);
	return nullptr;
      }
      if (quark == QUARK_SET) {
	Object*   obj = argv->get (0);
	Literal* cobj = dynamic_cast <Literal*> (obj);
	if ((obj != nullptr) && (cobj == nullptr)) 
	  throw Exception ("type-error", "invalid object to set in cell",
			   Object::repr (obj));
	set (cobj);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SET) {
	Object*   obj = argv->get (0);
	Literal* cobj = dynamic_cast <Literal*> (obj);
	if ((obj != nullptr) && (cobj == nullptr)) 
	  throw Exception ("type-error", "invalid object to set in cell",
			   Object::repr (obj));
	bool cflg = argv->getbool (1);
	set (cobj, cflg);
	return nullptr;
      }
    }    // call the saveas method
    return Saveas::apply (zobj, nset, quark, argv);
  }
}
