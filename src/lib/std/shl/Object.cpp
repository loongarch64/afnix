// ---------------------------------------------------------------------------
// - Object.cpp                                                              -
// - standard object library - base object implementation                    -
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
#include "Vector.hpp"
#include "Lockrw.hpp"
#include "Monitor.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "cmem.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - memory allocation section                                             -
  // -------------------------------------------------------------------------

  // the object memory allocator

  void* Object::operator new (const t_size size) {
    return c_galloc (size);
  }

  void* Object::operator new [] (const t_size size) {
    return c_galloc (size);
  }

  // the object memory deallocator

  void Object::operator delete (void* handle) {
    c_gfree (handle);
  }

  void Object::operator delete [] (void* handle) {
    c_gfree (handle);
  }

  // enable/disable the dynamic memory debug

  void Object::setmdbg (const bool flag) {
    c_setmdbg (flag);
  }

  // set a memory label

  void Object::setmlbl (const String& label) {
    // get a character representation
    char* cptr = label.tochar ();
    // set the memory label
    c_setmlbl (cptr);
    // clean the label
    delete [] cptr;
  }

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the access control structure
  struct s_oacs {
    // the reference count
    long    d_rcnt;
    // the reference count monitor
    Monitor d_rmon;
    // constructor
    s_oacs (void) {
      d_rcnt = 0L;
    }
  };

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // increment the object reference count

  Object* Object::iref (Object* object) {
    // check for nil first
    s_oacs* axs = (object == nullptr) ? nullptr : object->p_oacs;
    if (axs == nullptr) return object;
    // lock and reference
    axs->d_rmon.enter ();
    axs->d_rcnt++;
    axs->d_rmon.leave ();
    return object;
  }
    
  // decrement the reference count and destroy the object if null

  void Object::dref (Object* object) {
    // check for nil first
    s_oacs* axs = (object == nullptr) ? nullptr : object->p_oacs;
    if (axs == nullptr) return;
    // lock and reference
    axs->d_rmon.enter ();
    if (--axs->d_rcnt <= 0) {
      axs->d_rmon.leave ();
      delete object;
      return;
    }
    axs->d_rmon.leave ();
  }
  
  // clean this object if the reference count is null
  
  void Object::cref (Object* object) {
    // check for nil first
    s_oacs* axs = (object == nullptr) ? nullptr : object->p_oacs;
    if (axs == nullptr) return;
    // lock and reference
    axs->d_rmon.enter ();
    if (axs->d_rcnt <= 0) {
      axs->d_rmon.leave ();
      delete object;
      return;
    }
    axs->d_rmon.leave ();
  }
  
  // decrement the object reference count but do not destroy if null

  void Object::tref (Object* object) {
    // check for nil first
    s_oacs* axs = (object == nullptr) ? nullptr : object->p_oacs;
    if (axs == nullptr) return;
    // lock and reference
    axs->d_rmon.enter ();
    if (axs->d_rcnt > 0) axs->d_rcnt--;
    axs->d_rmon.leave ();
  }

  // return true if the object has reference equal to 0 or 1

  bool Object::uref (Object* object) {
    // check for nil first
    s_oacs* axs = (object == nullptr) ? nullptr : object->p_oacs;
    if (axs == nullptr) return true;
    // lock and reference
    axs->d_rmon.enter ();
    bool result = (axs->d_rcnt <= 1);
    axs->d_rmon.leave ();
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new object with a 0 reference count

  Object::Object (void) {
    p_oacs = c_isbstk (this) ? nullptr : new s_oacs;
    p_lock = new Lockrw;
  }

  // copy move this object

  Object::Object (Object&& that) noexcept {
    that.wrlock ();
    try {
      p_oacs = c_isbstk (this) ? nullptr : new s_oacs;
      p_lock = new Lockrw;
      that.unlock ();
    } catch (...) {
      that.unlock ();
    } 
  }
  
  // destroy this object

  Object::~Object (void) {
    delete p_oacs;
    delete p_lock;
  }

  // move an object into this one

  Object& Object::operator = (Object&& that) noexcept {
    wrlock ();
    that.wrlock ();
    try {
      unlock ();
      that.unlock ();
    } catch (...) {
      unlock ();
      that.unlock ();
    }      
    return *this;
  }
  
  // return an object class name

  const String Object::repr (Object* object) {
    if (object == nullptr) return "nil";
    return object->repr ();
  }

  // return a clone of this object

  Object* Object::clone (void) const {
    throw Exception ("clone-error", "cannot clone object", repr ());
  }

  // get a read lock for this object

  void Object::rdlock (void) const {
    if (p_lock == nullptr) return;
    p_lock->rdlock ();
  }

  // get a write lock for this object

  void Object::wrlock (void) const {
    if (p_lock == nullptr) return;
    p_lock->wrlock ();
  }

  // get an adaptative read lock for this object

  void Object::arlock (void) const {
    if (p_lock == nullptr) return;
    p_lock->arlock ();
  }

  // unlock a previous lock

  void Object::unlock (void) const {
    if (p_lock == nullptr) return;
    p_lock->unlock ();
  }

  // reduce this object

  Object* Object::reduce (void) const {
    return const_cast <Object*> (this);
  }

  // -------------------------------------------------------------------------
  // - quark section                                                         -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 6;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_VDEF   = zone.intern ("=");
  static const long QUARK_REPR   = zone.intern ("repr");
  static const long QUARK_CLONE  = zone.intern ("clone");
  static const long QUARK_RDLOCK = zone.intern ("rdlock");
  static const long QUARK_WRLOCK = zone.intern ("wrlock");
  static const long QUARK_UNLOCK = zone.intern ("unlock");

  // return true if the given quark is defined

  bool Object::isquark (const long quark, const bool hflg) const {
    rdlock ();
    bool result = zone.exists (quark);
    unlock ();
    return result;
  }
  
  // operate this object with another one

  Object* Object::oper (t_oper type, Object* object) {
    throw Exception ("operator-error", "invalid operator call for object",
		     repr ());
  }

  // create or set a const object to this object

  Object* Object::cdef (Evaluable* zobj, Nameset* nset, Object* object) {
    throw Exception ("const-error", "invalid const define with object",
		     repr ());
  }

  // create or set a const object to this object by quark

  Object* Object::cdef (Evaluable* zobj, Nameset* nset, const long quark,
			Object* object) {
    String mesg = "invalid const define with name ";
    mesg = mesg + String::qmap (quark);
    mesg = mesg + " from object type";
    throw Exception ("const-error", mesg, repr ());
  }

  // create or set an object to this object

  Object* Object::vdef (Evaluable* zobj, Nameset* nset, Object* object) {
    throw Exception ("trans-error", "invalid trans define with object",
		     repr ());
  }

  // create or set an object to this object by quark

  Object* Object::vdef (Evaluable* zobj, Nameset* nset, const long quark,
			Object* object) {
    String mesg = "invalid trans define with name ";
    mesg = mesg + String::qmap (quark);
    mesg = mesg + " from object type";
    throw Exception ("trans-error", mesg, repr ());
  }

  // unreference an object

  Object* Object::udef (Evaluable* zobj, Nameset* nset) {
    throw Exception ("unref-error", "invalid unreference with object",
		     repr ());
  }
  
  // unreference an object by quark

  Object* Object::udef (Evaluable* zobj, Nameset* nset, const long quark) {
    String mesg = "invalid object unref by name ";
    mesg = mesg + String::qmap (quark);
    mesg = mesg + " from object type";
    throw Exception ("const-error", mesg, repr ());
  }


  // evaluate an object in the current nameset

  Object* Object::eval (Evaluable* zobj, Nameset* nset) {
    return this;
  }

  // evaluate an object in the current nameset by quark

  Object* Object::eval (Evaluable* zobj, Nameset* nset, const long quark) {
    return new Combo (quark, this);
  }

  // apply an object with a set of arguments

  Object* Object::apply (Evaluable* zobj, Nameset* nset, Cons* args) {
    throw Exception ("apply-error", "invalid call to apply method with object",
		     repr ());
  }

  // apply an object by quark with a set of arguments

  Object* Object::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Cons* args) {
    Vector* argv   = Vector::eval (zobj, nset, args);
    Object* result = nullptr;
    try {
      result = apply (zobj, nset, quark, argv);
    } catch (...) {
      delete argv;
      throw;
    }
    delete argv;
    return result;
  }

  // apply an object by object with a set of arguments

  Object* Object::apply (Evaluable* zobj, Nameset* nset, Object* object,
			 Cons* args) {
    String mesg = "invalid call to apply with object ";
    mesg = mesg + Object::repr (object);
    mesg = mesg + " from object type";
    throw Exception ("apply-error", mesg, repr ());
  }

  // apply an object by object with a vector of arguments

  Object* Object::apply (Evaluable* zobj, Nameset* nset, Object* object,
			 Vector* argv) {
    String mesg = "invalid call to apply with object ";
    mesg = mesg + Object::repr (object);
    mesg = mesg + " from object type";
    throw Exception ("apply-error", mesg, repr ());
  }

  // apply an object by name with a vector of arguments

  Object* Object::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 arguments
    if (argc == 0) {
      if (quark == QUARK_REPR) return new String  (repr ());
      if (quark == QUARK_RDLOCK) {
	rdlock ();
	zobj->post (this);
	return this;
      }
      if (quark == QUARK_WRLOCK) {
	wrlock ();
	zobj->post (this);
	return this;
      }
      if (quark == QUARK_UNLOCK) {
	unlock ();
	zobj->post (this);
	return this;
      }
      if (quark == QUARK_CLONE) {
	Object* result = clone ();
	zobj->post (result);
	return result;
      }
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_VDEF) {
	Object* obj = argv->get (0);
	return vdef (zobj, nset, obj);
      }
    }

    // no way - error
    String mesg = "invalid call to apply with name ";
    mesg = mesg + String::qmap (quark);
    mesg = mesg + " from object type";
    throw Exception ("apply-error", mesg, repr ());
  }

  // -------------------------------------------------------------------------
  // - transient section                                                     -
  // -------------------------------------------------------------------------

  // create a null transient
  
  Transient::Transient (void) {
    p_tobj = nullptr;
  }

  // create a transient object
  
  Transient::Transient (Object* tobj) {
    Object::iref (p_tobj = tobj);
  }

  // copy construct this transient
  
  Transient::Transient (const Transient& that) {
    Object::iref (p_tobj = that.p_tobj);
  }

  // copy move this transient
  
  Transient::Transient (Transient&& that) noexcept {
    p_tobj = that.p_tobj; that.p_tobj = nullptr;
  }

  // destroy this transient

  Transient::~Transient (void) {
    Object::dref (p_tobj);
  }

  // assign a transient to this one
  
  Transient& Transient::operator = (const Transient& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // assign locally
    Object::iref (that.p_tobj); Object::dref (p_tobj); p_tobj = that.p_tobj;
    return *this;
  }

  // move a transient to this one
  
  Transient& Transient::operator = (Transient&& that) noexcept {
    // check for self-assignation
    if (this == &that) return *this;
    // move locally
    p_tobj = that.p_tobj; that.p_tobj = nullptr;
    return *this;
  }

  // get the transient object

  Object* Transient::operator * (void) {
    return p_tobj;
  }

  // get the transient object

  const Object* Transient::operator * (void) const {
    return p_tobj;
  }
}
