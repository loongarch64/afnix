// ---------------------------------------------------------------------------
// - Qualified.cpp                                                           -
// - afnix engine - qualified name class implementation                      -
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

#include "Engsid.hxx"
#include "Vector.hpp"
#include "Strvec.hpp"
#include "Lexical.hpp"
#include "Integer.hpp"
#include "Nameset.hpp"
#include "Evaluable.hpp"
#include "Qualified.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure update the qualified length and quark array by name
  static void qualified_update (const String& name, long& ql, long** qa) {
    // clean the old quark array
    if (*qa != nullptr) delete [] (*qa);
    // extract the name
    Strvec path = Strvec::split (name, ":");
    ql = path.length ();
    if (ql < 2) 
      throw Exception ("syntax-error", "invalid qualified name", name);
    // build the quarks
    *qa = new long[ql];
    for (long i = 0; i < ql; i++) {
      String data = path.get (i);
      if (Lexical::valid (data) == false) {
	delete [] *qa;
	throw Exception ("syntax-error", "invalid qualified name", name);
      }
      (*qa)[i] = data.toquark ();
    }
  }

  // this procedure performs a partial evaluation and return the object
  static inline Object* qualified_eval (Evaluable* zobj, Nameset* nset, 
					const long len, long* quarks) {
    Object* bobj = nset->eval (zobj, nset, quarks[0]);
    try {
      for (long i = 1; i < len; i++) {
	if (bobj == nullptr) return nullptr;
	Object::iref (bobj);
	Object* pobj = bobj->eval (zobj, nset, quarks[i]);
	Object::dref (bobj);
	bobj = pobj;
      }
      return bobj;
    } catch (...) {
      Object::tref (bobj);
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default qualified

  Qualified::Qualified (void) {
    d_lnum   = 0L;
    d_length = 0L;
    p_quarks = nullptr;
  }

  // create a qualified with a name

  Qualified::Qualified (const String& name) {
    // save data value
    d_name = name;
    d_lnum = 0L;
    // update the qualified
    p_quarks = nullptr;
    qualified_update (name, d_length, &p_quarks);
  }

  // create a qualified with a name and a line number

  Qualified::Qualified (const String& name, const long lnum) {
    // save data value
    d_name = name;
    d_lnum = lnum;
    // update the qualified
    p_quarks = nullptr;
    qualified_update (name, d_length, &p_quarks);
  }

  // copy constructor for this qualified

  Qualified::Qualified (const Qualified& that) {
    that.rdlock ();
    try {
      // save data value
      d_name = that.d_name;
      d_lnum = that.d_lnum;
      // update the qualified
      p_quarks = nullptr;
      qualified_update (d_name, d_length, &p_quarks);    
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this qualified

  Qualified::~Qualified (void) {
    delete [] p_quarks;
  }

  // return the class name

  String Qualified::repr (void) const {
    return "Qualified";
  }

  // return a clone of this object

  Object* Qualified::clone (void) const {
    return new Qualified (*this);
  }

  // clear this qualified name

  void Qualified::clear (void) {
    wrlock ();
    try {
      d_name   = "";
      d_lnum   = 0L;
      d_length = 0L;
      delete [] p_quarks; p_quarks = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation of this qualified

  String Qualified::toliteral (void) const {
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
  
  // return a string representation of this qualified

  String Qualified::tostring (void) const {
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

  // return the serial did
  
  t_word Qualified::getdid (void) const {
    return SRL_DEOD_ENG;
  }

  // return the serial sid

  t_word Qualified::getsid (void) const {
    return SRL_QUAL_SID;
  }
  
  // serialize this qualified

  void Qualified::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      Integer lnum (d_lnum);
      d_name.wrstream (os);
      lnum.wrstream   (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this qualified

  void Qualified::rdstream (InputStream& is) {
    wrlock ();
    try {
      String  sval;
      Integer ival;
      sval.rdstream (is);
      ival.rdstream (is);
      d_name  = sval;
      d_lnum  = ival.tolong ();
      qualified_update (d_name, d_length, &p_quarks);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // the length of the qualified name path

  long Qualified::length (void) const {
    rdlock ();
    try {
      long result = d_length;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a partial name by index
  String Qualified::getname (const long index) const {
    rdlock ();
    try {
      if ((index < 0) || (index >= d_length )) {
	throw Exception ("index-error", "out of bound qualified index");
      }
      String result = String::qmap (p_quarks[index]);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the qualified line number

  long Qualified::getlnum (void) const {
    rdlock ();
    try {
      long result = d_lnum;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                         -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the qualified quarks
  static const long QUARK_MAP     = zone.intern ("map");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_GETNAME = zone.intern ("get-name");
  static const long QUARK_GETLNUM = zone.intern ("get-lnum");

  // create a qualified in a generic way

  Object* Qualified::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new Qualified;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Qualified (name);
    }
    // illegal arguments
    throw Exception ("argument-error", "too many arguments with qualified");
  }

  // return true if the given quark is defined

  bool Qualified::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // set a constant object to this object

  Object* Qualified::cdef (Evaluable* zobj, Nameset* nset, Object* object) {
    // lock and protect called object as it might be in the evaluable
    // which is changed during the eval partial
    wrlock ();
    Object::iref (object);
    // evaluate the partial object
    Object* pobj = nullptr;
    long    plen = d_length - 1;
    try {
      // get the partial object
      pobj = qualified_eval (zobj, nset, plen, p_quarks);
      if (pobj == nullptr) {
	throw Exception ("eval-error", 
			 "nil object with qualified name", d_name);
      }
    } catch (...) {
      Object::tref (object);
      unlock ();
      throw;
    }
    // evaluate with partial
    try {
      Object::iref (pobj);
      Object* result = pobj->cdef (zobj, nset, p_quarks[plen], object);
      zobj->post (result);
      // clean and unlock
      Object::dref (pobj);
      Object::tref (object);
      unlock ();
      return result;
    } catch (...) {
      Object::dref (pobj);
      Object::tref (object);
      unlock ();
      throw;
    }
  }

  // set an object to this object

  Object* Qualified::vdef (Evaluable* zobj, Nameset* nset, Object* object) {
    // lock and protect called object as it might be in the evaluable
    // which is changed during the eval partial
    wrlock ();
    Object::iref (object);
    // evaluate the partial object
    Object* pobj = nullptr;
    long plen    = d_length - 1;
    try {
      // get the partial object
      pobj = qualified_eval (zobj, nset, plen, p_quarks);
      if (pobj == nullptr) {
	throw Exception ("eval-error", 
			 "nil object with qualified name", d_name);
      }
    } catch (...) {
      Object::tref (object);
      unlock ();
      throw;
    }
    // evaluate with partial
    try {
      Object::iref (pobj);
      Object* result = pobj->vdef (zobj, nset, p_quarks[plen], object);
      zobj->post (result);
      // clean and unlock
      Object::dref (pobj);
      Object::tref (object);
      unlock ();
      return result;
    } catch (...) {
      Object::dref (pobj);
      Object::tref (object);
      unlock ();
      throw;
    }
  }

  // unreference an object

  Object* Qualified::udef (Evaluable* zobj, Nameset* nset) {
    wrlock ();
    // evaluate the partial object
    Object* pobj = nullptr;
    long    plen = d_length - 1;
    try {
      // get the partial object
      pobj = qualified_eval (zobj, nset, plen, p_quarks);
      if (pobj == nullptr) {
	throw Exception ("eval-error", 
			 "nil object with qualified name", d_name);
      }
    } catch (...) {
      unlock ();
      throw;
    }
    // unreference with the partial object
    try {
      Object::iref (pobj);
      Object* result = pobj->udef (zobj, nset, p_quarks[plen]);
      zobj->post (result);
      // clean and unlock
      Object::dref (pobj);
      unlock ();
      return result;
    } catch (...) {
      Object::dref (pobj);
      unlock ();
      throw;
    }
  }

  // evaluate this object

  Object* Qualified::eval (Evaluable* zobj, Nameset* nset) {
    rdlock ();
    Object* pobj = nullptr;
    try {
      // get the partial object
      long plen = d_length - 1;
      pobj = qualified_eval (zobj, nset, plen, p_quarks);
      if (pobj == nullptr) {
	throw Exception ("eval-error", 
			 "nil object with qualified name", d_name);
      }
      // evaluate with partial
      Object::iref (pobj);
      Object* result = pobj->eval (zobj, nset, p_quarks[plen]);
      zobj->post (result);
      // clean and unlock
      Object::dref (pobj);
      unlock ();
      return result;
    } catch (...) {
      Object::dref (pobj);
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Qualified::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH)  return new Integer (length  ());
      if (quark == QUARK_GETLNUM) return new Integer (getlnum ());
      if (quark == QUARK_MAP) {
	long qrk = p_quarks[d_length - 1];
	Object* result = (nset == nullptr) ? nullptr : nset->find (qrk);
	zobj->post (result);
	return result;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_GETNAME) {
	long index = argv->getlong (0);
	return new String (getname (index));
      }
    }
    // call the literal method
    return Literal::apply (zobj, nset, quark, argv);
  }
}
