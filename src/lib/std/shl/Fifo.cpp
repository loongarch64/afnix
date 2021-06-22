// ---------------------------------------------------------------------------
// - Fifo.cpp                                                                -
// - standard object library - fifo class implementation                     -
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

#include "Fifo.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  static const long DEF_FIFO_SIZE = 64;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a new fifo
  
  Fifo::Fifo (void) {
    d_size = DEF_FIFO_SIZE;
    p_fifo = new Object*[d_size];
    for (long i = 0; i < d_size; i++) p_fifo[i] = nullptr;
  }
  
  // create a new fifo by size
  
  Fifo::Fifo (const long size) {
    d_size = (size > 0) ? size : DEF_FIFO_SIZE;
    p_fifo = new Object*[d_size];
    for (long i = 0; i < d_size; i++) p_fifo[i] = nullptr;
  }

  // copy construct this fifo
  
  Fifo::Fifo (const Fifo& that) {
    that.rdlock ();
    try {
      d_size = that.d_size;
      p_fifo = new Object*[d_size];
      for (long i = 0; i < d_size; i++) {
	Object::iref (p_fifo[i] = that.p_fifo[i]);
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this fifo

  Fifo::~Fifo (void) {
    for (long i = 0; i < d_size; i++) Object::dref (p_fifo[i]);
    delete [] p_fifo;
  }

  // return the class name

  String Fifo::repr (void) const {
    return "Fifo";
  }

  // assign a fifo to this one

  Fifo& Fifo::operator = (const Fifo& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // remove the old fifo
      for (long i = 0; i < d_size; i++) Object::dref (p_fifo[i]);
      delete [] p_fifo; p_fifo = nullptr;
      // create a new fifo
      d_size = that.d_size;
      p_fifo = new Object*[d_size];
      for (long i = 0; i < d_size; i++) {
	Object::iref (p_fifo[i] = that.p_fifo[i]);
      }
      // unlock and return
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this fifo class

  void Fifo::reset (void) {
    wrlock ();
    try {
      for (long i = 0; i < d_size; i++) {
	Object::dref (p_fifo[i]);
	p_fifo[i] = nullptr;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the fifo size

  long Fifo::getsize (void) const {
    rdlock ();
    try {
      long result = d_size;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the fifo length

  long Fifo::length (void) const {
    rdlock ();
    try {
      long result = 0;
      for (long i = 0; i < d_size; i++) {
	if (p_fifo[i] == nullptr) break;
	result++;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the fifo is empty

  bool Fifo::empty (void) const {
    rdlock ();
    try {
      bool result = (p_fifo[0] == nullptr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the fifo is full

  bool Fifo::full (void) const {
    rdlock ();
    try {
      bool result = (p_fifo[d_size-1] != nullptr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // push an object into the fifo

  void Fifo::push (Object* obj) {
    // do nothing if nil
    if (obj == nullptr) return;
    // lock and push
    wrlock ();
    try {
      // clean the last element
      Object::dref (p_fifo[d_size-1]);
      // shift by one position
      for (long i = d_size-1; i > 0; i--) p_fifo[i] = p_fifo[i-1];
      // push the object
      Object::iref (p_fifo[0] = obj);
      // unlock and return 
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pop an object from the fifo

  Object* Fifo::pop (void) {
    wrlock ();
    try {
      // save the object result
      Object* result = p_fifo[0];
      // shift the object
      for (long i = 1; i < d_size; i++) p_fifo[i-1] = p_fifo[i];
      // clean the last position
      p_fifo[d_size-1] = nullptr;
      // unlock and return
      Object::tref (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a fifo object by index

  Object* Fifo::get (const long index) const {
    rdlock ();
    try {
      // check that we are bounded
      if ((index < 0) || (index >= d_size)) {
	throw Exception ("index-error","fifo index is out of range");
      }
      // get the object and unlock
      Object* result = p_fifo[index];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resize the fifo

  void Fifo::resize (const long size) {
    wrlock ();
    try {
      // do nothing with same size
      if (size == d_size) {
	unlock ();
	return;
      }
      // check the new size
      if (size <= 0) {
	throw Exception ("fifo-error","invalid size during resize operation");
      }
      // create a new fifo
      Object** fifo = new Object*[size];
      for (long i = 0; i < size; i++) fifo[i] = nullptr;
      // check for reduction
      if (size < d_size) {
	for (long i = 0; i < size; i++) fifo[i] = p_fifo[i];
	for (long i = size; i < d_size; i++) Object::dref (p_fifo[i]);
      }
      // check for expansion
      if (d_size < size) {
	for (long i = 0; i < d_size; i++) fifo[i] = p_fifo[i];
      }
      // clean the old fifo arry
      delete [] p_fifo;
      // update with new fifo
      d_size = size;
      p_fifo = fifo;
      // unlock and return
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
  static const long QUARK_ZONE_LENGTH = 9;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_POP     = zone.intern ("pop");
  static const long QUARK_PUSH    = zone.intern ("push");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_FULLP   = zone.intern ("full-p");
  static const long QUARK_EMPTYP  = zone.intern ("empty-p");
  static const long QUARK_RESIZE  = zone.intern ("resize");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_GETSIZE = zone.intern ("get-size");
  
  // create a new object in a generic way

  Object* Fifo::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check 0 argument
    if (argc == 0) return new Fifo;
    // check 1 argument
    if (argc == 1) {
      long size = argv->getlong (0);
      return new Fifo (size);
    }
    throw Exception ("argument-error", "too many argument for fifo");
  }

  // return true if the given quark is defined
  
  bool Fifo::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object class with a set of arguments and a quark
  
  Object* Fifo::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_FULLP)   return new Boolean (full    ());
      if (quark == QUARK_EMPTYP)  return new Boolean (empty   ());
      if (quark == QUARK_LENGTH)  return new Integer (length  ());
      if (quark == QUARK_GETSIZE) return new Integer (getsize ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
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
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_PUSH) {
	Object* obj = argv->get (0);
	push (obj);
	return nullptr;
      }
      if (quark == QUARK_GET) {
	long index = argv->getlong (0);
	rdlock ();
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
      if (quark == QUARK_RESIZE) {
	long size = argv->getlong (0);
	resize (size);
	return nullptr;
      }
    }
    // apply these arguments with the object
    return Object::apply (zobj, nset, quark, argv);
  }
}
