// ---------------------------------------------------------------------------
// - Heap.cpp                                                                -
// - standard object library - heap class implementation                     -
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

#include "Heap.hpp"
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

  // the heap node
  struct s_heap {
    // the node key
    t_long d_key;
    // the node object
    Object* p_obj;
    // create a default node
    s_heap (void) {
      d_key = 0;
      p_obj = nullptr;
    }
    // destroy a heap element
    ~s_heap (void) {
      Object::dref (p_obj);
    }
    // copy construct a heap node
    s_heap (const s_heap& that) {
      d_key= that.d_key;
      Object::iref (p_obj = that.p_obj);
    }
    // assign a heap node to this one
    s_heap& operator = (const s_heap& that) {
      if (this == &that) return *this;
      Object::iref (that.p_obj);
      Object::dref (p_obj);
      d_key = that.d_key;
      p_obj = that.p_obj;
      return *this;
    }
    // reset this heap node
    void reset (void) {
      d_key = 0;
      Object::dref (p_obj);
      p_obj = nullptr;
    }
    // set the node by key and object
    void set (const t_long key, Object* obj) {
      Object::iref (obj);
      Object::dref (p_obj);
      d_key = key;
      p_obj = obj;
    }
  };

  // compare two heap node
  static inline bool cmp_node (const bool mode, 
			       const s_heap& x, const s_heap& y) {
    return mode ? x.d_key <= y.d_key : x.d_key >= y.d_key;
  }

  // get the parent index in the btree
  static inline long bt_pidx (const long index) {
    if (index == 0) return 0;
    return (index - 1) >> 1;
  }
  // get the left index in the btree
  static inline long bt_lidx (const long index) {
    return (index << 1) + 1;
  }
  // get the right index in the btree
  static inline long bt_ridx (const long index) {
    return (index + 1) << 1;
  }

  // heap initial size
  static const long HEAP_SIZE = 256;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a new heap
  
  Heap::Heap (void) {
    d_size = HEAP_SIZE;
    p_heap = new s_heap[d_size];
    d_mode = true;
    d_minf = false;
    d_mink = 0LL;
    d_maxf = false;
    d_maxk = 0LL;
    reset ();
  }
  
  // create a new heap by size
  
  Heap::Heap (const long size) {
    d_size = (size > 0) ? size : HEAP_SIZE;
    p_heap = new s_heap[d_size];
    d_mode = true;
    d_minf = false;
    d_mink = 0LL;
    d_maxf = false;
    d_maxk = 0LL;
    reset ();
  }

  // create a new heap by mode
  
  Heap::Heap (const bool mode) {
    d_size = HEAP_SIZE;
    p_heap = new s_heap[d_size];
    d_mode = mode;
    d_minf = false;
    d_mink = 0LL;
    d_maxf = false;
    d_maxk = 0LL;
    reset ();
  }

  // create a new heap by size and mode
  
  Heap::Heap (const long size, const bool mode) {
    d_size = (size > 0) ? size : HEAP_SIZE;
    p_heap = new s_heap[d_size];
    d_mode = mode;
    d_minf = false;
    d_mink = 0LL;
    d_maxf = false;
    d_maxk = 0LL;
    reset ();
  }

  // destroy this heap

  Heap::~Heap (void) {
    delete [] p_heap;
  }

  // return the class name

  String Heap::repr (void) const {
    return "Heap";
  }

  // reset this heap class

  void Heap::reset (void) {
    wrlock ();
    try {
      d_hlen = 0;
      for (long i = 0; i < d_size; i++) p_heap[i].reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the heap mode

  void Heap::setmode (const bool mode) {
    wrlock ();
    try {
      d_mode = mode;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the heap mode
  
  bool Heap::getmode (void) const {
    rdlock ();
    try {
      bool result = d_mode;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the min flag
  
  bool Heap::ismink (void) const {
    rdlock ();
    try {
      bool result = d_minf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the max flag
  
  bool Heap::ismaxk (void) const {
    rdlock ();
    try {
      bool result = d_maxf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset the min flag

  void Heap::rstmink (void) {
    wrlock ();
    try {
      d_minf = false;
      d_mink = 0LL;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset the max flag

  void Heap::rstmaxk (void) {
    wrlock ();
    try {
      d_maxf = false;
      d_maxk = 0LL;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the min key value

  void Heap::setmink (const t_long mink) {
    wrlock ();
    try {
      d_minf = true;
      d_mink = mink;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the min key value

  t_long Heap::getmink (void) const {
    rdlock ();
    try {
      t_long result = d_mink;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the max key value

  void Heap::setmaxk (const t_long maxk) {
    wrlock ();
    try {
      d_maxf = true;
      d_maxk = maxk;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the max key value

  t_long Heap::getmaxk (void) const {
    rdlock ();
    try {
      t_long result = d_maxk;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the heap length

  long Heap::length (void) const {
    rdlock ();
    try {
      long result = d_hlen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the heap is empty

  bool Heap::empty (void) const {
    rdlock ();
    try {
      bool result = (d_hlen == 0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an object by key

  void Heap::add (const t_long key, Object* obj) {
    // check the object and lock
    if (obj == nullptr) return;
    wrlock ();
    // check limits first
    if (((d_minf == true) && (key <= d_mink)) ||
	((d_maxf == true) && (d_maxk <= key))) {
      unlock ();
      return;
    }
    try {
      // check if we need to resize
      if (d_hlen == d_size) resize (d_size * 2);
      // save the node position
      long npos = d_hlen;
      // place the element at the end
      p_heap[d_hlen++].set (key, obj);
      // balance the tree
      while (npos > 0) {
	// compute parent index
	long ppos = bt_pidx (npos);
	// check if the tree condition is satisfied
	if (cmp_node (d_mode, p_heap[ppos], p_heap[npos]) == true) break;
	// here the node need to be swaped
	s_heap  node = p_heap[npos];
	p_heap[npos] = p_heap[ppos];
	p_heap[ppos] = node;
	// udate the node position
	npos = ppos;
      }
      unlock  ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pop an object frm the heap

  Object* Heap::pop (void) {
    wrlock ();
    try {
      // check that the heap is not empty
      if (d_hlen == 0) {
	throw Exception ("heap-error", "trying to pop an empty heap");
      }
      // save the root object
      Object* result = p_heap[0].p_obj;
      Object::iref (result);
      // place the last object in the root node
      p_heap[0] = p_heap[--d_hlen];
      // reset the last object
      p_heap[d_hlen].reset ();
      // initialize node position and balance the tree
      long npos = 0;
      while (true) {
	// get the left and right child index
	long lpos = bt_lidx (npos);
	long rpos = bt_ridx (npos);
	// preset the swap index
	long spos = npos;
	// check for out-of-bound condition
	if (lpos >= d_hlen) break;
	//  check with right index
	if (rpos < d_hlen) {
	  // check if the tree condition is satisfied
	  if ((cmp_node (d_mode, p_heap[npos], p_heap[lpos]) == true) &&
	      (cmp_node (d_mode, p_heap[npos], p_heap[rpos]) == true)) break;
	  // update the node index for swaping
	  spos = cmp_node (d_mode, p_heap[lpos], p_heap[rpos]) ? lpos : rpos;
	} else {
	  // check if the tree condition is satisfied
	  if (cmp_node (d_mode, p_heap[npos], p_heap[lpos]) == true) break;
	  // update the node index for swaping
	  spos = lpos;
	}
	// double security - should not occur
	if (npos == spos) break;
	// swap the node
	s_heap  node = p_heap[npos];
	p_heap[npos] = p_heap[spos];
	p_heap[spos] = node;
	// update the index
	npos = spos;
      }
      Object::tref (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the root key

  t_long Heap::getkey (void) const {
    rdlock ();
    try {
      // check that the heap is not empty
      if (d_hlen == 0) {
	throw Exception ("heap-error", "trying to access an empty heap");
      }
      // get the root key
      t_long result = p_heap[0].d_key;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the heap key by index

  t_long Heap::getkey (const long index) const {
    rdlock ();
    try {
      if ((index < 0) || (index >= d_hlen)) {
	throw Exception ("index-error", "index is out of range");
      }
      t_long result = p_heap[index].d_key;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the heap object by index

  Object* Heap::getobj (const long index) const {
    rdlock ();
    try {
      if ((index < 0) || (index >= d_hlen)) {
	throw Exception ("index-error", "index is out of range");
      }
      Object* result = p_heap[index].p_obj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resize this heap

  void Heap::resize (const long size) {
    wrlock ();
    try {
      // check the new size
      if (size < d_hlen) {
	unlock ();
	return;
      }
      // copy the old array in the new one
      s_heap* heap = new s_heap[size];
      for (long i = 0; i < d_hlen; i++) heap[i] = p_heap[i];
      // finish the initialization
      for (long i = d_hlen; i < size; i++) heap[i].reset ();
      // clean and update indexes
      delete [] p_heap;
      p_heap = heap;
      d_size = size;
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
  static const long QUARK_ZONE_LENGTH = 17;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_POP     = zone.intern ("pop");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_EMPTYP  = zone.intern ("empty-p");
  static const long QUARK_GETKEY  = zone.intern ("get-key");
  static const long QUARK_GETOBJ  = zone.intern ("get-object");
  static const long QUARK_RESIZE  = zone.intern ("resize");
  static const long QUARK_GETMODE = zone.intern ("get-mode");
  static const long QUARK_ISMINKP = zone.intern ("min-key-p");
  static const long QUARK_ISMAXKP = zone.intern ("max-key-p");
  static const long QUARK_SETMINK = zone.intern ("set-min-key");
  static const long QUARK_GETMINK = zone.intern ("get-min-key");
  static const long QUARK_SETMAXK = zone.intern ("set-max-key");
  static const long QUARK_GETMAXK = zone.intern ("get-max-key");
  static const long QUARK_RSTMINK = zone.intern ("reset-min-key");
  static const long QUARK_RSTMAXK = zone.intern ("reset-max-key");
  
  // create a new object in a generic way

  Object* Heap::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check 0 argument
    if (argc == 0) return new Heap;
    // check 1 argument
    if (argc == 1) {
       Object* obj = argv->get (0);
      // check for an integer
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj !=nullptr) {
	long size = iobj->tolong ();
	return new Heap (size);
      }
      // check for a boolean
      Boolean* bobj = dynamic_cast <Boolean*> (obj);
      if (bobj !=nullptr) {
	bool mode = bobj->tobool ();
	return new Heap (mode);
      }
      throw Exception ("type-error", "invalid object with heap",
		       Object::repr (obj));
    }
    // check for 2 arguments
    if (argc == 2) {
      long size = argv->getlong (0);
      bool mode = argv->getbool (1);
      return new Heap (size, mode);
    }
    throw Exception ("argument-error", "too many argument for heap");
  }

  // return true if the given quark is defined
  
  bool Heap::isquark (const long quark, const bool hflg) const {
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
  
  Object* Heap::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH)  return new Integer (length  ());
      if (quark == QUARK_EMPTYP)  return new Boolean (empty   ());
      if (quark == QUARK_GETMODE) return new Boolean (getmode ());
      if (quark == QUARK_GETKEY)  return new Integer (getkey  ());
      if (quark == QUARK_ISMINKP) return new Boolean (ismink  ());
      if (quark == QUARK_ISMAXKP) return new Boolean (ismaxk  ());
      if (quark == QUARK_GETMINK) return new Integer (getmink ());
      if (quark == QUARK_GETMAXK) return new Integer (getmaxk ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_RSTMINK) {
	rstmink ();
	return nullptr;
      }
      if (quark == QUARK_RSTMAXK) {
	rstmaxk ();
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
      if (quark == QUARK_GETKEY) {
	long index = argv->getlong (0);
	return new Integer (getkey (index));
      }
      if (quark == QUARK_GETOBJ) {
	long index = argv->getlong (0);
	rdlock ();
	try {
	  Object* result = getobj (index);
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
      if (quark == QUARK_SETMINK) {
	t_long mink = argv->getlong (0);
	setmink (mink);
	return nullptr;
      }
      if (quark == QUARK_SETMAXK) {
	t_long maxk = argv->getlong (0);
	setmaxk (maxk);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_ADD) {
	t_long  key = argv->getlong (0);
	Object* obj = argv->get    (1);
	add (key, obj);
	return nullptr;
      }
    }
    // apply these arguments with the object
    return Object::apply (zobj, nset, quark, argv);
  }
}
