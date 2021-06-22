// ---------------------------------------------------------------------------
// - Plist.cpp                                                               -
// - standard object library - property list class implementation            -
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

#include "Real.hpp"
#include "Plist.hpp"
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new empty plist

  Plist::Plist (void) {
    reset ();
  }

  // create a plist by case flag

  Plist::Plist (const bool cifg) {
    reset ();
    d_hash.setcifg (cifg);    
  }

  // create a plist by name

  Plist::Plist (const String& name) : Taggable (name) {
    reset ();
  }

  // create a plist by name and info

  Plist::Plist (const String& name,
		const String& info) : Taggable (name, info) {
    reset ();
  }

  // copy construct this plist

  Plist::Plist (const Plist& that) {
    reset ();
    that.rdlock ();
    try {
      // copy base taggable
      Taggable::operator = (that);
      // copy locally
      d_hash.setcifg (that.d_hash.getcifg ());
      // copy the properties
      long plen = that.length ();
      for (long i = 0; i < plen; i++) {
	Property* prop = that.get (i);
	if (prop == nullptr) continue;
	add (new Property (*prop));
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this set

  Plist::~Plist (void) {
    reset ();
  }
  
  //  assign a plist to this one

  Plist& Plist::operator = (const Plist& that) {
    // check for equality
    if (this == &that) return *this;
    // lock and copy
    wrlock ();
    that.rdlock ();
    try {
      // reset the property list list
      reset ();
      // assign the taggable object
      Taggable::operator = (that);
      // assign locally
      d_hash.setcifg (that.d_hash.getcifg ());
      // copy the property list
      long plen = that.length ();
      for (long i = 0; i < plen; i++) {
	Property* prop = that.get (i);
	if (prop == nullptr) continue;
	add (new Property (*prop));
      }
      // unlock and done
      that.unlock ();
      unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }

  // add a plist by merging them

  Plist& Plist::operator += (const Plist& plst) {
    // check for self insertion
    if (this == &plst) return *this;
    wrlock ();
    try {
      // get the list length
      long plen = plst.length ();
      // loop in the plist
      for (long k = 0; k < plen; k++) set (plst.get (k));
      // done
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the class name

  String Plist::repr (void) const {
    return "Plist";
  }

  // return a clone of this object

  Object* Plist::clone (void) const {
    return new Plist (*this);
  }

  // return the plist did

  t_word Plist::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the plist sid

  t_word Plist::getsid (void) const {
    return SRL_PLST_SID;
  }

  // serialize this plist

  void Plist::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // save base taggable
      Taggable::wrstream (os);
      // save the property list
      d_list.wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this plist

  void Plist::rdstream (InputStream& is) {
    wrlock ();
    try {
      // reset the property list
      reset  ();
      // read the base taggable
      Taggable::rdstream (is);
      // read the property list
      d_list.rdstream (is);
      // update the hash table
      long len = d_list.length ();
      for (long i = 0; i < len; i++) {
	Property* prop = dynamic_cast <Property*> (d_list.get (i));
	if (prop == nullptr) {
	  throw Exception ("internal-error", 
			   "invalid nil property whie deserializing");
	}
	String name = prop->getname ();
	d_hash.add (name, prop);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this plist

  void Plist::reset (void) {
    wrlock ();
    try {
      d_list.reset ();
      d_hash.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the list is empty

  bool Plist::empty (void) const {
    rdlock ();
    try {
      bool result = d_list.empty ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the length of this plist

  long Plist::length (void) const {
    rdlock ();
    try {
      long result = d_list.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a property object by index

  Property* Plist::get (const long index) const {
    rdlock ();
    try {
      Property* result = dynamic_cast <Property*> (d_list.get (index));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // locate a property index by name

  long Plist::locate (const String& name) const {
    rdlock ();
    try {
      long llen = d_list.length ();
      long result = -1L;
      for (long k = 0L; k < llen; k++) {
	Property* prop = dynamic_cast <Property*> (d_list.get (k));
	if (prop == nullptr) continue;
	if (prop->getname () == name) {
	  result = k;
	  break;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a property by name

  Property* Plist::find (const String& name) const {
    rdlock ();
    try {
      Property* result = dynamic_cast <Property*> (d_hash.get (name));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a property exists by name

  bool Plist::exists (const String& name) const {
    rdlock ();
    try {
      bool result = d_hash.exists (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a property exists by name with a certain value

  bool Plist::exists (const String& name, const Literal& lval) const {
    rdlock ();
    try {
      if (d_hash.exists (name) == false) {
	unlock ();
	return false;
      }
      // get the property value
      String pval = getpval (name);
      bool result = (pval == lval.tostring ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a property to this plist

  void Plist::add (Property* prop) {
    if (prop == nullptr) return;
    wrlock ();
    try {
      // get the propery name
      String name = prop->getname ();
      // check if the property alreay exists
      if (exists (name) == true) {
	unlock ();
	throw Exception ("plist-error", "propety already exists", name);
      }
      // add the property in the list
      d_list.add (prop);
      // add the property in the hash
      d_hash.add (name, prop);
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a property by name and boolean value

  void Plist::add (const String& name, const bool bval) {
    Property* prop = new Property (name, bval);
    wrlock ();
    try {
      add (prop);
      unlock ();
    } catch (...) {
      delete prop;
      unlock ();
      throw;
    }
  }

  // add a property by name and integer value

  void Plist::add (const String& name, const t_long ival) {
    Property* prop = new Property (name, ival);
    wrlock ();
    try {
      add (prop);
      unlock ();
    } catch (...) {
      delete prop;
      unlock ();
      throw;
    }
  }

  // add a property by name and real value

  void Plist::add (const String& name, const t_real rval) {
    Property* prop = new Property (name, rval);
    wrlock ();
    try {
      add (prop);
      unlock ();
    } catch (...) {
      delete prop;
      unlock ();
      throw;
    }
  }

  // add a property by name and value

  void Plist::add (const String& name, const Literal& lval) {
    Property* prop = new Property (name, lval);
    wrlock ();
    try {
      add (prop);
      unlock ();
    } catch (...) {
      delete prop;
      unlock ();
      throw;
    }
  }

  // add a property by name, info and boolean value

  void Plist::add (const String& name, const String& info, const bool bval) {
    Property* prop = new Property (name, info, bval);
    wrlock ();
    try {
      add (prop);
      unlock ();
    } catch (...) {
      delete prop;
      unlock ();
      throw;
    }
  }

  // add a property by name, info and integer value

  void Plist::add (const String& name, const String& info, 
		   const t_long  ival) {
    Property* prop = new Property (name, info, ival);
    wrlock ();
    try {
      add (prop);
      unlock ();
    } catch (...) {
      delete prop;
      unlock ();
      throw;
    }
  }

  // add a property by name, info and real value

  void Plist::add (const String& name, const String& info,
		   const t_real  rval) {
    Property* prop = new Property (name, info, rval);
    wrlock ();
    try {
      add (prop);
      unlock ();
    } catch (...) {
      delete prop;
      unlock ();
      throw;
    }
  }

  // add a property by name, info and value

  void Plist::add (const String&  name, const String& info,
		   const Literal& lval) {
    Property* prop = new Property (name, info, lval);
    wrlock ();
    try {
      add (prop);
      unlock ();
    } catch (...) {
      delete prop;
      unlock ();
      throw;
    }
  }

  // get a property by name or throw an exception

  Property* Plist::lookup (const String& name) const {
    rdlock ();
    try {
      Property* result = dynamic_cast <Property*> (d_hash.lookup (name));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a property literal by name

  Literal* Plist::toliteral (const String& name) const {
    rdlock ();
    try {
      // get the property
      Property* prop = lookup (name);
      // the property literal
      Literal* result = (prop == nullptr) ? nullptr : prop->toliteral();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a property value by name

  String Plist::getpval (const String& name) const {
    rdlock ();
    try {
      // get the property
      Property* prop = lookup (name);
      // map the value
      String result = prop->getpval ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a property to a boolean value

  bool Plist::tobool (const String& name) const {
    rdlock ();
    try {
      // get the property
      Property* prop = lookup (name);
      // map to a boolean value
      bool result = prop->tobool ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a property to a integer boolean value

  bool Plist::toboil (const String& name) const {
    rdlock ();
    try {
      // get the property
      Property* prop = lookup (name);
      // map to a boolean value
      bool result = prop->toboil ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a property to an integer value

  t_long Plist::tolong (const String& name) const {
    rdlock ();
    try {
      // get the property
      Property* prop = lookup (name);
      // map to an integer value
      t_long result = prop->tolong ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a property to a real value

  t_real Plist::toreal (const String& name) const {
    rdlock ();
    try {
      // get the property
      Property* prop = lookup (name);
      // map to a real value
      t_real result = prop->toreal ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a property to this plist

  void Plist::set (Property* prop) {
    if (prop == nullptr) return;
    wrlock ();
    try {
      // get the propery name
      String name = prop->getname ();
      if (exists (name) == true) {
	Property* pobj = lookup (name);
	(*pobj) = (*prop);
      } else {
	// set the property in the list
	d_list.add (prop);
	// add the property in the hash
	d_hash.add (name, prop);
      }
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a property by name and boolean value

  void Plist::set (const String& name, const bool bval) {
    wrlock ();
    try {
      if (exists (name) == true) {
	Property* prop = lookup (name);
	prop->setpval (bval);
      } else {
	add (name, bval);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a property by name and integer value

  void Plist::set (const String& name, const t_long ival) {
    wrlock ();
    try {
      if (exists (name) == true) {
	Property* prop = lookup (name);
	prop->setpval (ival);
      } else {
	add (name, ival);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a property by name and real value

  void Plist::set (const String& name, const t_real rval) {
    wrlock ();
    try {
      if (exists (name) == true) {
	Property* prop = lookup (name);
	prop->setpval (rval);
      } else {
	add (name, rval);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a property by name and value

  void Plist::set (const String& name, const Literal& lval) {
    wrlock ();
    try {
      if (exists (name) == true) {
	Property* prop = lookup (name);
	prop->setpval (lval);
      } else {
	add (name, lval);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a property by name, info and value

  void Plist::set (const String&  name, const String& info, 
		   const Literal& lval) {
    wrlock ();
    try {
      if (exists (name) == true) {
	Property* prop = lookup (name);
	prop->setpval (lval);
      } else {
	add (name, info, lval);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // merge a plist into this one

  Plist Plist::merge (const Plist& plst) const {
    rdlock ();
    try {
      // get the resukt plist
      Plist result = *this;
      // get the list length
      long plen = plst.length ();
      // loop in the plist
      for (long k = 0; k < plen; k++) result.set (plst.get (k));
      // done
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // merge a plist into this one by index

  Plist Plist::merge (const Plist& plst, const long pidx) const {
    rdlock ();
    try {
      // get the resukt plist
      Plist result = *this;
      // get the list length
      long plen = plst.length ();
      // loop in the plist
      for (long k = 0; k < plen; k++) {
	Property* prop = plst.get (k);
	if (prop == nullptr) continue;
	String name = prop->getname () + "[" + pidx + "]";
	prop->setname (name);
	result.set (prop);
      }
      // done
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // convert this property list into a print table

  PrintTable* Plist::toptbl (const bool iflg) const {
    rdlock ();
    try {
      Style lstl;
      PrintTable* result = toptbl (iflg, lstl);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert this property list into a print table

  PrintTable* Plist::toptbl (const bool iflg, const Style& lstl) const {
    rdlock ();
    PrintTable* result = nullptr;
    try {
      // create a 2 or 3 columns print table
      result = (iflg == true) ? new PrintTable (3) : new PrintTable (2);
      // loop in the plist
      long plen = length ();
      for (long i = 0; i < plen; i++) {
	// get the property
	Property* prop = get (i);
	if (prop == nullptr) continue;
	// get the property name
	String name = prop->getname ();
	// get the property value
	String pval = prop->format (lstl);
	// add a new row
	long row = result->add ();
	// set the table value
	result->set (row, 0, name);
	if (iflg == true) {
	  String info = prop->getinfo ();
	  result->set (row, 1, info);
	  result->set (row, 2, pval);
	} else {
	  result->set (row, 1, pval);
	}
      }
      // here it is
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // return a plist iterator

  Iterator* Plist::makeit (void) {
    rdlock ();
    try {
      Iterator* result = new Plistit (this);
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
  static const long QUARK_ZONE_LENGTH = 17;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_AEQ     = zone.intern ("+=");
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_SET     = zone.intern ("set");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_FIND    = zone.intern ("find");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_MERGE   = zone.intern ("merge");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_LOOKUP  = zone.intern ("lookup");
  static const long QUARK_EMPTYP  = zone.intern ("empty-p");
  static const long QUARK_EXISTP  = zone.intern ("exists-p");
  static const long QUARK_TOPLIT  = zone.intern ("to-literal");
  static const long QUARK_TOBOOL  = zone.intern ("to-boolean");
  static const long QUARK_TOLONG  = zone.intern ("to-integer");
  static const long QUARK_TOREAL  = zone.intern ("to-real");
  static const long QUARK_TOPTBL  = zone.intern ("to-print-table");
  static const long QUARK_GETPVAL = zone.intern ("get-value");

  // create a new object in a generic way

  Object* Plist::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new Plist;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Plist (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Plist (name, info);
    }
    // too many arguments
    throw Exception ("argument-error", 
		     "too many argument with plist constructor");
  }

  // return true if the given quark is defined

  bool Plist::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Iterable::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? Taggable::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Plist::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_EMPTYP)  return new Boolean (empty  ());
      if (quark == QUARK_LENGTH)  return new Integer (length ());
      if (quark == QUARK_TOPTBL)  return toptbl (false);
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    
    // dispatch 1 argument
    if (argc == 1) {	
      if (quark == QUARK_ADD) {
	Object* obj = argv->get (0);
	Property* prop = dynamic_cast <Property*> (obj);
	if ((obj != nullptr) && (prop == nullptr)) {
	  throw Exception ("type-error", "invalid object with add",
			   Object::repr (obj));
	}
	add (prop);
	return nullptr;
      }
      if (quark == QUARK_AEQ) {
	wrlock ();
	try {
	  Object*  obj = argv->get (0);
	  Plist*  pobj = dynamic_cast <Plist*> (obj);
	  if (pobj == nullptr) {
	    throw Exception ("type-error", "invalid object with operator +=",
			     Object::repr (obj));
	  }
	  *this = *this+= *pobj;
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_MERGE) {
	Object*  obj = argv->get (0);
	Plist*  pobj = dynamic_cast <Plist*> (obj);
	if (pobj == nullptr) {
	  throw Exception ("type-error", "invalid object with merge",
			   Object::repr (obj));
	}
	return new Plist (merge(*pobj));
      }
      if (quark == QUARK_SET) {
	Object* obj = argv->get (0);
	Property* prop = dynamic_cast <Property*> (obj);
	if ((obj != nullptr) && (prop == nullptr)) {
	  throw Exception ("type-error", "invalid object with set",
			   Object::repr (obj));
	}
	set (prop);
	return nullptr;
      }
      if (quark == QUARK_FIND) {
	rdlock ();
	try {
	  String name = argv->getstring (0);
	  Object* result = find (name);
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
	  String name = argv->getstring (0);
	  Object* result = lookup (name);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_EXISTP) {
	String name = argv->getstring (0);
	return new Boolean (exists (name));
      }
      if (quark == QUARK_TOPLIT) {
	String name = argv->getstring (0);
	return toliteral (name);
      }
      if (quark == QUARK_TOBOOL) {
	String name = argv->getstring (0);
	return new Boolean (tobool (name));
      }
      if (quark == QUARK_TOLONG) {
	String name = argv->getstring (0);
	return new Integer (tolong (name));
      }
      if (quark == QUARK_TOREAL) {
	String name = argv->getstring (0);
	return new Real (toreal (name));
      }
      if (quark == QUARK_GETPVAL) {
	String name = argv->getstring (0);
	return new String (getpval (name));
      }
      if (quark == QUARK_GET) {
	long index = argv->getlong (0);
	rdlock();
	try {
	  Object* result = get (index);
	  zobj->post (result);
	  unlock ();	    
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_TOPTBL) {
	bool iflg = argv->getbool (0);
	return toptbl (iflg);
      }      
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_EXISTP) {
	String   name = argv->getstring (0);
	Object*   obj = argv->get (1);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if (lobj == nullptr) {
	throw Exception ("type-error", "invalid object with exists",
			 Object::repr (obj));
	}
	return new Boolean (exists (name, *lobj));
      }
      if (quark == QUARK_ADD) {
	String   name = argv->getstring (0);
	Object*   obj = argv->get (1);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if (lobj == nullptr) {
	throw Exception ("type-error", "invalid object with add",
			 Object::repr (obj));
	}
	add (name, *lobj);
	return nullptr;
      }
      if (quark == QUARK_SET) {
	String   name = argv->getstring (0);
	Object*   obj = argv->get (1);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if (lobj == nullptr) {
	throw Exception ("type-error", "invalid object with set",
			 Object::repr (obj));
	}
	set (name, *lobj);
	return nullptr;
      }
      if (quark == QUARK_MERGE) {
	Object*  obj = argv->get (0);
	Plist*  pobj = dynamic_cast <Plist*> (obj);
	if (pobj == nullptr) {
	  throw Exception ("type-error", "invalid object with merge",
			   Object::repr (obj));
	}
	long pidx = argv->getlong (1);
	return new Plist (merge(*pobj, pidx));
      }
      if (quark == QUARK_TOPTBL) {
	bool iflg = argv->getbool (0);
	Object* obj = argv->get (1);
	Style* sobj = dynamic_cast <Style*> (obj);
	if (sobj == nullptr) {
	  throw Exception ("type-error", "invalid object for print table style",
			   Object::repr (obj));
	}
	return toptbl (iflg, *sobj);
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_ADD) {
	String   name = argv->getstring (0);
	String   info = argv->getstring (1);
	Object*   obj = argv->get (2);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if (lobj == nullptr) {
	throw Exception ("type-error", "invalid object with add",
			 Object::repr (obj));
	}
	add (name, info, *lobj);
	return nullptr;
      }
      if (quark == QUARK_SET) {
	String   name = argv->getstring (0);
	String   info = argv->getstring (1);
	Object*   obj = argv->get (2);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if (lobj == nullptr) {
	throw Exception ("type-error", "invalid object with add",
			 Object::repr (obj));
	}
	set (name, info, *lobj);
	return nullptr;
      }
    }
    // check the iterable method
    if (Iterable::isquark (quark, true) == true) {
      return Iterable::apply (zobj, nset, quark, argv);
    }
    // call the taggable method
    return Taggable::apply (zobj, nset, quark, argv);
  }

  // -------------------------------------------------------------------------
  // - iterator section                                                      -
  // -------------------------------------------------------------------------

  // create a new set iterator

  Plistit::Plistit (Plist* plst) {
    Object::iref (p_plst = plst);
    d_pidx = 0;
    begin ();
  }

  // destroy this set iterator

  Plistit::~Plistit (void) {
    Object::dref (p_plst);
  }

  // return the class name

  String Plistit::repr (void) const {
    return "Plistit";
  }

  // reset the iterator to the begining

  void Plistit::begin (void) {
    wrlock ();
    try {
      d_pidx = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset the iterator to the end

  void Plistit::end (void) {
    wrlock ();
    if (p_plst != nullptr) p_plst->rdlock ();
    try {
      if (p_plst != nullptr) {
	long plen = p_plst->length ();
	d_pidx = (plen == 0) ? 0 : plen - 1;
      } else {
	d_pidx = 0;
      }
      if (p_plst != nullptr) p_plst->unlock ();    
      unlock ();
    } catch (...) {
      if (p_plst != nullptr) p_plst->unlock ();    
      unlock ();
      throw;
    }
  }

  // go to the next object

  void Plistit::next (void) {
    wrlock ();
    if (p_plst != nullptr) p_plst->rdlock ();
    try {
      if (p_plst != nullptr) {
	long plen = p_plst->length ();      
	if (++d_pidx >= plen) d_pidx = plen;
      } else {
	d_pidx = 0;
      }
      if (p_plst != nullptr) p_plst->unlock ();    
      unlock ();
    } catch (...) {
      if (p_plst != nullptr) p_plst->unlock ();    
      unlock ();
      throw;
    }
  }

  // go to the previous object

  void Plistit::prev (void) {
    wrlock ();
    try {
      if (--d_pidx < 0) d_pidx = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the object at the current position

  Object* Plistit::getobj (void) const {
    rdlock ();
    if (p_plst != nullptr) p_plst->rdlock ();
    try {
      Object* result = nullptr;
      long plen = p_plst->length ();            
      if ((p_plst != nullptr) && (d_pidx < plen)) {
	result = p_plst->get (d_pidx);
      }
      if (p_plst != nullptr) p_plst->unlock ();    
      unlock ();
      return result;
    } catch (...) {
      if (p_plst != nullptr) p_plst->unlock ();    
      unlock ();
      throw;
    }
  }

  // return true if the iterator is at the end

  bool Plistit::isend (void) const {
    rdlock ();
    if (p_plst != nullptr) p_plst->rdlock ();
    try {
      bool result = false;
      if (p_plst != nullptr) {
	long plen = p_plst->length ();            
	result = (d_pidx >= plen);
      }
      if (p_plst != nullptr) p_plst->unlock ();    
      unlock ();
      return result;
    } catch (...) {
      if (p_plst != nullptr) p_plst->unlock ();    
      unlock ();
      throw;
    }
  }
}
