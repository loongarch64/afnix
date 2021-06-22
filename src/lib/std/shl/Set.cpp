// ---------------------------------------------------------------------------
// - Set.cpp                                                                 -
// - standard object library - set class implementation                      -
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

#include "Set.hpp"
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "Comparable.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure compares two objects
  static bool prv_eql_obj (Object* xobj, Object* yobj) {
    // check for strong equality
    if (xobj == yobj) return true;
    // check for x comparable
    auto xc = dynamic_cast<Comparable*>(xobj);
    if ((xc != nullptr) && (xc->equal (yobj) == true)) return true;
    // check for y comparable
    auto yc = dynamic_cast<Comparable*>(yobj);
    if ((yc != nullptr) && (yc->equal (xobj) == true)) return true;
    // not equal
    return false;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new empty set

  Set::Set (void) {
    d_size = 0;
    d_slen = 0;
    p_oset = nullptr;
  }

  // create a new set with an object

  Set::Set (Object* obj) {
    d_size = 0;
    d_slen = 0;
    p_oset = nullptr;
    add (obj);
  }

  // copy constructor for this set

  Set::Set (const Set& that) {
    that.rdlock ();
    try {
      d_size = that.d_size;
      d_slen = that.d_slen;
      p_oset = new Object*[d_size];
      for (long k = 0; k < d_slen; k++) {
	p_oset[k] = Object::iref (that.p_oset[k]);
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // copy move this set

  Set::Set (Set&& that) noexcept {
    that.wrlock ();
    try {
      // move base serial
      Serial::operator = (static_cast<Serial&&>(that));
      // copy move locally
      d_size = that.d_size; that.d_size = 0L;
      d_slen = that.d_slen; that.d_slen = 0L;
      p_oset = that.p_oset; that.p_oset = nullptr;
      that.unlock ();
    } catch (...) {
      d_size = 0L;
      d_slen = 0L;
      p_oset = nullptr;
      that.unlock ();
    }
  }
  
  // destroy this set

  Set::~Set (void) {
    reset ();
  }

  // assignment a set to this one

  Set& Set::operator = (const Set& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock everybody
    wrlock ();
    that.rdlock ();
    try {
      // clean vector first
      if (d_slen != 0) {
	for (long k = 0L; k < d_slen; k++) Object::dref (p_oset[k]);
	delete [] p_oset;
      }
      // copy old to new
      d_size = that.d_size;
      d_slen = that.d_slen;
      p_oset = new Object*[d_size];
      for (long k = 0L; k < d_slen; k++) {
	p_oset[k] = Object::iref (that.p_oset[k]);
      }
      that.unlock ();
      unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }

  // move a vector into this one

  Set& Set::operator = (Set&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock everybody
    wrlock ();
    that.wrlock ();
    try {
      // reset locally
      reset ();
      // assign base serial
      Serial::operator = (static_cast<Serial&&>(that));
      // assign locally
      d_size = that.d_size; that.d_size = 0L;
      d_slen = that.d_slen; that.d_slen = 0L;
      p_oset = that.p_oset; that.p_oset = nullptr;
      unlock ();
      that.unlock ();
    } catch (...) {
      d_size = 0L;
      d_slen = 0L;
      p_oset = nullptr;
      unlock ();
      that.unlock ();
    }
    return *this;
  }
  
  // return the class name
  String Set::repr (void) const {
    return "Set";
  }

  // return the set did

  t_word Set::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the set sid

  t_word Set::getsid (void) const {
    return SRL_OSET_SID;
  }

  // serialize this set

  void Set::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the set size
      Serial::wrlong (d_slen, os);
      // write the objects
      for (long k = 0L; k < d_slen; k++) {
	Object* obj = p_oset[k];
	if (obj == nullptr) {
	  throw Exception ("serialize-error", 
			   "invalid nil object for set serialization");
	}
	// get the object serial form
	Serial* sobj = dynamic_cast <Serial*> (obj);
	if (sobj == nullptr) {
	  throw Exception ("serial-error", "cannot serialize object", 
			   obj->repr ());
	}
	sobj->serialize (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this set

  void Set::rdstream (InputStream& is) {
    wrlock ();
    try {
      reset ();
      // get the vector length
      long size = Serial::rdlong (is);
      // read in each object
      for (long k = 0L; k < size; k++) add (Serial::deserialize (is));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this set

  void Set::reset (void) {
    wrlock ();
    try {
      for (long k = 0L; k < d_slen; k++) Object::dref (p_oset[k]);
      delete [] p_oset;
      d_size = 0L;
      d_slen = 0L;
      p_oset = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the size of this set

  long Set::length (void) const {
    rdlock ();
    try {
      long result = d_slen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an object by index

  Object* Set::get (const long index) const {
    rdlock ();
    try {
      if ((index < 0L) || (index >= d_slen)) {
	throw Exception ("index-error", "illegal index in set access");
      }
      Object* result = p_oset[index];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if the set is empty

  bool Set::empty (void) const {
    rdlock ();
    try {
      bool result = (d_slen == 0L);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
	
  // check if an object exists

  bool Set::exists (Object* obj) const {
    if (obj == nullptr) return false;
    rdlock ();
    try {
      for (long k = 0L; k < d_slen; k++) {
	if (prv_eql_obj (p_oset[k], obj) == true) {
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

  // add an object to this set

  void Set::add (Object* obj) {
    if (obj == nullptr) return;
    wrlock ();
    try {
      // check that the object is not already there
      if (exists (obj) == false) {
	// do we need to resize
	if (d_slen == d_size) resize (d_slen*2);
	// add the object
	Object::iref (p_oset[d_slen++]= obj);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove an object from this set

  bool Set::remove (Object* obj) {
    // check for nil object
    if (obj == nullptr) return false;
    // lock and remove
    wrlock ();
    bool pack = false;
    for (long k = 0L; k < d_slen; k++) {
      if (pack == false) {
	if (prv_eql_obj (p_oset[k], obj) == true) {
	  Object::dref (obj);
	  p_oset[k] = nullptr;
	  pack = true;
	}
      } else {
	p_oset[k-1] = p_oset[k];
	p_oset[k] = nullptr;
      }
    }
    if (pack == true) d_slen--;
    unlock ();
    return pack;
  }

  // replace an object with another one

  bool Set::replace (Object* tobj, Object* sobj) {
    // check for nil object
    if ((sobj == nullptr) || (tobj == nullptr)) return false;
    // lock and replace
    wrlock ();
    try {
      bool result = false;
      for (long k = 0L; k < d_slen; k++) {
	if (prv_eql_obj (p_oset[k], sobj) == true) {
	  Object::iref (tobj);
	  Object::dref (sobj);
	  p_oset[k] = tobj;
	  result = true;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // merge a set into this one

  void Set::merge (const Set& cset) {
    wrlock      ();
    cset.rdlock ();
    try {
      if (cset.p_oset != nullptr) {
	for (long k = 0L; k < cset.d_slen; k++) {
	  Object* obj = cset.p_oset[k];
	  add (obj);
	}
      }      
      cset.unlock ();
      unlock      ();
    } catch (...) {
      cset.unlock ();
      unlock      ();
      throw;
    }
  }
  // remix this set with a certain number of passes

  void Set::remix (const long pnum) {
    if (pnum <= 0L) return;
    wrlock ();
    // check for null size
    if (d_slen == 0L) {
      unlock ();
      return;
    }
    // resize the set to its cardinal
    resize (d_slen);
    // loop for remix
    for (long i = 0; i < pnum; i++) {
      for (long j = 0; j < d_slen; j++) {
	// get random indexes
	long x = Utility::longrnd (d_slen-1);
	long y = Utility::longrnd (d_slen-1);
	// check indexes
	if ((x < 0) || (x >= d_slen)) x = j;
	if ((y < 0) || (y >= d_slen)) y = j;
	if (x == y) continue;
	// swap the object
	Object* obj = p_oset[x];
	p_oset[x] = p_oset[y];
	p_oset[y] = obj;
      }
    }
    unlock ();
  }

  // get a random subset by size

  Set* Set::getrss (const long size) const {
    rdlock ();
    try {
      Set* result = new Set;
      if (p_oset != nullptr) {
	for (long i = 0; i < size; i++) {
	  long index = Utility::longrnd (d_slen-1);
	  if ((index < 0) || (index >= d_slen)) continue;
	  result->add (p_oset[index]);
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resize this set

  void Set::resize (const long size) {
    wrlock ();
    try {
      // check for valid size
      if (size < d_slen) {
	unlock ();
	return;
      }
      // process 0 case first
      if (size == 0) {
	d_size = 1;
	p_oset = new Object*[d_size];
	p_oset[0] = nullptr;
	unlock ();
	return;
      }
      // prepare new vector
      Object** oset = new Object*[size];
      for (long i = 0; i < d_slen; i++) oset[i] = p_oset[i];
      for (long i = d_slen; i < size; i++) oset[i] = nullptr;
      // clean old and restore
      delete [] p_oset;
      d_size = size;
      p_oset = oset;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a set iterator

  Iterator* Set::makeit (void) {
    rdlock ();
    try {
      Iterator* result = new Setit (this);
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
  static const long QUARK_ZONE_LENGTH = 11;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_MERGE   = zone.intern ("merge");
  static const long QUARK_REMIX   = zone.intern ("remix");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_EMPTYP  = zone.intern ("empty-p");
  static const long QUARK_EXISTP  = zone.intern ("exists-p");
  static const long QUARK_REMOVE  = zone.intern ("remove");
  static const long QUARK_GETRSS  = zone.intern ("get-random-subset");
  static const long QUARK_REPLACE = zone.intern ("replace");

  // create a new object in a generic way

  Object* Set::mknew (Vector* argv) {
    // create the set
    Set* result = new Set;
    // get the objects
    long argc = (argv == nullptr) ? 0 : argv->length ();
    for (long i = 0; i < argc; i++) result->add (argv->get (i));
    return result;
  }

  // return true if the given quark is defined

  bool Set::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Iterable::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? Serial::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark

  Object* Set::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH) return new Integer (length ());
      if (quark == QUARK_EMPTYP) return new Boolean (empty  ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) {
	Object* result = argv->get (0);
	add (result);
	zobj->post (result);
	return result;
      }
      if (quark == QUARK_EXISTP) {
	Object* obj = argv->get (0);
	return new Boolean (exists (obj));
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
      if (quark == QUARK_MERGE) {
	Object* obj = argv->get (0);
	Set* cset = dynamic_cast<Set*> (obj);
	if (cset == nullptr) {
	  throw Exception ("type-error", "invalid object with merge",
			   Object::repr (obj));
	}
	merge (*cset);
	return nullptr;
      }
      if (quark == QUARK_REMIX) {
	long num = argv->getlong (0);
	remix (num);
	return nullptr;
      }
      if (quark == QUARK_REMOVE) {
	Object* obj = argv->get (0);
	bool result = remove (obj);
	return new Boolean (result);
      }
      if (quark == QUARK_GETRSS) {
	long size = argv->getlong (0);
	return getrss (size);
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_REPLACE) {
	Object* tobj = argv->get (0);
	Object* sobj = argv->get (1);
	return new Boolean (replace (tobj, sobj));
      }
    }
    // check the iterable method
    if (Iterable::isquark (quark, true) == true) {
      return Iterable::apply (zobj, nset, quark, argv);
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }

  // -------------------------------------------------------------------------
  // - iterator section                                                      -
  // -------------------------------------------------------------------------

  // create a new set iterator

  Setit::Setit (Set* set) {
    Object::iref (p_set = set);
    d_idx = 0;
    begin ();
  }

  // destroy this set iterator

  Setit::~Setit (void) {
    Object::dref (p_set);
  }

  // return the class name

  String Setit::repr (void) const {
    return "Setit";
  }

  // reset the iterator to the begining

  void Setit::begin (void) {
    wrlock ();
    try {
      d_idx = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset the iterator to the end

  void Setit::end (void) {
    wrlock ();
    if (p_set != nullptr) p_set->rdlock ();
    try {
      if (p_set != nullptr) {
	d_idx = (p_set->d_slen == 0) ? 0 : p_set->d_slen;
      } else {
	d_idx = 0;
      }
      if (p_set != nullptr) p_set->unlock ();
      unlock ();
    } catch (...) {
      if (p_set != nullptr) p_set->unlock ();
      unlock ();
      throw;
    }
  }

  // go to the next object

  void Setit::next (void) {
    wrlock ();
    if (p_set != nullptr) p_set->rdlock ();
    try {
      if (p_set != nullptr) {
	if (++d_idx >= p_set->d_slen) d_idx = p_set->d_slen;
      } else {
	d_idx = 0;
      }
      if (p_set != nullptr) p_set->unlock ();
      unlock ();
    } catch (...) {
      if (p_set != nullptr) p_set->unlock ();
      unlock ();
      throw;
    }
  }

  // go to the previous object
  void Setit::prev (void) {
    wrlock ();
    try {
      if (--d_idx < 0) d_idx = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the object at the current position

  Object* Setit::getobj (void) const {
    rdlock ();
    if (p_set != nullptr) p_set->rdlock ();
    try {
      Object* result = nullptr;
      if ((p_set != nullptr) && (d_idx < p_set->d_slen)) {
	result = p_set->get (d_idx);
      }
      if (p_set != nullptr) p_set->unlock ();
      unlock ();
      return result;
    } catch (...) {
      if (p_set != nullptr) p_set->unlock ();
      unlock ();
      throw;
    }
  }

  // return true if the iterator is at the end

  bool Setit::isend (void) const {
    rdlock ();
    if (p_set != nullptr) p_set->rdlock ();
    try {
      bool result = false;
      if (p_set != nullptr) {
	result = (d_idx >= p_set->d_slen);
      }
      if (p_set != nullptr) p_set->unlock ();
      unlock ();
      return result;
    } catch (...) {
      if (p_set != nullptr) p_set->unlock ();
      unlock ();
      throw;
    }
  }
}
