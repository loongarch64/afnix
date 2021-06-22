// ---------------------------------------------------------------------------
// - QuarkTable.cpp                                                          -
// - standard object library - quark table class implementation              -
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

#include "Prime.hpp"
#include "QuarkTable.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the quark table qnode
  struct s_quanode {
    // the object quark
    long d_quark;
    // the object 
    Object* p_object;
    // next record in the list
    s_quanode* p_next;
    // simple constructor
    s_quanode (void) {
      d_quark  = 0;
      p_object = nullptr;
      p_next   = nullptr;
    }
    // simple destructor
    ~s_quanode (void) {
      Object::dref (p_object);
      delete p_next;
    }
  };
  
  // find a qnode by quark given its root qnode
  static inline s_quanode* getqnode (s_quanode* qnode, const long quark) {
    // simple check as fast as we can
    if (qnode == nullptr) return nullptr;
    // loop until we have a match
    while (qnode != nullptr) {
      if (qnode->d_quark == quark) return qnode;
      qnode = qnode->p_next;
    }
    // no quark node found
    return nullptr;
  }
  
  // extract a quark node by quark given its root node
  static inline s_quanode* rmqnode (s_quanode** root, const long quark) {
    s_quanode* qnode = *root;
    // simple check as fast as we can
    if (qnode == nullptr) return nullptr;
    // first case for the root qnode
    if (qnode->d_quark == quark) {
      *root = qnode->p_next;
      qnode->p_next = nullptr;
      return qnode;
    }
    // loop until we have a match
    while (qnode->p_next != nullptr) {
      if (qnode->p_next->d_quark == quark) {
	s_quanode* result = qnode->p_next;
	qnode->p_next = result->p_next;
	result->p_next = nullptr;
	return result;
      }
      qnode = qnode->p_next;
    } 
    // no node found
    return nullptr;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a new quark table 
  
  QuarkTable::QuarkTable (void) {
    // build the array
    d_size   = Prime::mkthrp (0);
    d_thrs   = (d_size * 7) / 10;
    d_count  = 0;
    p_table  = new s_quanode*[d_size];
    for (long i = 0; i < d_size; i++) p_table[i] = nullptr;
  }
  
  // create a new quark table with a predefined size
  
  QuarkTable::QuarkTable (const long size) {
    // build the array - threshold at 70%
    d_size   = Prime::mkthrp (size);
    d_thrs   = (size * 7) / 10;
    d_count  = 0;
    p_table  = new s_quanode*[d_size];    
    for (long i = 0; i < d_size; i++) p_table[i] = nullptr;
  }
  
  // delete this quark table 
  
  QuarkTable::~QuarkTable (void) {
    // protect ourself
    Object::iref (this);
    if (p_table != nullptr) {
      for (long i = 0; i < d_size; i++) delete p_table[i];
      delete [] p_table;
    }
  }

  // return the class name

  String QuarkTable::repr (void) const {
    return "QuarkTable";
  }

  // clear this quark table
  
  void QuarkTable::reset (void) {
    // protect us and get the write lock
    Object::iref (this);
    wrlock ();
    // clear everything
    if (p_table != nullptr) {
      for (long i = 0; i < d_size; i++) {
	delete p_table[i];
	p_table[i] = nullptr;
      }
    }
    d_count = 0;
    // release lock and protection
    Object::tref (this);
    unlock ();
  }  

  // get the number of elements

  long QuarkTable::length (void) const {
    rdlock ();
    long result = d_count;
    unlock ();
    return result;
  }

  // get the element name by index

  String QuarkTable::getname (const long index) const {
    rdlock ();
    long npos = 0;
    for (long i = 0; i < d_size; i++) {
      s_quanode* node = p_table[i];
      while (node != nullptr) {
	if (npos == index) {
	  String result = String::qmap (node->d_quark);
	  unlock ();
	  return result;
	}
	npos++;
	node = node->p_next;
      }
    }
    unlock ();
    throw Exception ("index-error", "index is out of range");
  }

  // get the element object by index

  Object* QuarkTable::getobj (const long index) const {
    rdlock ();
    long npos = 0;
    for (long i = 0; i < d_size; i++) {
      s_quanode* node = p_table[i];
      while (node != nullptr) {
	if (npos == index) {
	  Object* result = node->p_object;
	  unlock ();
	  return result;
	}
	npos++;
	node = node->p_next;
      }
    }
    unlock ();
    throw Exception ("index-error", "index is out of range");
  }

  // set or create an object in this table
  
  void QuarkTable::add (const long quark, Object* object) {
    // protect the object
    Object::iref (object);
    // get the write lock
    wrlock ();
    // compute the table id
    long hid = quark % d_size;
    // look for existing symbol
    s_quanode* qnode = getqnode (p_table[hid],quark);
    if (qnode != nullptr) {
      Object::dref (qnode->p_object);
      qnode->p_object = object;
      unlock ();
      return;
    }
    // the quark node does not exist, create it 
    qnode           = new s_quanode;
    qnode->d_quark  = quark;
    qnode->p_object = object;
    qnode->p_next   = p_table[hid];
    p_table[hid]     = qnode;
    if (++d_count > d_thrs) resize (Prime::mkthrp (d_size + 1));
    unlock ();
  }
  
  // get an object with a read lock by quark.
  
  Object* QuarkTable::get (const long quark) const {
    // get the read lock
    rdlock ();
    // compute table id
    long hid = quark % d_size;
    // look for the node and get the object
    s_quanode* qnode = getqnode (p_table[hid],quark);
    Object* result = (qnode == nullptr) ? nullptr : qnode->p_object;
    // unlock and return
    unlock ();
    return result;
  }

  // get an object by quark. If the quark is not found an exception is raised

  Object* QuarkTable::lookup (const long quark) const {
    // get the read lock
    rdlock ();    
    // compute table id
    long hid = quark % d_size;
    // look for the node and find symbol
    s_quanode* qnode = getqnode (p_table[hid],quark);
    if (qnode != nullptr) {
      Object* result =  qnode->p_object;
      unlock ();
      return result;
    }
    // unlock and notify
    unlock ();
    throw Exception ("name-error", "name not found", String::qmap (quark));
  }
  
  // return true if a quark exists in this table

  bool QuarkTable::exists (const long quark) const {
    // get the read lock
    rdlock ();
    // compute table id
    long hid = quark % d_size;
    // look for the node and find symbol
    s_quanode* qnode = getqnode (p_table[hid],quark);
    unlock ();
    return (qnode != nullptr);
  }
  
  // remove an object by quark
  
  void QuarkTable::remove (const long quark) {
    // get the write lock
    wrlock ();
    // compute table id
    long hid = quark % d_size;    
    // extract the qnode and delete
    s_quanode* qnode = rmqnode (&p_table[hid],quark);
    delete qnode;
    d_count--;
    // release the write lock
    unlock ();
  }
  
  // resize the quark table by creating a new one.
  // no need to lock here since the procedure is private and called
  // from the critical region
  
  void QuarkTable::resize (const long size) {
    // check for the size
    if (size < d_size) return;

    // initialize the new table
    s_quanode** table = new s_quanode*[size];
    for (long i = 0; i < size; i++) table[i] = nullptr;

    // rebuild the table
    for (long i = 0; i < d_size; i++) {
      s_quanode* qnode = p_table[i];
      while (qnode != nullptr) {
	s_quanode* next = qnode->p_next;
	qnode->p_next = nullptr;
	long hid = qnode->d_quark  % size;
	qnode->p_next = table[hid];
	table[hid] = qnode;
	qnode = next;
      }
    }
    // clean the old table
    delete [] p_table;
    // restore the new table
    d_size  = size;
    d_thrs  = (d_size * 7) / 10;
    p_table = table;
  }
}
