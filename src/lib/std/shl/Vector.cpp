// ---------------------------------------------------------------------------
// - Vector.cpp                                                              -
// - standard object library - dynamic vector class implementation           -
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

#include "Byte.hpp"
#include "Cons.hpp"
#include "Real.hpp"
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Character.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty vector

  Vector::Vector (void) {
    d_size = 0L;
    d_vlen = 0L;
    p_vobj = nullptr;
  }

  // create a vector with an original size
  
  Vector::Vector (const long size) {
    if (size < 0L) {
      throw Exception ("size-error","negative vector size");
    }
    d_size = size;
    d_vlen = 0L;
    p_vobj = new Object*[d_size];
    for (long k = 0L; k < d_size; k++) p_vobj[k] = nullptr;
  }
  
  // copy constructor for this vector

  Vector::Vector (const Vector& that) {
    that.rdlock ();
    try {
      d_size = that.d_size;
      d_vlen = that.d_vlen;
      p_vobj = new Object*[d_size];
      for (long k = 0L; k < d_vlen; k++) {
	p_vobj[k] = Object::iref (that.p_vobj[k]);
      }
      for (long k = d_vlen; k < d_size; k++) {
	p_vobj[k] = nullptr;
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // copy move this vector

  Vector::Vector (Vector&& that) noexcept {
    that.wrlock ();
    try {
      // move base collectable
      Collectable::operator = (static_cast<Collectable&&>(that));
      // copy move locally
      d_size = that.d_size; that.d_size = 0L;
      d_vlen = that.d_vlen; that.d_vlen = 0L;
      p_vobj = that.p_vobj; that.p_vobj = nullptr;
    } catch (...) {
      d_size = 0L;
      d_vlen = 0L;
      p_vobj = nullptr;
    }
    that.unlock (); 
  }
  
  // destroy this vector

  Vector::~Vector (void) {
    for (long k = 0L; k < d_vlen; k++) Object::dref (p_vobj[k]);
    delete [] p_vobj;
  }

  // assign a vector to this one

  Vector& Vector::operator = (const Vector& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // clean vector first
      if (d_vlen != 0) {
	for (long k = 0L; k < d_vlen; k++) Object::dref (p_vobj[k]);
	delete [] p_vobj; p_vobj = nullptr;
      }
      // copy old to new
      d_size = that.d_size;
      d_vlen = that.d_vlen;
      p_vobj = new Object*[d_size];
      for (long k = 0L; k < d_vlen; k++) {
	p_vobj[k] = Object::iref (that.p_vobj[k]);
      }
      for (long k = d_vlen; k < d_size; k++) {
	p_vobj[k] = nullptr;
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

  Vector& Vector::operator = (Vector&& that) noexcept {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // reset locally
      reset ();
      // assign base collectable
      Collectable::operator = (static_cast<Collectable&&>(that));
      // move locally
      d_size = that.d_size; that.d_size = 0L;
      d_vlen = that.d_vlen; that.d_vlen = 0L;
      p_vobj = that.p_vobj; that.p_vobj = nullptr;
    } catch (...) {
      d_size = 0L;
      d_vlen = 0L;
      p_vobj = nullptr;
    }      
    unlock ();
    that.unlock ();
    return *this;
  }
  
  // return the class name

  String Vector::repr (void) const {
    return "Vector";
  }

  // get a clone of this object

  Object* Vector::clone (void) const {
    return new Vector (*this);
  }

  // return the vector did

  t_word Vector::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the vector sid

  t_word Vector::getsid (void) const {
    return SRL_VECT_SID;
  }

  // serialize this vector

  void Vector::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the vector length
      Serial::wrlong (d_vlen, os);
      // write the objects
      for (long i = 0; i < d_vlen; i++) {
	Object* obj = get (i);
	if (obj == nullptr) {
	  Serial::wrnilid (os);
	} else {
	  Serial* sobj = dynamic_cast <Serial*> (obj);
	  if (sobj == nullptr) {
	    throw Exception ("serial-error", "cannot serialize object", 
			     obj->repr ());
	  }
	  sobj->serialize (os);
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this vector

  void Vector::rdstream (InputStream& is) {
    wrlock ();
    try {
      reset ();
      // get the vector length
      long len = Serial::rdlong (is);
      // read in each object
      for (long i = 0; i < len; i++) add (Serial::deserialize (is));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // release this object links

  void Vector::release (void) {
    wrlock ();
    try {
      for (long k = 0L; k < d_vlen; k++) {
	auto co = dynamic_cast <Collectable*> (p_vobj[k]);
	if (co != nullptr) co->release ();
      }
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // reset this vector

  void Vector::reset (void) {
    wrlock ();
    try {
      for (long k = 0L; k < d_vlen; k++) Object::dref (p_vobj[k]);
      d_size = 0L;
      d_vlen = 0;
      delete [] p_vobj; p_vobj = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the vector is empty

  bool Vector::empty (void) const {
    rdlock ();
    try {
      bool result = (d_vlen == 0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the object exists in this vector

  bool Vector::exists (Object* object) const {
    rdlock ();
    try {
      for (long i = 0; i < d_vlen; i++) {
	if (p_vobj[i] == object) {
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

  // get the number of element in this vector

  long Vector::length (void) const {
    rdlock ();
    try {
      long result = d_vlen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new element in this vector

  void Vector::add (Object* object) {
    wrlock ();
    try {
      // check if we have to resize the vector
      if (d_vlen >= d_size) {
	long size = (d_size <= 0) ? 1 : d_size * 2;
	Object** vector = new Object*[size];
	for (long i = 0; i < d_vlen; i++)
	  vector[i] = p_vobj[i];
	delete [] p_vobj;
	d_size = size;
	p_vobj = vector;
      }
      // set the object in this vector
      p_vobj[d_vlen++] = Object::iref (object);
      // unlock the vector
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new element in this vector at a certain position

  void Vector::add (const long index, Object* object) {
    wrlock ();
    try {
      // check for negative index
      if (index < 0) {
	throw Exception ("index-error", "out-of-bound index in add");
      }
      // check for large index
      if (index >= d_vlen) {
	// resize upto the index
	for (long k = d_vlen; k < index; k++) add (nullptr);
	add (object);
	unlock ();
	return;
      }
      // check if we have to resize the vector
      if (d_vlen >= d_size) {
	long size = (d_size <= 0) ? 1 : d_size * 2;
	Object** vector = new Object*[size];
	for (long i = 0; i < d_vlen; i++)
	  vector[i] = p_vobj[i];
	delete [] p_vobj;
	d_size = size;
	p_vobj = vector;
      }
      // move the objects
      for (long i = d_vlen; i > index; i--) {
	p_vobj[i] = p_vobj[i-1];
      }
      // set the object in this vector
      p_vobj[index] = Object::iref (object);
      // adjust the length
      d_vlen++;
      // unlock the vector
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set an object at a certain position in this vector. The old object is
  // destroyed.
  
  void Vector::set (const long index, Object* object) {
    wrlock ();
    try {
      // check that we are bounded
      if (index >= d_vlen) {
	throw Exception ("index-error","index is out of range");
      }
      // protect the object first
      Object::iref (object);
      // set the object
      Object::dref (p_vobj[index]);
      p_vobj[index] = object;
      // unlock the vector
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an object at a certain position
  
  Object* Vector::get (const long index) const {
    rdlock ();
    try {
      // check that we are bounded
      if ((index < 0) || (index >= d_vlen)) {
	throw Exception ("index-error","index is out of range");
      }
      // get the object and unlock
      Object* result = p_vobj[index];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the first object in this vector
    
  Object* Vector::first (void) const {
    rdlock ();
    try {
      Object* result = get (0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the last object in this vector

  Object* Vector::last (void) const {
    rdlock ();
    try {
      Object* result = get (d_vlen-1);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // pop the first object from this vector

  Object* Vector::pop (void) {
    wrlock ();
    try {
      // get the first object
      Object* result = get (0);
      // shift the remaining arguments
      for (long i = 1; i < d_vlen; i++) {
	p_vobj[i-1] = p_vobj[i];
	p_vobj[i]   = nullptr;
      }
      // fix length and result object
      d_vlen--;
      Object::tref (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pop the last object from this vector

  Object* Vector::rml (void) {
    wrlock ();
    try {
      // get the last object
      Object* result = last ();
      // fix length and result object
      if (d_vlen > 0L) d_vlen--;
      Object::tref (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // find an object in this vector

  long Vector::find (Object* object) {
    if (object == nullptr) return -1;
    rdlock ();
    try {
      for (long i = 0; i < d_vlen; i++) {
	if (p_vobj[i] == object) {
	  unlock ();
	  return i;
	}
      }
      unlock ();
      return -1;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove an object by index and repack the vector

  void Vector::remove (const long index) {
    wrlock ();
    try {
      if ((index < 0) || (index >= d_vlen)) {
	throw Exception ("index-error","index is out of range");
      }
      // remove the object
      Object::dref (p_vobj[index]);
      // repack the vector
      long mark = d_vlen - 1;
      for (long i = index; i < mark; i++) {
	p_vobj[i] = p_vobj[i+1];
      }
      d_vlen = mark;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove and object and repack the vector

  void Vector::remove (Object* object) {
    if (object == nullptr) return;
    wrlock ();
    try {
      long index = find (object);
      if (index == -1) {
	unlock ();
	return;
      }
      remove (index);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // shift the vector and create a new one

  Vector* Vector::shift (void) const {
    rdlock ();
    Vector* result = nullptr;
    try {
      result = new Vector;
      for (long i = 1; i < d_vlen; i++) {
	result->add (p_vobj[i]);
      }
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // merge a vector into this one

  void Vector::merge (const Vector& v) {
    wrlock ();
    try {
      long len = v.length ();
      for (long i = 0; i < len; i++) add (v.get (i));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an integer value from an index

  t_long Vector::getlong (const long index) const {
    rdlock ();
    try {
      Object*  obj  = get (index);
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj == nullptr) 
	throw Exception ("type-error", "looking for integer but got",
			 Object::repr (obj));
      t_long result = iobj->tolong ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an integer value from an index

  t_octa Vector::getocta (const long index) const {
    rdlock ();
    try {
      Object*  obj  = get (index);
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj == nullptr) 
	throw Exception ("type-error", "looking for integer but got",
			 Object::repr (obj));
      t_octa result = iobj->tolong ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a real value from an index

  t_real Vector::getreal (const long index) const {
    rdlock ();
    try {
      Object* obj  = get (index);
      Real*   zobj = dynamic_cast <Real*> (obj);
      if (zobj == nullptr) 
	throw Exception ("type-error", "looking for real but got",
			 Object::repr (obj));
      t_real result = zobj->toreal ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a real value from an index (either from an integer)

  t_real Vector::getrint (const long index) const {
    rdlock ();
    try {
      Object*   obj = get (index);
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj != nullptr) return iobj->tolong ();
      Real* zobj = dynamic_cast <Real*> (obj);
      if (zobj == nullptr) {
	throw Exception ("type-error", "looking for real but got", 
			 Object::repr (obj));
      }
      t_real result = zobj->toreal ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a boolean value from an index

  bool Vector::getbool (const long index) const {
    rdlock ();
    try {
      Object*   obj = get (index);
      Boolean* bobj = dynamic_cast <Boolean*> (obj);
      if (bobj == nullptr) 
	throw Exception ("type-error", "looking for boolean but got",
			 Object::repr (obj));
      bool result = bobj->tobool ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a byte value from an index

  t_byte Vector::getbyte (const long index) const {
    rdlock ();
    try {
      Object* obj = get (index);
      // check for a byte
      Byte* bobj = dynamic_cast <Byte*> (obj);
      if (bobj != nullptr) {
	t_byte result =  bobj->tobyte ();
	unlock ();
	return result;
      }
      // check for a character
      Character* cobj = dynamic_cast <Character*> (obj);
      if (cobj != nullptr) {
	t_byte result =  (t_byte) cobj->tochar ();
	unlock ();
	return result;
      }
      // check for an integer
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj != nullptr) {
	t_long ival = iobj->tolong ();
	if ((ival >= 0) && (ival <= 255)) {
	  t_byte result = (t_byte) ival;
	  unlock ();
	  return result;
	} else {
	  throw Exception ("range-error", "out-of-range integer for byte");
	}
      }
      throw Exception ("type-error", "invalid object as byte representation",
		       Object::repr (obj));
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a word value from an index

  t_word Vector::getword (const long index) const {
    rdlock ();
    try {
      Object* obj = get (index);
      // check for a byte
      Byte* bobj = dynamic_cast <Byte*> (obj);
      if (bobj != nullptr) {
	t_word result =  (t_word) bobj->tobyte ();
	unlock ();
	return result;
      }
      // check for a character
      Character* cobj = dynamic_cast <Character*> (obj);
      if (cobj != nullptr) {
	t_word result =  (t_word) cobj->tochar ();
	unlock ();
	return result;
      }
      // check for an integer
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj != nullptr) {
	t_long ival = iobj->tolong ();
	if ((ival >= 0) && (ival <= 65535)) {
	  t_word result = (t_word) ival;
	  unlock ();
	  return result;
	} else {
	  throw Exception ("range-error", "out-of-range integer for word");
	}
      }
      throw Exception ("type-error", "invalid object as word representation",
		       Object::repr (obj));
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a character value from an index

  t_quad Vector::getchar (const long index) const {
    rdlock ();
    try {
      Object*     obj = get (index);
      Character* cobj = dynamic_cast <Character*> (obj);
      if (cobj == nullptr) 
	throw Exception ("type-error", "looking for character but got",
			 Object::repr (obj));
      t_quad result = cobj->toquad ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a string value from an index

  String Vector::getstring (const long index) const {
    rdlock ();
    try {
      Object*  obj = get (index);
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj == nullptr) 
	throw Exception ("type-error", "looking for string but got",
			 Object::repr (obj));
      String result = *sobj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a new vector iterator

  Iterator* Vector::makeit (void) {
    rdlock ();
    try {
      Iterator* result = new Vectorit (this);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an iterator at the beginning

  Vectorit Vector::begin (void) {
    rdlock ();
    try {
      Vectorit result = this;
      result.begin ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get an iterator at the end

  Vectorit Vector::end (void) {
    rdlock ();
    try {
      Vectorit result = this;
      result.end ();
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
  static const long QUARK_ZONE_LENGTH = 14;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_GET    = zone.intern ("get");
  static const long QUARK_SET    = zone.intern ("set");
  static const long QUARK_POP    = zone.intern ("pop");
  static const long QUARK_RML    = zone.intern ("pop-last");
  static const long QUARK_FIND   = zone.intern ("find");
  static const long QUARK_LAST   = zone.intern ("last");
  static const long QUARK_FIRST  = zone.intern ("first");
  static const long QUARK_RESET  = zone.intern ("reset");
  static const long QUARK_CLEAN  = zone.intern ("clean");
  static const long QUARK_LENGTH = zone.intern ("length");
  static const long QUARK_EXISTS = zone.intern ("exists-p");
  static const long QUARK_REMOVE = zone.intern ("remove");
  static const long QUARK_EMPTYP = zone.intern ("empty-p");

  // generate a vector of arguments

  Vector* Vector::eval (Evaluable* zobj, Nameset* nset, Cons* args) {
    long len = 0;
    if ((args == nullptr) || ((len = args->length ()) == 0)) return nullptr;
    Vector* result = new Vector (len);
  
    // loop in the cons cell and accumulate arguments
    try {
      while (args != nullptr) {
	Object* car = args->getcar ();
	if (car == nullptr) 
	  result->add ((Object*) nullptr);
	else
	  result->add (car->eval (zobj,nset));
	args = args->getcdr ();
      }
    } catch (...) {
      delete result;
      throw;
    }
    return result;
  }

  // create a new object in a generic way

  Object* Vector::mknew (Vector* argv) {
    if ((argv == nullptr) || (argv->length () == 0)) return new Vector;
    return new Vector (*argv);
  }

  // return true if the given quark is defined

  bool Vector::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Collectable::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? Iterable::isquark (quark, hflg) : false;
      }
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
  
  Object* Vector::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH) return new Integer (length ());
      if (quark == QUARK_EMPTYP) return new Boolean (empty  ());
      if (quark == QUARK_RESET) {
	reset  ();
	return nullptr;
      }
      if (quark == QUARK_FIRST) {
	rdlock ();
	try {
	  Object* result = first ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_LAST) {
	rdlock ();
	try {
	  Object* result = last ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_POP) {
	wrlock ();
	try {
	  Object* result = pop ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_RML) {
	wrlock ();
	try {
	  Object* result = rml ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GET) {
	rdlock ();
	try {
	  Object* result = get (argv->getlong (0));
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_ADD) {
	Object* result = argv->get (0);
	add (result);
	zobj->post (result);
	return result;
      }
      if (quark == QUARK_EXISTS) {
	Object* obj = argv->get (0);
	bool result = exists (obj);
	return new Boolean (result);
      }
      if (quark == QUARK_FIND) {
	Object* obj = argv->get (0);
	long index = find (obj);
	if (index == -1) return nullptr;
	return new Integer (index);
      }
      if (quark == QUARK_CLEAN) {
	long index = argv->getlong (0);
	remove (index);
	return nullptr;
      }
      if (quark == QUARK_REMOVE) {
	Object* obj = argv->get (0);
	remove (obj);
	return nullptr;
      }
    }

    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SET) {
	long     index = argv->getlong (0);
	Object* result = argv->get (1);
	set (index, result);
	zobj->post (result);
	return result;
      }
      if (quark == QUARK_ADD) {
	long     index = argv->getlong (0);
	Object* result = argv->get (1);
	add (index, result);
	zobj->post (result);
	return result;
      }
    }
    // check the collectable method
    if (Collectable::isquark (quark, true) == true) {
      return Collectable::apply (zobj, nset, quark, argv);
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

  // create a new vector iterator

  Vectorit::Vectorit (Vector* vobj) {
    Object::iref (p_vobj = vobj);
    begin ();
  }

  // copy construct this iterator

  Vectorit::Vectorit (const Vectorit& that) {
    that.rdlock ();
    try {
      Object::iref (p_vobj = that.p_vobj);
      d_vidx = that.d_vidx;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // destroy this vector iterator

  Vectorit::~Vectorit (void) {
    Object::dref (p_vobj);
  }

  // assign an iterator to this one

  Vectorit& Vectorit::operator = (const Vectorit& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Object::iref (that.p_vobj);
      Object::dref (p_vobj);
      p_vobj = that.p_vobj;
      d_vidx = that.d_vidx;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // compare two iterators

  bool Vectorit::operator == (const Vectorit& it) const {
    rdlock ();
    try {
      bool result = (p_vobj == it.p_vobj) && (d_vidx == it.d_vidx);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // compare two iterators

  bool Vectorit::operator != (const Vectorit& it) const {
    rdlock ();
    try {
      bool result = (p_vobj != it.p_vobj) || (d_vidx != it.d_vidx);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move the iterator to the next position

  Vectorit& Vectorit::operator ++ (void) {
    wrlock ();
    try {
      next ();
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // move the iterator to the previous position

  Vectorit& Vectorit::operator -- (void) {
    wrlock ();
    try {
      prev ();
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the iterator object

  Object* Vectorit::operator * (void) const {
    rdlock ();
    try {
      Object* result = getobj ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
	
  // return the class name

  String Vectorit::repr (void) const {
    return "Vectorit";
  }

  // reset the iterator to the begining

  void Vectorit::begin (void) {
    wrlock ();
    try {
      d_vidx = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset the iterator to the end

  void Vectorit::end (void) {
    wrlock ();
    if (p_vobj != nullptr) p_vobj->rdlock ();
    try {
      if (p_vobj != nullptr) {
	d_vidx = (p_vobj->d_vlen == 0) ? 0 : p_vobj->d_vlen;
      } else {
	d_vidx = 0;
      }
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
    } catch (...) {
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
      throw;
    }
  }

  // go to the next object

  void Vectorit::next (void) {
    wrlock ();
    if (p_vobj != nullptr) p_vobj->rdlock ();
    try {
      if (p_vobj != nullptr) {
	if (++d_vidx >= p_vobj->d_vlen) d_vidx = p_vobj->d_vlen;
      } else {
	d_vidx = 0;
      }
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
    } catch (...) {
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
      throw;
    }
  }

  // go to the previous object

  void Vectorit::prev (void) {
    wrlock ();
    try {
      if (--d_vidx < 0) d_vidx = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the object at the current position

  Object* Vectorit::getobj (void) const {
    rdlock ();
    if (p_vobj != nullptr) p_vobj->rdlock ();
    try {
      Object* result = nullptr;
      if ((p_vobj != nullptr) && (d_vidx < p_vobj->d_vlen)) {
	result = p_vobj->get (d_vidx);
      }
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
      return result;
    } catch (...) {
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
      throw;
    }
  }

  // return true if the iterator is at the end

  bool Vectorit::isend (void) const {
    rdlock ();
    if (p_vobj != nullptr) p_vobj->rdlock ();
    try {
      bool result = false;
      if (p_vobj != nullptr) {
	result = (d_vidx >= p_vobj->d_vlen);
      }
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
      return result;
    } catch (...) {
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
      throw;
    }
  }
}
