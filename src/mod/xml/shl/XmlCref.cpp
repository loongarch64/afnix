// ---------------------------------------------------------------------------
// - XmlCref.cpp                                                             -
// - afnix:xml module - xml character reference node class implementation    -
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

#include "XmlCref.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Character.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure convert a quad value to a string reference value
  static String cval_to_xref (const t_quad cval) {
    String result = "x";
    result += Utility::tohexa (cval);
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default cref node

  XmlCref::XmlCref (void) {
    setcval (nilq);
  }

  // create a text node by value

  XmlCref::XmlCref (const t_quad cval) {
    setcval (cval);
  }

  // return the document class name

  String XmlCref::repr (void) const {
    return "XmlCref";
  }

  // get a clone of this node

  Object* XmlCref::clone (void) const {
    rdlock ();
    try {
      // create a new node
      XmlCref* result = new XmlCref (d_cval);
      // copy the ref arguments
      result->d_xref = d_xref;
      // update the node info
      result->d_eflg = d_eflg;
      result->d_lnum = d_lnum;
      result->d_snam = d_snam;
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cref node value

  void XmlCref::setcval (const t_quad cval) {
    wrlock ();
    try {
      d_cval = cval;
      d_xref = cval_to_xref (d_cval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cref value

  t_quad XmlCref::getcval (void) const {
    rdlock ();
    try {
      t_quad result = d_cval;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the reference node content as a text

  String XmlCref::totext (void) const {
    rdlock ();
    try {
      String result = d_cval;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // write a cref node to a buffer

  void XmlCref::write (Buffer& buf) const {
    rdlock ();
    try {
      buf.add ("&#");
      buf.add (d_xref);
      buf.add (";");
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a cref node to an output stream

  void XmlCref::write (OutputStream& os) const {
    rdlock ();
    try {
      os.write ("&#");
      os.write (d_xref);
      os.write (";");
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
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETCVAL = zone.intern ("set-value");
  static const long QUARK_GETCVAL = zone.intern ("get-value");

  // create a new object in a generic way

  Object* XmlCref::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XmlCref;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a character
      Character* cobj = dynamic_cast <Character*> (obj);
      if (cobj != nullptr) {
	t_quad cval = cobj->toquad ();
	return new XmlCref (cval);
      }
      // check for an integer
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj != nullptr) {
	t_quad cval = iobj->tolong ();
	return new XmlCref (cval);
      }
      // invalid object
      throw Exception ("type-error", "invalid object for cref constructor",
		       Object::repr (obj));
    }
    throw Exception ("argument-error",
                     "too many arguments with xml cref node constructor");
  }

  // return true if the given quark is defined

  bool XmlCref::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? XmlRef::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XmlCref::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETCVAL) return new Character (getcval ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETCVAL) {
	Object* obj = argv->get (0);
	// check for a character
	Character* cobj = dynamic_cast <Character*> (obj);
	if (cobj != nullptr) {
	  t_quad cval = cobj->toquad ();
	  setcval (cval);
	  return nullptr;
      }
      // check for an integer
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj != nullptr) {
	t_quad cval = iobj->tolong ();
	setcval (cval);
	return nullptr;
      }
      // invalid object
      throw Exception ("type-error", "invalid object for set-value",
		       Object::repr (obj));
      }
    }
    // call the node method
    return XmlRef::apply (zobj, nset, quark, argv);
  }
}
