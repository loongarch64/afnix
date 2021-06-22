// ---------------------------------------------------------------------------
// - Xref.cpp                                                                -
// - afnix:sps module - cross reference class implementation                 -
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

#include "Xref.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
   
  // create a default xref table
  
  Xref::Xref (void) {
    reset ();
  }

  // return the class name

  String Xref::repr (void) const {
    return "Xref";
  }

  // reset this xref table

  void Xref::reset (void) {
    wrlock ();
    d_htbl.reset ();
    unlock ();
  }

  // return the xref table length

  long Xref::length (void) const {
    rdlock ();
    try {
      long result = d_htbl.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the xref name by index

  String Xref::getname (const long index) const {
    rdlock ();
    try { 
      String result = d_htbl.getkey (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the index entry exists

  bool Xref::exists (const String& name) const {
    rdlock ();
    try {
      bool result = d_htbl.exists (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return an index by index

  Index* Xref::get (const long index) const {
    rdlock ();
    try { 
      Object*   iobj = d_htbl.getobj (index);
      Index*  result = dynamic_cast <Index*> (iobj);
      if (result == nullptr) {
	throw Exception ("internal-error", "nil index with object",
			 Object::repr (iobj));
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return an index by name

  Index* Xref::get (const String& name) const {
    rdlock ();
    try {
      Object* iobj = d_htbl.get (name);
      if (iobj == nullptr) {
	unlock ();
	return nullptr;
      }
      Index* indx = dynamic_cast <Index*> (iobj);
      if (indx == nullptr) {
	throw Exception ("internal-error", "nil index with object",
			 Object::repr (iobj));
      }
      unlock ();
      return indx;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return an index by name or throw an exception

  Index* Xref::lookup (const String& name) const {
    rdlock ();
    try {
      Object* iobj = d_htbl.lookup (name);
      Index*  indx = dynamic_cast <Index*> (iobj);
      if (indx == nullptr) {
	throw Exception ("internal-error", "nil index with object",
			 Object::repr (iobj));
      }
      unlock ();
      return indx;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new index by name and coordinate

  void Xref::add (const String& name, const long cidx) {
    // check the name and lock
    if (name.isnil () == true) return;
    wrlock ();
    try {
      // check if the entry exist or create it
      Object* iobj = d_htbl.get (name);
      if (iobj != nullptr) {
	Index* indx = dynamic_cast <Index*> (iobj);
	if (indx == nullptr) {
	  throw Exception ("internal-error", "nil index with object",
			   Object::repr (iobj));
	}
	indx->add (cidx);
      } else {
	Index* indx = new Index (cidx);
	d_htbl.add (name, indx);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new index by name and coordinate

  void Xref::add (const String& name, const long cidx, const long ridx) {
    // check the name and lock
    if (name.isnil () == true) return;
    wrlock ();
    try {
      // check if the entry exist or create it
      Object* iobj = d_htbl.get (name);
      if (iobj != nullptr) {
	Index* indx = dynamic_cast <Index*> (iobj);
	if (indx == nullptr) {
	  throw Exception ("internal-error", "nil index with object",
			   Object::repr (iobj));
	}
	indx->add (cidx, ridx);
      } else {
	Index* indx = new Index (cidx, ridx);
	d_htbl.add (name, indx);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new index by name and coordinate

  void Xref::add (const String& name, const long cidx, const long ridx,
		  const long sidx) {
    // check the name and lock
    if (name.isnil () == true) return;
    wrlock ();
    try {
      // check if the entry exist or create it
      Object* iobj = d_htbl.get (name);
      if (iobj != nullptr) {
	Index* indx = dynamic_cast <Index*> (iobj);
	if (indx == nullptr) {
	  throw Exception ("internal-error", "nil index with object",
			   Object::repr (iobj));
	}
	indx->add (cidx, ridx, sidx);
      } else {
	Index* indx = new Index (cidx, ridx, sidx);
	d_htbl.add (name, indx);
      }
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
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_LOOKUP  = zone.intern ("lookup");
  static const long QUARK_EXISTSP = zone.intern ("exists-p");
  static const long QUARK_GETNAME = zone.intern ("get-name");

  // create a new object in a generic way

  Object* Xref::mknew (Vector* argv) {
    // get number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Xref;
    throw Exception ("argument-error", "too many arguments with index");
  }

  // return true if the given quark is defined

  bool Xref::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply an object method with a set of arguments and a quark

  Object* Xref::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH) return new Integer (length ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GET) {
	Object* obj = argv->get (0);
	// check for an integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  long index = iobj->tolong ();
	  rdlock ();
	  try {
	    Index* indx = get (index);
	    zobj->post (indx);
	    unlock ();
	    return indx;
	  } catch (...) {
	    unlock ();
	    throw;
	  }
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  rdlock ();
	  try {
	    Index* indx = get (*sobj);
	    zobj->post (indx);
	    unlock ();
	    return indx;
	  } catch (...) {
	    unlock ();
	    throw;
	  }
	}
	// invalid argument
	throw Exception ("argument-error", "invalid argument with get",
			 Object::repr (obj));
      }
      if (quark == QUARK_LOOKUP) {
	rdlock ();
	try {
	  String name = argv->getstring (0);
	  Index* indx = lookup (name);
	  zobj->post (indx);
	  unlock ();
	  return indx;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_EXISTSP) {
	String name = argv->getstring (0);
	return new Boolean (exists (name));
      }
      if (quark == QUARK_GETNAME) {
	long index = argv->getlong (0);
	return new String (getname (index));
      }    
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_ADD) {
	String name = argv->getstring (0);
	long   cidx = argv->getlong (1);
	add (name, cidx);
	return nullptr;
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_ADD) {
	String name = argv->getstring (0);
	long   cidx = argv->getlong (1);
	long   ridx = argv->getlong (2);
	add (name, cidx, ridx);
	return nullptr;
      }
    }
    // dispatch 4 arguments
    if (argc == 4) {
      if (quark == QUARK_ADD) {
	String name = argv->getstring (0);
	long   cidx = argv->getlong (1);
	long   ridx = argv->getlong (2);
	long   sidx = argv->getlong (3);
	add (name, cidx, ridx, sidx);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
