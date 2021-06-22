// ---------------------------------------------------------------------------
// - Cons.cpp                                                                -
// - standard object library - cons cell class implementation                -
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
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Boolean.hpp"
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

  // create a new cons cell initialized to nil

  Cons::Cons (void) {
    d_cctp = CCTP_NORM;
    p_car  = nullptr;
    p_cdr  = nullptr;
    p_mon  = nullptr;
  }

  // create a new cons cell with a type

  Cons::Cons (const t_cctp cctp) {
    d_cctp = cctp;
    p_car  = nullptr;
    p_cdr  = nullptr;
    p_mon  = nullptr;
  }

  // create a new cons cell with a car

  Cons::Cons (Object* car) {
    d_cctp = CCTP_NORM;
    p_car  = Object::iref (car);
    p_cdr  = nullptr;
    p_mon  = nullptr;
  }

  // create a new cons cell with a type and a car

  Cons::Cons (const t_cctp cctp, Object* car) {
    d_cctp = cctp;
    p_car  = Object::iref (car);
    p_cdr  = nullptr;
    p_mon  = nullptr;
  }

  // copy constructor for this cons cell

  Cons::Cons (const Cons& that) {
    that.rdlock ();
    try {
      d_cctp = that.d_cctp;
      Object::iref (p_car = that.p_car);
      Object::iref (p_cdr = that.p_cdr);
      p_mon  = (that.p_mon == nullptr) ? nullptr : new Monitor;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // copy move this cons cell

  Cons::Cons (Cons&& that) noexcept {
    that.wrlock ();
    try {
      // assign base collectable
      Collectable::operator = (static_cast<Collectable&&>(that));
      // copy move locally
      d_cctp = that.d_cctp; that.d_cctp = CCTP_NORM;
      p_car  = that.p_car;  that.p_car  = nullptr;
      p_cdr  = that.p_cdr;  that.p_cdr  = nullptr;
      p_mon  = that.p_mon;  that.p_mon  = nullptr;
    } catch (...) {
      d_cctp = CCTP_NORM;
      p_car  = nullptr;
      p_cdr  = nullptr;
      p_mon  = nullptr;
    } 
    that.unlock ();
  }
  
  // destroy this cons cell

  Cons::~Cons (void) {
    delete p_mon;
    Object::dref (p_car);
    Object::dref (p_cdr);
  }

  // assign a cons cell to this one

  Cons& Cons::operator = (const Cons& that) {
    // check for self assignation
    if (this == &that) return *this;
    wrlock ();
    that.rdlock ();
    try {
      // protect again same assignation
      Object::iref (that.p_car);
      Object::iref (that.p_cdr);
      Object::dref (p_car);
      Object::dref (p_cdr);
      // assign cell
      d_cctp = that.d_cctp;
      p_car  = that.p_car;
      p_cdr  = that.p_cdr;
      p_mon  = (that.p_mon == nullptr) ? nullptr : new Monitor;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // move a cons cell to this one

  Cons& Cons::operator = (Cons&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      // clean locally
      delete p_mon; p_mon = nullptr;
      Object::dref (p_car);
      Object::dref (p_cdr);
      // assign base collectable
      Collectable::operator = (static_cast<Collectable&&>(that));
      // move locally
      d_cctp = that.d_cctp; that.d_cctp = CCTP_NORM;
      p_car  = that.p_car;  that.p_car  = nullptr;
      p_cdr  = that.p_cdr;  that.p_cdr  = nullptr;
      p_mon  = that.p_mon;  that.p_mon  = nullptr;
    } catch (...) {
      d_cctp = CCTP_NORM;
      p_car  = nullptr;
      p_cdr  = nullptr;
      p_mon  = nullptr;
    }
    unlock ();
    that.unlock ();
    return *this;
  }
  
  // return the class name

  String Cons::repr (void) const {
    return "Cons";
  }

  // return a clone of this object

  Object* Cons::clone (void) const {
    return new Cons (*this);
  }

  // return the cons cell did

  t_word Cons::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the cons cell sid

  t_word Cons::getsid (void) const {
    return SRL_CONS_SID;
  }

  // serialize this cons cell

  void Cons::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the cons cell type
      Serial::wrbyte (d_cctp, os);
      // try to serialize the car
      if (p_car == nullptr) {
	Serial::wrnilid (os);
      } else {
	Serial* sobj = dynamic_cast <Serial*> (p_car);
	if (sobj == nullptr) {
	  throw Exception ("serial-error", "cannot serialize object", 
			   p_car->repr ());
	}
	sobj->serialize (os);
      }
      // try to serialize the cdr
      if (p_cdr == nullptr) {
	Serial::wrnilid (os);
      } else {
	Serial* sobj = dynamic_cast <Serial*> (p_cdr);
	if (sobj == nullptr) {
	  throw Exception ("serial-error", "cannot serialize object", 
			   p_car->repr ());
	}
	sobj->serialize (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this cons cell

  void Cons::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the type
      d_cctp = static_cast<t_cctp>(Serial::rdbyte(is));
      // deserialize the car
      setcar (Serial::deserialize (is));
      // deserialize the cdr
      Object* obj = Serial::deserialize (is);
      if (obj == nullptr) {
	setcdr ((Cons*) nullptr);
      } else {
	Cons* cdr = dynamic_cast <Cons*> (obj);
	if (cdr == nullptr) {
	  throw Exception ("deserialize-error", "non cons cell to deserialize",
			   obj->repr ());
	}
	setcdr (cdr);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // release this cons cell

  void Cons::release (void) {
    wrlock ();
    try {
      // protect us
      Object::iref (this);
      // release the cdr
      Cons* cell = p_cdr;
      p_cdr = nullptr;
      if (cell != nullptr) cell->release ();
      Object::dref (cell);
      // release and unlock
      Object::tref (this);
      unlock ();
    } catch (...) {
      Object::tref (this);
      unlock ();
      throw;
    }
  }

  // add an object to the last cdr of this cons cell

  void Cons::add (Object* object) {
    wrlock ();
    try {
      // create a new cons cell 
      Cons* cons = new Cons (object);
      // find the last cons cell
      Cons* last = this;
      while (last->p_cdr != nullptr) last = last->p_cdr;
      // attach this new cons cell
      last->p_cdr = cons;
      Object::iref (cons);
      // unlock the cons cell
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the car of this cons cell

  void Cons::setcar (Object* object) {
    wrlock ();
    try {
      // set the car
      Object::dref (p_car);
      p_car = Object::iref (object);
      // unlock the cons cell
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cdr of this cons cell

  void Cons::setcdr (Cons* cdr) {
    wrlock ();
    try {
      // set the cdr
      Object::dref (p_cdr);
      p_cdr = cdr; Object::iref (cdr);
      // unlock the cons cell
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the car of this cons cell

  Object* Cons::getcar (void) const {
    rdlock ();
    try {
      Object* result = p_car;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the cdr of this cons cell

  Cons* Cons::getcdr (void) const {
    rdlock ();
    try {
      Cons* result = p_cdr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }


  // return the car of the cdr of this cons cell

  Object* Cons::getcadr (void) const {
    rdlock ();
    try {
      if (p_cdr == nullptr) {
	unlock ();
	return nullptr;
      }
      Object* result = p_cdr->p_car;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the car of the cdr of the cdr of this cons cell

  Object* Cons::getcaddr (void) const {
    rdlock ();
    try {
      if (p_cdr == nullptr) {
	unlock ();
	return nullptr;
      }
      Cons* cdr = p_cdr->p_cdr;
      if (cdr == nullptr) {
	unlock ();
	return nullptr;
      }
      Object* result = cdr->p_car;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the car of the cdr of the cdr of the cdr of this cons cell

  Object* Cons::getcadddr (void) const {
    rdlock ();
    try {
      if (p_cdr == nullptr) {
	unlock ();
	return nullptr;
      }
      Cons* cdr = p_cdr->p_cdr;
      if (cdr == nullptr) {
	unlock ();
	return nullptr;
      }
      cdr = cdr->p_cdr;
      if (cdr == nullptr) {
	unlock ();
	return nullptr;
      }
      Object* result = cdr->p_car;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the car of the cdr of the cdr of the cdr of this cons cell

  Object* Cons::getcaddddr (void) const {
    rdlock ();
    try {
      if (p_cdr == nullptr) {
	unlock ();
	return nullptr;
      }
      Cons* cdr = p_cdr->p_cdr;
      if (cdr == nullptr) {
	unlock ();
	return nullptr;
      }
      cdr = cdr->p_cdr;
      if (cdr == nullptr) {
	unlock ();
	return nullptr;
      }
      cdr = cdr->p_cdr;
      if (cdr == nullptr) {
	unlock ();
	return nullptr;
      }
      Object* result = cdr->p_car;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return true if the car is nil

  bool Cons::isnil (void) const {
    rdlock ();
    try {
      bool result = (p_car == nullptr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the cons cell is a block cell

  bool Cons::isblock (void) const {
    rdlock ();
    try {
      bool result = (d_cctp == CCTP_BLOK);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the length of this cons cell

  long Cons::length (void) const {
    rdlock ();
    try {
      long result      = 0;
      const Cons* cons = this;
      do {
	result++;
      } while ((cons->p_cdr != this) && ((cons = cons->p_cdr) != nullptr));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return an object by index

  Object* Cons::get (const long index) const {
    rdlock ();
    try {
      long count       = 0;
      const Cons* cons = this;
      if (index < 0) throw Exception ("index-error",
				      "invalid negative index in cons get");
      // loop in the cons cell list
      while (cons != nullptr) {
	if (count == index) {
	  Object* result = cons->p_car;
	  unlock ();
	  return result;
	}
	count++;
	cons = cons->p_cdr;
      }
      throw Exception ("index-error", "invalid index in cons get method");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the form synchronizer
  
  void Cons::mksync (void) {
    wrlock ();
    try {
      if (p_mon == nullptr) p_mon = new Monitor;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a cons iterator

  Iterator* Cons::makeit (void) {
    rdlock ();
    try {
      Iterator* result = new Consit (this);
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
  static const long QUARK_ZONE_LENGTH = 12;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD       = zone.intern ("add");
  static const long QUARK_GET       = zone.intern ("get");
  static const long QUARK_NILP      = zone.intern ("nil-p");
  static const long QUARK_BLOCKP    = zone.intern ("block-p");
  static const long QUARK_LENGTH    = zone.intern ("length");
  static const long QUARK_GETCAR    = zone.intern ("get-car");
  static const long QUARK_SETCAR    = zone.intern ("set-car");
  static const long QUARK_GETCDR    = zone.intern ("get-cdr");
  static const long QUARK_SETCDR    = zone.intern ("set-cdr");
  static const long QUARK_GETCADR   = zone.intern ("get-cadr");
  static const long QUARK_GETCADDR  = zone.intern ("get-caddr");
  static const long QUARK_GETCADDDR = zone.intern ("get-cadddr");

  // evaluate each car of a cons cell and return a new form

  Cons* Cons::eval (Evaluable* zobj, Nameset* nset, Cons* args) {
    // check for nil and lock
    if (args == nullptr) return nullptr;
    args->rdlock ();
    // create the result cell and loop
    Cons* cons = args;
    Cons* form = nullptr;
    try {
      while (cons != nullptr) {
	Object* car = cons->getcar ();
	Object* obj = (car == nullptr) ? nullptr : car->eval (zobj, nset);
	if (form == nullptr) {
	  form = new Cons (obj);
	} else {
	  form->add (obj);
	}
	cons = cons->getcdr ();
      }
      // unlock and return
      args->unlock ();
      return form;
    } catch (...) {
      delete form;
      args->unlock ();
      throw;
    }
  }

  // evaluate each car of a cons cell - if the object is not a cons cell
  // the object is returned

  Object* Cons::mkobj (Evaluable* zobj, Nameset* nset, Object* object) {
    // try to get a cons cell
    Cons* cons = dynamic_cast <Cons*> (object);
    if (cons == nullptr) return object;
    // create an evaluated form
    return Cons::eval (zobj, nset, cons);
  }

  // create a new object in a generic way

  Object* Cons::mknew (Vector* argv) {
    long len = 0;
    if ((argv == nullptr) || ((len = argv->length ()) == 0)) return nullptr;
    // build the cons cell
    Cons* result = nullptr;
    for (long i = 0; i < len; i++) {
      if (result == nullptr)
	result = new Cons (argv->get (i));
      else
	result->add (argv->get (i));
    }
    return result;
  }

  // return true if the given quark is defined

  bool Cons::isquark (const long quark, const bool hflg) const {
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

  // set an object to the car of this cons cell

  Object* Cons::vdef (Evaluable* zobj, Nameset* nset, Object* object) {
    setcar (object);
    return object;
  }

  // evaluate this object in the current nameset

  Object* Cons::eval (Evaluable* zobj, Nameset* nset) {
    // synchronize the form
    if (p_mon != nullptr) p_mon->enter ();
    Object* result = nullptr;
    try {
      if (d_cctp == CCTP_BLOK) {
	Cons* cons = this;
	while (cons != nullptr) {
	  Object::cref (result);
	  Object* car = cons->getcar ();
	  result = (car == nullptr) ? nullptr : car->eval (zobj, nset);
	  cons   = cons->getcdr ();
	}
      } else {
	if (p_car == nullptr) {
	  if (p_mon != nullptr) p_mon->leave ();
	  return nullptr;
	}
	Object* func = Object::iref (p_car->eval (zobj, nset));
	if (func == nullptr) {
	  if (p_mon != nullptr) p_mon->leave ();
	  return nullptr;
	}
	try {
	  result = func->apply (zobj, nset, p_cdr);
	  Object::dref (func);
	} catch (...) {
	  Object::dref (func);
	  throw;
	}
      }
      if (p_mon != nullptr) p_mon->leave ();
      return result;
    } catch (...) {
      if (p_mon != nullptr) p_mon->leave ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Cons::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETCAR)   {
	rdlock ();
	try {
	  Object* result = getcar ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETCDR) {
	rdlock ();
	try {
	  Object* result = getcdr ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETCADR) {
	rdlock ();
	try {
	  Object* result = getcadr ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      } 
      if (quark == QUARK_GETCADDR) {
	rdlock ();
	try {
	  Object* result = getcaddr ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETCADDDR) {
	rdlock ();
	try {
	  Object* result = getcadddr ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_LENGTH) return new Integer (length ());
      if (quark == QUARK_NILP)   return new Boolean (isnil ());
      if (quark == QUARK_BLOCKP) return new Boolean (isblock ());
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETCAR) {
	Object* result = argv->get (0);
	setcar (result);
	zobj->post (result);
	return result;
      }

      if (quark == QUARK_SETCDR) {
	Object* result = argv->get (0);
	if (result == nullptr) {
	  setcdr ((Cons*) nullptr);
	  zobj->post (result);
	  return nullptr;
	}
	Cons* cdr = dynamic_cast <Cons*> (result);
	if (cdr == nullptr) 
	  throw Exception ("type-error", "invalid object with set-cdr method",
			   Object::repr (result));
	
	setcdr (cdr);
	zobj->post (cdr);
	return result;
      }

      if (quark == QUARK_ADD) {
	Object* result = argv->get (0);
	add (result);
	zobj->post (result);
	return result;
      }

      if (quark == QUARK_GET) {
	wrlock ();
	try {
	  long val = argv->getlong (0);
	  Object* result = get (val);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
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

  // create a new cons iterator

  Consit::Consit (Cons* cons) {
    Object::iref (p_cons = cons);
    p_cell = nullptr;
    begin ();
  }

  // destroy this cons iterator

  Consit::~Consit (void) {
    Object::dref (p_cell);
    Object::dref (p_cons);
  }

  // return the class name

  String Consit::repr (void) const {
    return "Consit";
  }

  // reset the iterator to the begining

  void Consit::begin (void) {
    wrlock ();
    if (p_cons != nullptr) p_cons->rdlock ();
    try {
      Object::dref (p_cell);
      Object::iref (p_cell = p_cons);
      if (p_cons != nullptr) p_cons->unlock ();
      unlock ();
    } catch (...) {
      if (p_cons != nullptr) p_cons->unlock ();
      unlock ();
      throw;
    }
  }

  // reset the iterator to the end

  void Consit::end (void) {
    throw Exception ("iterator-error", "cannot set a cons iterator to end");
  }

  // go to the next object

  void Consit::next (void) {
    wrlock ();
    try {
      Cons* cdr = p_cell->p_cdr;
      Object::iref (cdr);
      Object::dref (p_cell);
      p_cell = cdr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // go to the previous object

  void Consit::prev (void) {
    throw Exception ("iterator-error", "cannot move back a cons iterator");
  }

  // get the object at the current position

  Object* Consit::getobj (void) const {
    rdlock ();
    try {
      Object* result = (p_cell == nullptr) ? nullptr : p_cell->p_car;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the iterator is at the end

  bool Consit::isend (void) const {
    rdlock ();
    try {
      bool result = (p_cell == nullptr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
