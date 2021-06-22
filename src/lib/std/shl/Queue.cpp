// ---------------------------------------------------------------------------
// - Queue.cpp                                                               -
// - standard object library - dynamic queue class implementation            -
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

#include "Queue.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  static const long DEF_QUEUE_SIZE = 64;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a default queue

  Queue::Queue (void) {
    d_size  = DEF_QUEUE_SIZE;
    p_queue = new Object*[d_size];
    d_qidx  = 0;
    d_didx  = 0;
  }

  // create a queue with a predefined size

  Queue::Queue (const long size) {
    d_size  = (size <= 0) ? DEF_QUEUE_SIZE : size;
    p_queue = new Object*[d_size];
    d_qidx  = 0;
    d_didx  = 0;
  }

  // create a queue with a vector of objects

  Queue::Queue (const Vector& vobj) {
    long size = vobj.length ();
    d_size  = (size == 0) ? 64 : size;
    p_queue = new Object*[d_size];
    d_qidx  = 0;
    d_didx  = 0;
    for (long i = 0; i < size; i++) enqueue (vobj.get (i));
  }

  // destroy this queue

  Queue::~Queue (void) {
    for (long i = d_didx; i < d_qidx; i++)
      Object::dref (p_queue[i]);
    delete [] p_queue;
  }

  // return the class name

  String Queue::repr (void) const {
    return "Queue";
  }

  // return the number of elements in this queue

  long Queue::length (void) const {
    rdlock ();
    try {
      long result = d_qidx - d_didx;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the queue is empty

  bool Queue::empty (void) const {
    rdlock ();
    try {
      bool result = (d_didx == d_qidx);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // flush this queue

  void Queue::flush (void) {
    wrlock ();
    try {
      while (empty () == false) {
	Object::dref (dequeue ());
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }

  }
      
  // get an object by index in this queue

  Object* Queue::get (const long index) const {
    rdlock ();
    try {
      long i = index + d_didx;
      if (i >= d_qidx) {
	throw Exception ("bound-error", "out of bound queue get index");
      }
      Object* result = p_queue[i];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // queue an object into this queue

  void Queue::enqueue (Object* object) {
    wrlock ();
    try {
      // check first the queue index
      if (d_qidx + 1 >= d_size) resize ();
      // queue the object and unlock
      p_queue[d_qidx++] = Object::iref (object);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // dequeue an object

  Object* Queue::dequeue (void) {
    wrlock ();
    try {
      // any object here
      if (d_didx == d_qidx) {
	unlock ();
	return nullptr;
      }
      // get the result
      Object* result = p_queue[d_didx++];
      // check if we reset the queue
      if (d_didx == d_qidx) {
	d_qidx = 0;
	d_didx = 0;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resize this queue - if some objects have been dequeued we shift
  // the object and reset the indexes - if no room is available we resize
  // the array

  void Queue::resize (void) {
    wrlock ();
    try {
      if (d_didx == 0) {
	long size = d_size * 2;
	Object** array = new Object*[size];
	for (long i = 0; i < d_size; i++)
	  array[i] = p_queue[i];
	delete [] p_queue;
	d_size  = size;
	p_queue = array;
      } else {
	long shift = d_didx;
	for (long i = d_didx; i < d_qidx; i++)
	  p_queue[i - shift] = p_queue[i];
	d_didx = 0;
	d_qidx -= shift;
      }
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 6;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

 // the object supported quarks
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_FLUSH   = zone.intern ("flush");
  static const long QUARK_EMPTY   = zone.intern ("empty-p");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_ENQUEUE = zone.intern ("enqueue");
  static const long QUARK_DEQUEUE = zone.intern ("dequeue");

  // create a new object queue in a generic way

  Object* Queue::mknew (Vector* argv) {
    if ((argv == nullptr) || (argv->length () == 0)) return new Queue;
    return new Queue (*argv);
  }

  // return true if the given quark is defined

  bool Queue::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply an object with a set of arguments and a quark

  Object* Queue::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_EMPTY)   return new Boolean (empty ());
      if (quark == QUARK_LENGTH)  return new Integer (length ());
      if (quark == QUARK_DEQUEUE) {
	wrlock ();
	Object* result = dequeue ();
	zobj->post   (result);
	Object::tref (result);
	unlock ();
	return result;
      }
      if (quark == QUARK_FLUSH) {
	flush ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ENQUEUE) {
	Object* result = argv->get (0);
	enqueue (result);
	zobj->post (result);
	return result;
      }      
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
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
